#include "ADCTool.h"


ADCTool::ADCTool(word count){
  num_samples = count;
  samples_ready = false;  // shouldnt be needed
  // Now array length is known; allocate full samples[num_samples]
  samples = (word *) malloc(num_samples * sizeof(word));
}


void ADCTool::handler() {
  static volatile int count = 0;
  samples[count] = ADC->RESULT.bit.RESULT;
  if (++count == num_samples) {
    count = 0;
    samples_ready = true;
  }
  ADC->INTFLAG.bit.RESRDY = 1;  // Not needed if we read result register
}


// Wait for synchronization of registers between the clock domains
void ADCTool::syncADC() {
  while (ADC->STATUS.bit.SYNCBUSY == 1);
}


// setup Arduino ADC pin A0
void ADCTool::setupReadPin() {
  // This register allows the user to set one or more I/O pins as an input.
  REG_PORT_DIRCLR1 = PORT_PA02;
  // I think this is the same as:
  //   PORT->Group[0].DIRCLR.bit.DIRCLR = (1u << 2) // 2 is port number
  PORT->Group[0].PINCFG[3].bit.PMUXEN = 1;

  // Set Peripheral Multiplexing Even and Odd to Peripheral function B
  PORT->Group[0].PMUX[3].reg = PORT_PMUX_PMUXE_B | PORT_PMUX_PMUXO_B;
  // I think this is the same as:
  //   PORT->Group[0].PMUX[3].reg = (0x1u << 0) | (0x1u << 4);
}



// Enable the 8MHz clock used for the ADC
void ADCTool::enable8MHzClock() {
  SYSCTRL->OSC8M.reg |= SYSCTRL_OSC8M_ENABLE;
  // I think this is the same as:
  //   SYSCTRL->OSC8M.bit.ENABLE = 1;
}


// Setup generic clock for ADC using built in 8MHz clock
void ADCTool::setupGenericClock() {
  // Enable the APBC clock for the ADC
  REG_PM_APBCMASK |= PM_APBCMASK_ADC;
  // PM.APBCMASK.bit.ADC = 1;  // DS40001882C-page 164

  enable8MHzClock();

  // Setup clock GCLK3 for no (?)div factor
  GCLK->GENDIV.reg |= GCLK_GENDIV_ID(3)| GCLK_GENDIV_DIV(1);
  // Division Factor: These bits apply a division on each selected generic clock
  // generator. Writes to bits above the specified number will be ignored.
  // - Generic clock generator 0: 8 division factor bits - DIV[7:0]
  // - Generic clock generator 1: 16 division factor bits - DIV[15:0]
  // - Generic clock generators 2: 5 division factor bits - DIV[4:0]
  // - Generic clock generators 3-8: 8 division factor bits - DIV[7:0]
  //   GCLK->GENDIV.bit.DIV = 1; // ?? 16 division factor bits - DIV[15:0]
  // Generic Clock Generator Selection: These bits select the generic clock
  // generator on which the division factor will be applied
  //   GCLK->GENDIV.bit.ID = 0x3;  // Generic clock generator 3
  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

  // Configure the generator of the generic clock, which is 8MHz clock
  GCLK->GENCTRL.reg |= GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC8M | GCLK_GENCTRL_ID(3) | GCLK_GENCTRL_DIVSEL;
  // This bit is used to decide how the clock source used by the generic clock
  // generator will be divided. If the clock source should not be divided, the
  // DIVSEL bit must be zero and the GENDIV.DIV value for the corresponding
  // generic clock generator must be zero or one.
  //   0: The generic clock generator equals the clock source divided by GENDIV.DIV.
  //   1: The generic clock generator equals the clock source divided by 2^(GENDIV.DIV+1)
  //   GCLK->GENCTRL.bit.DIVSEL = 1;
  // This bit is used to enable and disable the generic clock generator.
  //   GCLK->GENCTRL.bit.GENEN = 1;
  // These bits define the clock source to be used as the source for the generic
  // clock generator.
  //   GCLK->GENCTRL.bit.SRC = 0x06;  // OSC8M oscillator output
  // These bits select the generic clock generator that will be configured/read.
  //   GCLK->GENCTRL.bit.ID = 0x3;  // Generic clock generator 3
  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

  // Enable clock, set gen clock number, and ID to where the clock goes
  GCLK->CLKCTRL.reg |= GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(3) | GCLK_CLKCTRL_ID(30);
  // GCLK->CLKCTRL.bit.CLKEN = 1;
  // GCLK->CLKCTRL.bit.GEN = 0x3;  // Generic clock generator 3
  // GCLK->CLKCTRL.bit.ID = 0x1E;  // "GCLK_ADC" (ADC)
  while (GCLK->STATUS.bit.SYNCBUSY);
}



void ADCTool::setupADC() {
  // Reference Control
  // Reference Selection: These bits select the reference for the ADC.
  ADC->REFCTRL.reg |= ADC_REFCTRL_REFSEL_INTVCC1; // VDDANA/2, combine with gain DIV2 for full VCC range
  // ADC->REFCTRL.bit.REFSEL = 0x2;  // INTVCC1: 1/2 VDDANA (only for VDDANA > 2.0V)

  // Average control 1 sample, no right-shift
  ADC->AVGCTRL.reg |= ADC_AVGCTRL_ADJRES(0) | ADC_AVGCTRL_SAMPLENUM_1;

  // Sampling Time Control
  //   These bits control the ADC sampling time in number of half CLK_ADC 
  //   cycles, depending of the prescaler value, thus controlling the ADC input
  //   impedance. Sampling time is set according to the equation:
  //     Sampling time= (SAMPLEN+1) * (CLK_ADC / 2)
  ADC->SAMPCTRL.bit.SAMPLEN = 0x1;
  //ADC->SAMPCTRL.bit.SAMPLEN = 0x2;

  // Gain Factor Selection:  These bits set the gain factor of the ADC gain stage.
  //   0x0: 1X (1x)
  //   0x1: 2X (2x)
  //   0x2: 4X (4x)
  //   0x3: 8X (8x)
  //   0x4: 16X (16x)
  //   0xF: DIV2 (1/2x)
  ADC->INPUTCTRL.bit.GAIN = 0xF;  // DIV2 (1/2x)
  // ADC->INPUTCTRL.bit.GAIN = 0x4;  // 16X (16x)

  // Positive Mux Setting Offset:
  ADC->INPUTCTRL.bit.INPUTOFFSET = 0;

  // Number of Input Channels Included in Scan:
  ADC->INPUTCTRL.bit.INPUTSCAN = 0;

  // Negative Mux Input Selection:
  ADC->INPUTCTRL.bit.MUXNEG = 0x18;  // GND (Internal ground)

  // Positive Mux Input Selection:
  //   0x00: PIN0 (ADC AIN0 pin)
  //   0x01: PIN1 (ADC AIN1 pin)
  //   0x02: PIN2 (ADC AIN2 pin)
  //   0x03: PIN3 (ADC AIN3 pin)
  //   0x04: PIN4 (ADC AIN4 pin)
  //   0x05: PIN5 (ADC AIN5 pin)
  //   0x06: PIN6 (ADC AIN6 pin)
  //   0x1A: SCALEDCOREVCC (1/4 scaled core supply)
  //   0x1B: SCALEDIOVCC (1/4 scaled I/O supply)
  ADC->INPUTCTRL.bit.MUXPOS = 0x00;  // PIN0 (ADC AIN0 pin)

  syncADC();

  // Prescaler Configuration: 
  //   These bits define the ADC clock relative to the peripheral clock.
  //   0x0: DIV4 (Peripheral clock divided by 4)
  //   0x1: DIV8 (Peripheral clock divided by 8)
  //   0x2: DIV16 (Peripheral clock divided by 16)
  //   0x3: DIV32 (Peripheral clock divided by 32)
  //   0x4: DIV64 (Peripheral clock divided by 64)
  //   0x5: DIV128 (Peripheral clock divided by 128)
  //   0x6: DIV256 (Peripheral clock divided by 256)
  //   0x7: DIV512 (Peripheral clock divided by 512)
  // ADC->CTRLB.bit.PRESCALER = 0x0;  // DIV4 (Peripheral clock divided by 4)
  ADC->CTRLB.bit.PRESCALER = 0x2;  // DIV16: Peripheral clock divided by 16

  // Conversion Result Resolution:
  //   These bits define whether the ADC completes the conversion at 12-, 10- or
  //   8-bit result resolution.
  //   0x0: 12BIT (12-bit result)
  //   0x1: 16BIT (For averaging mode output
  //   0x2: 10BIT (10-bit result)
  //   0x3: 8BIT (8-bit result)
  ADC->CTRLB.bit.RESSEL = 0x0;  // 12BIT (12-bit result)

  // Digital Correction Logic Enabled:
  //   Enable the digital result correction. The ADC conversion result in the
  //   RESULT register is then corrected for gain and offset based on the values
  //   in the GAINCAL and OFFSETCAL registers. Conversion time will be increased
  //   by X cycles according to the value in the Offset Correction Value bit
  //   group in the Offset Correction register.
  ADC->CTRLB.bit.CORREN = 0;

  // Free Running Mode:
  //   The ADC is in free running mode and a new conversion will be initiated
  //   when a previous conversion completes.
  ADC->CTRLB.bit.FREERUN = 1;

  // Left-Adjusted Result:
  //   0: The ADC conversion result is right-adjusted in the RESULT register.
  //   1: The ADC conversion result is left-adjusted in the RESULT register. The
  //   high byte of the 12- bit result will be present in the upper part of the
  //   result register. Writing this bit to zero (default) will right-adjust the
  //   value in the RESULT register.
  ADC->CTRLB.bit.LEFTADJ = 0;

  // Differential Mode
  //   0: The ADC is running in singled-ended mode.
  //   1: The ADC is running in differential mode. In this mode, the voltage
  //   difference between the MUXPOS and MUXNEG inputs will be converted by the
  //   ADC.
  ADC->CTRLB.bit.DIFFMODE = 0;

  syncADC();
}


// Setup ADC interrupt on result ready with given priority.
// (0 is highest priority)
void ADCTool::setupInterrupt(byte priority) {
  ADC->INTENSET.bit.RESRDY = 1;  // Result Ready Interrupt Enable
  syncADC();
  NVIC_EnableIRQ(ADC_IRQn); // enable ADC interrupts
  NVIC_SetPriority(ADC_IRQn, priority); //set priority of the interrupt
}



void ADCTool::enableADC() {
  ADC->CTRLA.bit.ENABLE = 1;
  // Following required for free running mode?
  ADC->SWTRIG.bit.START = 1;
}


void ADCTool::disableADC() {
  ADC->CTRLA.bit.ENABLE = 0;
}


void ADCTool::stopSampling() {
  NVIC_DisableIRQ(ADC_IRQn);  // turn off interrupt
  disableADC();
}


//this function sets up the ADC and interrupts and starts
void ADCTool::startSampling() {
  setupReadPin();
  setupGenericClock();
  setupADC();
  setupInterrupt(0); // with highest priority
  enableADC();
}
