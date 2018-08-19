/*

Pretty good videos :
https://forcetronic.blogspot.com/2016/10/utilizing-advanced-adc-capabilities-on.html

Using application note AT03243: SAM D/R Analog to Digital Converter (ADC) Driver
http://ww1.microchip.com/downloads/en/AppNotes/Atmel-42109-SAM-Analog-to-Digital-Converter-ADC-Driver_ApplicationNote_AT03243.pdf

The ADC has up to 12-bit resolution, and is capable of converting up to 500K samples per second (KSPS)
The ADC may be configured for 8-, 10-, or 12-bit result. Using oversampling and decimation
mode the ADC resolution is increased from 12-bit to an effective 13-, 14-, 15-, or 16-bit.
The ADC can provide both signed and unsigned results
Note: Internal references will be enabled by the driver, but not disabled. Any reference not used by the
application should be disabled by the application
free running mode: new conversions are started as soon as the previous conversion is completed
prescaler: enables conversion at lower clock rates than the input Generic Clock to the ADC module
he ADC has two conversion modes; differential and single-ended. When measuring signals where the
positive input pin is always at a higher voltage than the negative input pin, the single-ended conversion
mode should be used in order to achieve a full 12-bit output resolution.
sampling time = (sample_length + 1) * (ADCclk / 2)

Using this function in ASF: void adc_get_config_defaults(struct adc_config * const config)
The default configuration is as follows:
*   GCLK generator 0 (GCLK main) clock source
*   1V from internal bandgap reference
*   Div 4 clock prescaler
*   12-bit resolution
*   Window monitor disabled
*   No gain
*   Positive input on ADC PIN 0
*   Negative input on ADC PIN 1
*   Averaging disabled
*   Oversampling disabled
*   Right adjust data
*   Single-ended mode
*   Free running disabled
*   All events (input and generation) disabled
*   Sleep operation disabled
*   No reference compensation
*   No gain/offset correction
*   No added sampling time
*   Pin scan mode disabled

I am not certain this is still the default configuration when using the Arduino IDE.



ADC_IRQn: ADC interrupt number definition


Cortex-M0 Nested Vectored Interrupt Controller (NVIC) supports:
* An implementation-defined number of interrupts, in the range 1-32.
* A programmable priority level of 0-192 in steps of 64 for each interrupt. A higher level corresponds to a lower priority, so level 0 is the highest interrupt priority.
* Level and pulse detection of interrupt signals.
* Interrupt tail-chaining.
* An external NMI.
The processor automatically stacks its state on exception entry and unstacks this state on exception exit, with no instruction overhead. This provides low latency exception handling. The hardware implementation of the NVIC registers is:

Table 4.2. NVIC register summary
Address Name  Type  Reset value  Description
0xE000E100    ISER  RW  0x00000000  Interrupt Set-enable Register
0xE000E180  ICER  RW  0x00000000  Interrupt Clear-enable Register
0xE000E200  ISPR  RW  0x00000000  Interrupt Set-pending Register
0xE000E280  ICPR  RW  0x00000000  Interrupt Clear-pending Register
0xE000E400-0xE000E41C IPR0-7  RW  0x00000000  Interrupt Priority Registers
http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0497a/CIHIGCIF.html




ADC members:
int *base = (int*)(0x42004000UL);
for (int i = 0; i < 256; i++){ Serial.println(base[i], HEX); }
3F000200
720
0
0
F001800
0
8
0
0
0
388
0
0
0
0
0
3F000200
720
0
0
F001800
0
8
0
0
0
388

Default values binary:
ADC->CTRLA = 111111000000000000001000000000
ADC->CTRLB = 11100100000
ADC->WINCTRL = 0
ADC->SWTRIG = 0
ADC->INPUTCTRL = 1111000000000001100000000000
ADC->EVCTRL = 0
ADC->INTFLAG = 1000
ADC->WINLT = 0
ADC->WINUT = 0
ADC->GAINCORR = 0
ADC->CALIB = 1110001000

Default values hex:
ADC->CTRLA = 3F000200
ADC->CTRLB = 720
ADC->WINCTRL = 0
ADC->SWTRIG = 0
ADC->INPUTCTRL = F001800
ADC->EVCTRL = 0
ADC->INTFLAG = 8
ADC->WINLT = 0
ADC->WINUT = 0
ADC->GAINCORR = 0
ADC->CALIB = 388

Serial.print("ADC->CTRLA = "); Serial.println(*((int *) &ADC->CTRLA), HEX);
//Serial.print("ADC->REFCTRL = "); Serial.println(*((int *) &ADC->REFCTRL), HEX);
//Serial.print("ADC->AVGCTRL = "); Serial.println(*((int *) &ADC->AVGCTRL), HEX);
//Serial.print("ADC->SAMPCTRL = "); Serial.println(*((int *) &ADC->SAMPCTRL), HEX);
Serial.print("ADC->CTRLB = "); Serial.println(*((int *) &ADC->CTRLB), HEX);
Serial.print("ADC->WINCTRL = "); Serial.println(*((int *) &ADC->WINCTRL), HEX);
Serial.print("ADC->SWTRIG = "); Serial.println(*((int *) &ADC->SWTRIG), HEX);
Serial.print("ADC->INPUTCTRL = "); Serial.println(*((int *) &ADC->INPUTCTRL), HEX);
Serial.print("ADC->EVCTRL = "); Serial.println(*((int *) &ADC->EVCTRL), HEX);
//Serial.print("ADC->INTENCLR = "); Serial.println(*((int *) &ADC->INTENCLR), HEX);
//Serial.print("ADC->INTENSET = "); Serial.println(*((int *) &ADC->INTENSET), HEX);
Serial.print("ADC->INTFLAG = "); Serial.println(*((int *) &ADC->INTFLAG), HEX);
//Serial.print("ADC->STATUS = "); Serial.println(*((int *) &ADC->STATUS), HEX);
//Serial.print("ADC->RESULT = "); Serial.println(*((int *) &ADC->RESULT), HEX);
Serial.print("ADC->WINLT = "); Serial.println(*((int *) &ADC->WINLT), HEX);
Serial.print("ADC->WINUT = "); Serial.println(*((int *) &ADC->WINUT), HEX);
Serial.print("ADC->GAINCORR = "); Serial.println(*((int *) &ADC->GAINCORR), HEX);
//Serial.print("ADC->OFFSETCORR = "); Serial.println(*((int *) &ADC->OFFSETCORR), HEX);
Serial.print("ADC->CALIB = "); Serial.println(*((int *) &ADC->CALIB), HEX);
//Serial.print("ADC->DBGCTRL = "); Serial.println(*((int *) &ADC->DBGCTRL), HEX);

Addresses:
ADC->CTRLA = 42004000
ADC->REFCTRL = 42004001
ADC->AVGCTRL = 42004002
ADC->SAMPCTRL = 42004003
ADC->CTRLB = 42004004
ADC->WINCTRL = 42004008
ADC->SWTRIG = 4200400C
ADC->INPUTCTRL = 42004010
ADC->EVCTRL = 42004014
ADC->INTENCLR = 42004016
ADC->INTENSET = 42004017
ADC->INTFLAG = 42004018
ADC->STATUS = 42004019
ADC->RESULT = 4200401A
ADC->WINLT = 4200401C
ADC->WINUT = 42004020
ADC->GAINCORR = 42004024
ADC->OFFSETCORR = 42004026
ADC->CALIB = 42004028
ADC->DBGCTRL = 4200402A

ADC->CTRLA.bit.SWRST = 0
ADC->CTRLA.bit.ENABLE = 0
ADC->CTRLA.bit.RUNSTDBY = 0
ADC->REFCTRL.bit.REFSEL = 10
ADC->REFCTRL.bit.REFCOMP = 0
ADC->AVGCTRL.bit.SAMPLENUM = 0
ADC->AVGCTRL.bit.ADJRES = 0
ADC->SAMPCTRL.bit.SAMPLEN = 111111
ADC->CTRLB.bit.DIFFMODE = 0
ADC->CTRLB.bit.LEFTADJ = 0
ADC->CTRLB.bit.FREERUN = 0
ADC->CTRLB.bit.CORREN = 0
ADC->CTRLB.bit.RESSEL = 10
ADC->CTRLB.bit.PRESCALER = 111
ADC->WINCTRL.bit.WINMODE = 0
ADC->SWTRIG.bit.FLUSH = 0
ADC->SWTRIG.bit.START = 0
ADC->INPUTCTRL.bit.MUXPOS = 0
ADC->INPUTCTRL.bit.MUXNEG = 11000
ADC->INPUTCTRL.bit.INPUTSCAN = 0
ADC->INPUTCTRL.bit.INPUTOFFSET = 0
ADC->INPUTCTRL.bit.GAIN = 1111
ADC->EVCTRL.bit.STARTEI = 0
ADC->EVCTRL.bit.SYNCEI = 0
ADC->EVCTRL.bit.RESRDYEO = 0
ADC->EVCTRL.bit.WINMONEO = 0
ADC->INTENCLR.bit.RESRDY = 0
ADC->INTENCLR.bit.OVERRUN = 0
ADC->INTENCLR.bit.WINMON = 0
ADC->INTENCLR.bit.SYNCRDY = 0
ADC->INTENSET.bit.RESRDY = 0
ADC->INTENSET.bit.OVERRUN = 0
ADC->INTENSET.bit.WINMON = 0
ADC->INTENSET.bit.SYNCRDY = 0
ADC->INTFLAG.bit.RESRDY = 0
ADC->INTFLAG.bit.OVERRUN = 0
ADC->INTFLAG.bit.WINMON = 0
ADC->INTFLAG.bit.SYNCRDY = 1
ADC->STATUS.bit.SYNCBUSY = 0
ADC->RESULT.bit.RESULT = 0
ADC->WINLT.bit.WINLT = 0 // window mode lower tolerance
ADC->WINUT.bit.WINUT = 0 // window mode upper tolerance
ADC->GAINCORR.bit.GAINCORR = 0
ADC->OFFSETCORR.bit.OFFSETCORR = 0
ADC->CALIB.bit.LINEARITY_CAL = 10001000
ADC->CALIB.bit.BIAS_CAL = 11
ADC->DBGCTRL.bit.DBGRUN = 0

*/

//#include "wiring_private.h"


volatile int bufn,obufn;
const word FFT_N = 512;
// Create a buffer for ADC samples to be stored in by the driver asynchronously.
word adc_result_buffers[FFT_N/128+1][128];
volatile int count = 0; // tracks how many samples we have collected


// Wait for synchronization of registers between the clock domains
static void syncADC() {
  // ADC->STATUS.bit.SYNCBUSY (Synchronization Busy)

  // This bit is cleared when the synchronization of registers between the clock
  // domains is complete. This bit is set when the synchronization of registers
  // between clock domains is started.

  while (ADC->STATUS.bit.SYNCBUSY == 1);
  // while (REG_ADC_STATUS & ADC_STATUS_SYNCBUSY);
}

// void ADC_Handler(void){
//    PORT->Group[PORTA].OUTTGL.reg = PORT_PA21;
//    if (ADC->INTFLAG.bit.RESRDY && ADC->INTENSET.bit.RESRDY) {  // A overflow caused the interrupt
//          ADCresult = ADC->RESULT.reg;
//          ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;   // reset interrupt flag
//     }
// }


// void ADC_Handler(){
//   int f=ADC->ADC_ISR;
//   if (f&(1<<27)){
//     bufn=(bufn+1);if(bufn==FFT_N/128+1)bufn=0;
//     ADC->ADC_RNPR=(uint32_t)buf[bufn];
//     ADC->ADC_RNCR=128;
//   }
// }


// Interrupt service routine (ISR) that runs when ADC result ready.
// The name ADC_Handler is defined in samd21g18a.h and is this required.
void ADC_Handler() {

  adc_result_buffers[bufn][count] = ADC->RESULT.bit.RESULT;
  if(++count == 128) {
    count = 0;
    if(++bufn == FFT_N/128+1) {
      bufn = 0;
    }
  }

  // The "Interrupt Flag Status" register shows when an interrupt fires. The
  // relevant bit needs to be cleared as we handle the interrupt so the next
  // interrupt can fire. The datasheet says we don't need to explicly clear the
  // bit as long as we read the result register.

  // ADC->INTFLAG.bit.RESRDY (Result Ready)

  // This flag is cleared by writing a one to the flag or by reading the RESULT
  // register. This flag is set when the conversion result is available, and an
  // interrupt will be generated if INTENCLRSET.RESRDY is one. Writing a zero to
  // this bit has no effect. Writing a one to this bit clears the Result Ready
  // interrupt flag.

  // Based on these macros in adc.h:
  //   #define ADC_INTFLAG_WINMON          (0x1u << ADC_INTFLAG_WINMON_Pos)
  //   #define ADC_INTFLAG_RESRDY          (0x1u << ADC_INTFLAG_RESRDY_Pos)
  //   #define ADC_INTFLAG_OVERRUN         (0x1u << ADC_INTFLAG_OVERRUN_Pos)
  //   #define ADC_INTFLAG_SYNCRDY         (0x1u << ADC_INTFLAG_SYNCRDY_Pos)
  // We can set register, or just a bit, like this:
  //   ADC->INTFLAG.reg = ADC_INTFLAG_WINMON;
  //   ADC->INTFLAG.bit.WINMON = 1;

  // ADC->INTFLAG.reg = ADC_INTENSET_RESRDY;
  ADC->INTFLAG.bit.RESRDY = 1;  // Not needed if we read result register

}


// setup Arduino ADC pin A0
void setupReadPin() {
  // Ref: sam0/utils/cmsis/samd21/include/component/adc.h
  // Ref: sam0/utils/cmsis/samd21/include/instance/port.h
  // Ref: sam0/utils/cmsis/samd21/include/pio/samd21g18a.h
  // Ref: http://ww1.microchip.com/downloads/en/DeviceDoc/SAM-D21-Family-Datasheet-DS40001882C.pdf

  // Set input pin for ADC to Arduino A0 (PA02) and enable multiplexing
  // Grom DS40001882C-page 407:

  // The I/O pins of the device are controlled by PORT peripheral registers.
  // Each port pin has a corresponding bit in the Data Direction (DIR) and Data
  // Output Value (OUT) registers to enable that pin as an output and to define
  // the output state.

  // The direction of each pin in a PORT group is configured by the DIR
  // register. If a bit in DIR is set to '1', the corresponding pin is
  // configured as an output pin. If a bit in DIR is set to '0', the
  // corresponding pin is configured as an input pin.

  // When the direction is set as output, the corresponding bit in the OUT
  // register will set the level of the pin. If bit y in OUT is written to '1',
  // pin y is driven HIGH. If bit y in OUT is written to '0', pin y is driven
  // LOW. Pin configuration can be set by Pin Configuration (PINCFGy) registers,
  // with y=00, 01, ..31 representing the bit position.

  // The Data Input Value (IN) is set as the input value of a port pin with
  // resynchronization to the PORT clock. To reduce power consumption, these
  // input synchronizers can be clocked only when system requires reading the
  // input value, as specified in the SAMPLING field of the Control register
  // (CTRL). The value of the pin can always be read, whether the pin is
  // configured as input or output. If the Input Enable bit in the Pin
  // Configuration registers (PINCFGy.INEN) is '0', the input value will not be
  // sampled.

  // In PORT, the Peripheral Multiplexer Enable bit in the PINCFGy register
  // (PINCFGy.PMUXEN) can be written to '1' to enable the connection between
  // peripheral functions and individual I/O pins. The Peripheral Multiplexing n
  // (PMUXn) registers select the peripheral function for the corresponding pin.
  // This will override the connection between the PORT and that I/O pin, and
  // connect the selected peripheral signal to the particular I/O pin instead of
  // the PORT line bundle.

  // Multiplexed Signals

  // Each pin is by default controlled by the PORT as a general purpose I/O and
  // alternatively it can be assigned to one of the peripheral functions A, B,
  // C, D, E, F, G or H. To enable a peripheral function on a pin, the
  // Peripheral Multiplexer Enable bit in the Pin Configuration register
  // corresponding to that pin (PINCFGn.PMUXEN, n = 0-31) in the PORT must be
  // written to one. The selection of peripheral function A to H is done by
  // writing to the Peripheral Multiplexing Odd and Even bits in the Peripheral
  // Multiplexing register (PMUXn.PMUXE/O) in the PORT.

  // Operation

  // Each I/O pin Pxy can be controlled by the registers in PORT. Each PORT
  // group x has its own set of PORT registers, with a base address at byte
  // address (PORT + 0x80 * group index) (A corresponds to group index 0, B to
  // 1, etc...). Within that set of registers, the pin index is y, from 0 to 31.
  // Refer to  I/O Multiplexing and Considerations  for details on available pin
  // configuration and PORT groups.

  // Configuring Pins as Output

  // To use pin Pxy as an  output , write bit y of the DIR register to '1'. This
  // can also be done by writing bit y in the DIRSET register to '1' - this will
  // avoid disturbing the configuration of other pins in that group. The y bit
  // in the OUT register must be written to the desired output value. Similarly,
  // writing an OUTSET bit to '1' will set the corresponding bit in the OUT
  // register to '1'. Writing a bit in OUTCLR to '1' will set that bit in OUT to
  // zero. Writing a bit in OUTTGL to '1' will toggle that bit in OUT.

  // Configuring Pins as Input

  // To use pin Pxy as an  input , bit y in the DIR register must be written to
  // '0'. This can also be done by writing bit y in the DIRCLR register to '1' -
  // this will avoid disturbing the configuration of other pins in that group.
  // The input value can be read from bit y in register IN as soon as the INEN
  // bit in the Pin Configuration register (PINCFGy.INEN) is written to '1'. By
  // default, the input synchronizer is clocked only when an input read is
  // requested. This will delay the read operation by two cycles of the PORT
  // clock. To remove the delay, the input synchronizers for each PORT group of
  // eight pins can be configured to be always active, but this will increase
  // power consumption. This is enabled by writing '1' to the corresponding
  // SAMPLINGn bit field of the CTRL register, see CTRL.SAMPLING for details.

  // Using Alternative Peripheral Functions

  // To use pin Pxy as one of the available peripheral functions, the
  // corresponding PMUXEN bit of the PINCFGy register must be '1'. The PINCFGy
  // register for pin Pxy is at byte offset (PINCFG0 + y). The peripheral
  // function can be selected by setting the PMUXO or PMUXE in the PMUXn
  // register. The PMUXO/PMUXE is at byte offset PMUX0 + (y/2). The chosen
  // peripheral must also be configured and enabled.

  // Macros:
  // REG_PORT_DIRCLR1 (0x41004484U) // (PORT) Data Direction Clear 1
  // #define PORT_PA04 (1u << 4) // PORT Mask for PA04
  // #define PORT_PA02 (1u << 2) // PORT Mask for PA02
  // #define PORT_PMUX_PMUXE_Pos 0 // (PORT_PMUX) Peripheral Multiplexing Even
  // #define PORT_PMUX_PMUXO_Pos 4 // (PORT_PMUX) Peripheral Multiplexing Odd
  // #define PORT_PMUX_PMUXE_B_Val 0x1u // (PORT_PMUX) Peripheral function B selected
  // #define PORT_PMUX_PMUXO_B_Val 0x1u // (PORT_PMUX) Peripheral function B selected
  // #define PORT_PMUX_PMUXE_B (PORT_PMUX_PMUXE_B_Val << PORT_PMUX_PMUXE_Pos)
  // #define PORT_PMUX_PMUXO_B (PORT_PMUX_PMUXO_B_Val << PORT_PMUX_PMUXO_Pos)

  // Ports to analog in mapping on schematic:
  // https://www.arduino.cc/en/uploads/Main/MKR1000-schematic.pdf
  // https://forum.arduino.cc/index.php?topic=346731.msg2911485#msg2911485
  // A0 - PA02 - None
  // A1 - PB02 - None
  // A2 - PB03 - None
  // A3 - PA04 - TCC0/WO[0] (same channel as TCC0/WO[4])
  // A4 - PA05 - TCC0/WO[1] (same channel as TCC0/WO[5])
  // A5 - PA06 - TCC1/WO[0]
  // A6 - PA07 - TCC1/WO[1]
  // D0 - PA22 - TCC0/WO[4] (same channel as TCC0/WO[0])
  // D1 - PA23 - TCC0/WO[5] (same channel as TCC0/WO[1])
  // D2 - PA10 - TCC1/WO[0]
  // D3 - PA11 - TCC1/WO[1]
  // D4 - PB10 - TCC0/WO[4] (same channel as TCC0/WO[0])
  // D5 - PB11 - TCC0/WO[5] (same channel as TCC0/WO[1])
  // D6 - PA20 - TCC0/WO[6] (same channel as TCC0/WO[2])
  // D7 - PA21 - TCC0/WO[7] (same channel as TCC0/WO[3])
  // D8 - PA16 - TCC0/WO[6] (same channel as TCC0/WO[2]) on peripheral F,
  //             TCC2/WO[0] on peripheral E
  // D9 - PA17 - TCC0/WO[7] (same channel as TCC0/WO[3]) on peripheral F,
  //             TCC2/WO[1] on peripheral E
  // D10 - PA19 - TCCO/WO[3] (same channel as TCC0/WO[7])
  // D11 - PA08 - TCC1/WO[2] (same channel as TCC1/WO[0]) on peripheral F,
  //              TCC0/WO[0] on peripheral E
  // D12 - PA09 - TCC1/WO[3] (same channel as TCC1/WO[1]) on peripheral F,
  //              TCC0/WO[1] on peripheral E
  // D13 - PB22 - None
  // D14 - PB23 - None

  // Data Direction Clear

  // This register allows the user to set one or more I/O pins as an input,
  // without doing a read-modify-write operation. Changes in this register will
  // also be reflected in the Data Direction (DIR), Data Direction Toggle
  // (DIRTGL) and Data Direction Set (DIRSET) registers. The I/O pins are
  // assembled in pin groups (”PORT groups”) with up to 32 pins. Group 0
  // consists of the PA pins, group 1 is for the PB pins, etc. Each pin group
  // has its own PORT registers, with a 0x80 address spacing. For example, the
  // register address offset for the Data Direction (DIR) register for group 0
  // (PA00 to PA31) is 0x00, and the register address offset for the DIR
  // register for group 1 (PB00 to PB31) is 0x80.

  REG_PORT_DIRCLR1 = PORT_PA02;
  // I think this is the same as:
  //   PORT->Group[0].DIRCLR.bit.DIRCLR = (1u << 2) // 2 is port number

  // Enable the peripheral multiplexer selection set in the Peripheral
  // Multiplexing register (PMUXn) to enable alternative peripheral control over
  // an I/O pin direction and output drive value. Writing '1' to this bit
  // enables the peripheral selection in PMUXn to control the pad. In this
  // configuration, the physical pin state may still be read from the Data Input
  // Value register (IN) if PINCFGn.INEN is set.

  // All analog pin functions are on peripheral function B. Peripheral function
  // B must be selected to disable the digital control of the pin.

  // "3" is from Table 7-1. PORT Function Multiplexing for A, B, and C Variant Devices (should read "E, G, J"?) on DS40001882C-page 27:

  //   |   Pin   | I/O Pin | Periph B |
  //   | SAMD21E | I/O Pin |   ADC    |
  //   |---------|---------|----------|
  //   |    3    |  PA02   | AIN[0]   | <--
  //   |    4    |  PA03   | AIN[1]   |
  //   |    -    |  PB08   | AIN[2]   |
  //   |    -    |  PB09   | AIN[3]   |
  //   |    5    |  PA04   | AIN[4]   |

  PORT->Group[0].PINCFG[3].bit.PMUXEN = 1;

  // Select peripheral function B for odd (PMUXO) and even (PMUXE) pins in
  // group. TODO: I am not really sure what's going on here. The datasheet makes
  // me think PMUX[n] corresponds to PINCFG[n], but the Forcetronic example had
  // PINCFG[4] and PMUX[1].

  // Set Peripheral Multiplexing Even and Odd to Peripheral function B
  PORT->Group[0].PMUX[3].reg = PORT_PMUX_PMUXE_B | PORT_PMUX_PMUXO_B;

  // I think this is the same as:
  //   PORT->Group[0].PMUX[3].reg = (0x1u << 0) | (0x1u << 4);
}



// Enable the 8MHz clock used for the ADC
void enable8MHzClock() {
  // SYSCTRL – System Controller

  // The System Controller (SYSCTRL) provides a user interface to the clock
  // sources, brown out detectors, on-chip voltage regulator and voltage
  // reference of the device. Through the interface registers, it is possible to
  // enable, disable, calibrate and monitor the SYSCTRL sub- peripherals. All
  // sub-peripheral statuses are collected in the Power and Clocks Status
  // register (PCLKSR). They can additionally trigger interrupts upon status
  // changes through the INTENSET (INTENSET), INTENCLR (INTENCLR) and INTFLAG
  // (INTFLAG) registers. Additionally, BOD33 and BOD12 interrupts can be used
  // to wake up the device from Stand-by mode upon a programmed brown-out
  // detection.

  // The SYSCTRL gathers controls for all device oscillators and provides clock
  // sources to the Generic Clock Controller (GCLK). The available clock sources
  // are: XOSC, XOSC32K, OSC32K, OSCULP32K, OSC8M, DFLL48M and FDPLL96M.

  // 8MHz Internal Oscillator (OSC8M) – Fast startup – Output frequency fine
  // tuning – 4/2/1MHz divided output frequencies available – Calibration value
  // loaded from Flash Factory Calibration at reset

  // Digital Frequency Locked Loop (DFLL48M) – Internal oscillator with no
  // external components – 48MHz output frequency – Operates standalone as a
  // high-frequency programmable oscillator in Open-Loop mode – Operates as an
  // accurate frequency multiplier against a known frequency in Closed-Loop mode

  // The user must ensure that the OSC8M is fully disabled before enabling it,
  // and that the OSC8M is fully enabled before disabling it by reading
  // OSC8M.ENABLE.
  SYSCTRL->OSC8M.reg |= SYSCTRL_OSC8M_ENABLE;
  // I think this is the same as:
  //   SYSCTRL->OSC8M.bit.ENABLE = 1;
}

// Setup generic clock for ADC using built in 8MHz clock
void setupGenericClock() {
  // GCLK - Generic Clock Controller

  // Depending on the application, peripherals may require specific clock
  // frequencies to operate correctly. The Generic Clock controller GCLK
  // provides nine Generic Clock Generators that can provide a wide range of
  // clock frequencies.

  // Generators can be set to use different external and internal oscillators as
  // source. The clock of each Generator can be divided. The outputs from the
  // Generators are used as sources for the Generic Clock Multiplexers, which
  // provide the Generic Clock (GCLK_PERIPHERAL) to the peripheral modules, as
  // shown in Generic Clock Controller Block Diagram. The number of Peripheral
  // Clocks depends on how many peripherals the device has.

  // Note: The Generator 0 is always the direct source of the GCLK_MAIN signal.

  // PM – Power Manager

  // The Power Manager (PM) controls the reset, clock generation and sleep modes
  // of the device.

  // Utilizing a main clock chosen from a large number of clock sources from the
  // GCLK, the clock controller provides synchronous system clocks to the CPU
  // and the modules connected to the AHB and the APBx bus. The synchronous
  // system clocks are divided into a number of clock domains; one for the CPU
  // and AHB and one for each APBx. Any synchronous system clock can be changed
  // at run-time during normal operation. The clock domains can run at different
  // speeds, enabling the user to save power by running peripherals at a
  // relatively low clock frequency, while maintaining high CPU performance. In
  // addition, the clock can be masked for individual modules, enabling the user
  // to minimize power consumption.

  // Before entering the Stand-by Sleep mode the user must make sure that a
  // significant amount of clocks and peripherals are disabled, so that the
  // voltage regulator is not overloaded. This is because during Stand-by Sleep
  // mode the internal voltage regulator will be in Low-Power mode.

  // Various sleep modes are provided in order to fit power consumption
  // requirements. This enables the PM to stop unused modules in order to save
  // power. In active mode, the CPU is executing application code. When the
  // device enters a Sleep mode, program execution is stopped and some modules
  // and clock domains are automatically switched off by the PM according to the
  // Sleep mode. The application code decides which Sleep mode to enter and
  // when. Interrupts from enabled peripherals and all enabled reset sources can
  // restore the device from a Sleep mode to Active mode.

  // The PM also contains a reset controller to collect all possible reset
  // sources. It issues a device reset and sets the device to its initial state,
  // and allows the reset source to be identified by software.

  // The PM bus clock (CLK_PM_APB) can be enabled and disabled in the Power
  // Manager, and the default state of CLK_PM_APB can be found in  Peripheral
  // Clock Default State table in the  Peripheral Clock Masking section. If this
  // clock is disabled in the Power Manager, it can only be re-enabled by a
  // reset. A generic clock (GCLK_MAIN) is required to generate the main clock.
  // The clock source for GCLK_MAIN is configured by default in the Generic
  // Clock Controller, and can be reconfigured by the user if needed.

  // The APBx clock (CLK_APBX) is the root clock source used by modules
  // requiring a clock on the APBx bus. The APBx clock is always synchronous to
  // the CPU clock, but can be divided by a prescaler, and will run even when
  // the CPU clock is turned off. A clock gater is inserted from the common APB
  // clock to any APBx clock of a module on APBx bus.

  // It is possible to disable or enable the clock for a peripheral in the AHB
  // or APBx clock domain by writing the corresponding bit in the Clock Mask
  // register (APBxMASK) to zero or one. Refer to the table below for the
  // default state of each of the peripheral clocks.

  // Ref: sam0/utils/cmsis/samd21/include/instance/pm.h
  // Ref: sam0/utils/cmsis/samd21/include/component/pm.h
  // Ref: sam0/utils/cmsis/samd21/include/component/gclk.h

  // #define REG_PM_APBCMASK            (0x40000420U) // (PM) APBC Mask */
  // #define PM_APBCMASK_ADC_Pos         16           // (PM_APBCMASK) ADC APB Clock Enable
  // #define PM_APBCMASK_ADC             (0x1u << PM_APBCMASK_ADC_Pos)

  // Enable the APBC clock for the ADC
  REG_PM_APBCMASK |= PM_APBCMASK_ADC;
  // PM.APBCMASK.bit.ADC = 1;  // DS40001882C-page 164

  enable8MHzClock();


  // Before a Generator is enabled, the corresponding clock source should be
  // enabled. The Peripheral clock must be configured as outlined by the
  // following steps:

  // 1.  The Generic Clock Generator division factor must be set by performing a
  // single 32-bit write to the Generic Clock Generator Division register
  // (GENDIV): –  The Generic Clock Generator that will be selected as the
  // source of the generic clock by setting the ID bit group (GENDIV.ID). –  The
  // division factor must be selected by the DIV bit group (GENDIV.DIV)

  // #define GCLK_GENDIV_ID_Pos          0            // (GCLK_GENDIV) Generic Clock Generator Selection
  // #define GCLK_GENDIV_ID_Msk          (0xFu << GCLK_GENDIV_ID_Pos)
  // #define GCLK_GENDIV_ID(value)       ((GCLK_GENDIV_ID_Msk & ((value) << GCLK_GENDIV_ID_Pos)))
  // #define GCLK_GENDIV_DIV_Pos         8            // (GCLK_GENDIV) Division Factor
  // #define GCLK_GENDIV_DIV_Msk         (0xFFFFu << GCLK_GENDIV_DIV_Pos)
  // #define GCLK_GENDIV_DIV(value)      ((GCLK_GENDIV_DIV_Msk & ((value) << GCLK_GENDIV_DIV_Pos)))
  // #define GCLK_GENDIV_MASK            0x00FFFF0Fu  // (GCLK_GENDIV) MASK Register */

  // #define GCLK_STATUS_SYNCBUSY_Pos    7            // (GCLK_STATUS) Synchronization Busy Status
  // #define GCLK_STATUS_SYNCBUSY        (0x1u << GCLK_STATUS_SYNCBUSY_Pos)

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

  // 2.  The generic clock generator must be enabled by performing a single
  // 32-bit write to the Generic Clock Generator Control register (GENCTRL): –
  // The Generic Clock Generator will be selected as the source of the generic
  // clock by the ID bit group (GENCTRL.ID) –  The Generic Clock generator must
  // be enabled (GENCTRL.GENEN=1)

  // #define GCLK_GENCTRL_GENEN_Pos      16           // (GCLK_GENCTRL) Generic Clock Generator Enable
  // #define GCLK_GENCTRL_GENEN          (0x1u << GCLK_GENCTRL_GENEN_Pos)

  // #define GCLK_GENCTRL_SRC_Pos        8            // (GCLK_GENCTRL) Source Select
  // #define GCLK_GENCTRL_SRC_OSC8M_Val      0x6u   // (GCLK_GENCTRL) OSC8M oscillator output
  // #define GCLK_GENCTRL_SRC_OSC8M_Val      0x6u   // (GCLK_GENCTRL) OSC8M oscillator output
  // #define GCLK_GENCTRL_SRC_OSC8M      (GCLK_GENCTRL_SRC_OSC8M_Val    << GCLK_GENCTRL_SRC_Pos)

  // #define GCLK_GENCTRL_ID_Pos         0            // (GCLK_GENCTRL) Generic Clock Generator Selection
  // #define GCLK_GENCTRL_ID_Msk         (0xFu << GCLK_GENCTRL_ID_Pos)
  // #define GCLK_GENCTRL_ID(value)      ((GCLK_GENCTRL_ID_Msk & ((value) << GCLK_GENCTRL_ID_Pos)))

  // #define GCLK_GENCTRL_DIVSEL_Pos     20           // (GCLK_GENCTRL) Divide Selection
  // #define GCLK_GENCTRL_DIVSEL         (0x1u << GCLK_GENCTRL_DIVSEL_Pos)

  // A Generator is enabled by setting the Generic Clock Generator Enable bit in
  // the Generic Clock Generator Control register (GENCTRL.GENEN=1). Each
  // Generator can individually select a clock source by setting the Source
  // Select bit group in GENCTRL (GENCTRL.SRC).

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

  // 3.  The generic clock must be configured by performing a single 16-bit
  // write to the Generic Clock Control register (CLKCTRL): –  The Generic Clock
  // that will be configured via the ID bit group (CLKCTRL.ID) –  The Generic
  // Clock Generator used as the source of the generic clock by writing the GEN
  // bit group (CLKCTRL.GEN)

  // #define GCLK_CLKCTRL_CLKEN_Pos      14           // (GCLK_CLKCTRL) Clock Enable
  // #define GCLK_CLKCTRL_CLKEN          (0x1u << GCLK_CLKCTRL_CLKEN_Pos)

  // #define GCLK_CLKCTRL_GEN_Pos        8            // (GCLK_CLKCTRL) Generic Clock Generator
  // #define GCLK_CLKCTRL_GEN_Msk        (0xFu << GCLK_CLKCTRL_GEN_Pos)
  // #define GCLK_CLKCTRL_GEN(value)     ((GCLK_CLKCTRL_GEN_Msk & ((value) << GCLK_CLKCTRL_GEN_Pos)))
  // #define GCLK_CLKCTRL_GEN_GCLK3_Val      0x3u   // (GCLK_CLKCTRL) Generic clock generator 3

  // #define GCLK_CLKCTRL_ID_Pos         0            // (GCLK_CLKCTRL) Generic Clock Selection ID
  // #define GCLK_CLKCTRL_ID_Msk         (0x3Fu << GCLK_CLKCTRL_ID_Pos)
  // #define GCLK_CLKCTRL_ID(value)      ((GCLK_CLKCTRL_ID_Msk & ((value) << GCLK_CLKCTRL_ID_Pos)))

  // Before a generic clock is enabled, one of the Generators must be selected
  // as the source for the generic clock by writing to CLKCTRL.GEN. The clock
  // source selection is individually set for each generic clock. When a
  // Generator has been selected, the generic clock is enabled by setting the
  // Clock Enable bit in CLKCTRL (CLKCTRL.CLKEN=1). The CLKCTRL.CLKEN bit must
  // be synchronized to the generic clock domain. CLKCTRL.CLKEN will continue to
  // read as its previous state until the synchronization is complete.

  // When changing a generic clock source by writing to CLKCTRL.GEN, the generic
  // clock must be disabled before being re-enabled with the new clock source
  // setting. This prevents glitches during the transition:
  // 1.  Write CLKCTRL.CLKEN=0
  // 2.  Assert that CLKCTRL.CLKEN reads '0'
  // 3.  Change the source of the generic clock by writing CLKCTRL.GEN
  // 4.  Re-enable the generic clock by writing CLKCTRL.CLKEN=1

  // Enable clock, set gen clock number, and ID to where the clock goes
  GCLK->CLKCTRL.reg |= GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(3) | GCLK_CLKCTRL_ID(30);
  // GCLK->CLKCTRL.bit.CLKEN = 1;
  // GCLK->CLKCTRL.bit.GEN = 0x3;  // Generic clock generator 3
  // GCLK->CLKCTRL.bit.ID = 0x1E;  // "GCLK_ADC" (ADC)
  while (GCLK->STATUS.bit.SYNCBUSY);
}



void setupADC() {
  // ADC – Analog-to-Digital Converter

  // The Analog-to-Digital Converter (ADC) converts analog signals to digital
  // values. The ADC has 12-bit resolution, and is capable of converting up to
  // 350ksps. The input selection is flexible, and both differential and single-
  // ended measurements can be performed. An optional gain stage is available to
  // increase the dynamic range. In addition, several internal signal inputs are
  // available. The ADC can provide both signed and unsigned results.

  // ADC measurements can be started by either application software or an
  // incoming event from another peripheral in the device. ADC measurements can
  // be started with predictable timing, and without software intervention.

  // Both internal and external reference voltages can be used.

  // An integrated temperature sensor is available for use with the ADC. The
  // bandgap voltage as well as the scaled I/O and core voltages can also be
  // measured by the ADC.

  // The ADC has a compare function for accurate monitoring of user-defined
  // thresholds, with minimum software intervention required.

  // The ADC may be configured for 8-, 10- or 12-bit results, reducing the
  // conversion time. ADC conversion results are provided left- or right-
  // adjusted, which eases calculation when the result is represented as a
  // signed value. It is possible to use DMA to move ADC results directly to
  // memory or peripherals when conversions are done.

  // I/O lines: I/O Lines Using ADC I/O lines requires the I/O pins to be
  // configured using the port configuration (PORT).
  // Clocks: The ADC requires a generic clock (GCLK_ADC). This clock must be
  // configured and enabled in the Generic Clock Controller (GCLK) before using
  // the ADC.
  // DMA: The DMA request line is connected to the Direct Memory Access
  // Controller (DMAC). Using the ADC DMA requests requires the DMA Controller
  // to be configured first.
  // Interrupts: The interrupt request line is connected to the interrupt
  // controller. Using the ADC interrupt requires the interrupt controller to be
  // configured first.

  // Initialization

  // Before enabling the ADC, the asynchronous clock source must be selected and
  // enabled, and the ADC reference must be configured. The first conversion
  // after the reference is changed must not be used. All other configuration
  // registers must be stable during the conversion. The source for GCLK_ADC is
  // selected and enabled in the System Controller (SYSCTRL). When GCLK_ADC is
  // enabled, the ADC can be enabled by writing a one to the Enable bit in the
  // Control Register A (CTRLA.ENABLE).

  // Enabling, Disabling and Reset

  // The ADC is enabled by writing a '1' to the Enable bit in the Control A
  // register (CTRLA.ENABLE). The ADC is disabled by writing CTRLA.ENABLE=0. The
  // ADC is reset by writing a '1' to the Software Reset bit in the Control A
  // register (CTRLA.SWRST). All registers in the ADC, except DBGCTRL, will be
  // reset to their initial state, and the ADC will be disabled. The ADC must be
  // disabled before it is reset.

  // Operation

  // In the most basic configuration, the ADC samples values from the configured
  // internal or external sources (INPUTCTRL register). The rate of the
  // conversion depends on the combination of the GCLK_ADCx frequency and the
  // clock prescaler.

  // To convert analog values to digital values, the ADC needs to be initialized
  // first. Data conversion can be started either manually by setting the Start
  // bit in the Software Trigger register (SWTRIG.START=1), or automatically by
  // configuring an automatic trigger to initiate the conversions. A free-
  // running mode can be used to continuously convert an input channel. When
  // using free-running mode the first conversion must be started, while
  // subsequent conversions will start automatically at the end of previous
  // conversions.

  // The automatic trigger can be configured to trigger on many different
  // conditions. The result of the conversion is stored in the Result register
  // (RESULT) overwriting the result from the previous conversion.

  // To avoid data loss if more than one channel is enabled, the conversion
  // result must be read as soon as it is available (INTFLAG.RESRDY). Failing to
  // do so will result in an overrun error condition, indicated by the OVERRUN
  // bit in the Interrupt Flag Status and Clear register (INTFLAG.OVERRUN). When
  // the RESRDY interrupt flag is set, the new result has been synchronized to
  // the RESULT register. To enable one of the available interrupts sources, the
  // corresponding bit in the Interrupt Enable Set register (INTENSET) must be
  // written to '1'.

  // Prescaler

  // The ADC is clocked by GCLK_ADC. There is also a prescaler in the ADC to
  // enable conversion at lower clock rates.

  // ADC Resolution

  // The ADC supports 8-bit, 10-bit or 12-bit resolution. Resolution can be
  // changed by writing the Resolution bit group in the Control B register
  // (CTRLB.RESSEL). By default, the ADC resolution is set to 12 bits.

  // Differential and Single-Ended Conversions

  // The ADC has two conversion options: differential and single-ended:
  // * If the positive input may go below the negative input, the  differential
  //   mode should be used in order to get correct results.
  // * If the positive input is always positive, the  single-ended  conversion
  //   should be used in order to have full 12-bit resolution in the conversion.

  // Accumulation

  // The result from multiple consecutive conversions can be accumulated. The
  // number of samples to be accumulated is specified by the Number of Samples
  // to be Collected field in the Average Control register (AVGCTRL.SAMPLENUM).
  // When accumulating more than 16 samples, the result will be too large to
  // match the 16-bit RESULT register size. To avoid overflow, the result is
  // right shifted automatically to fit within the available register size.
  // Note: To perform the accumulation of two or more samples, the Conversion
  // Result Resolution field in the Control B register (CTRLB.RESSEL) must be
  // set.

  // Averaging

  // Averaging is a feature that increases the sample accuracy, at the cost of a
  // reduced sampling rate. This feature is suitable when operating in noisy
  // conditions. Averaging is done by accumulating m samples and dividing the
  // result by m. The averaged result is available in the RESULT register. The
  // number of samples to be accumulated is specified by writing to
  // AVGCTRL.SAMPLENUM. The division is obtained by a combination of the
  // automatic right shift described above, and an additional right shift that
  // must be specified by writing to the Adjusting Result/Division Coefficient
  // field in AVGCTRL (AVGCTRL.ADJRES). Note: To perform the averaging of two or
  // more samples, the Conversion Result Resolution field in the Control B
  // register (CTRLB.RESSEL) must be set to '1'.

  // Oversampling and Decimation

  // By using oversampling and decimation, the ADC resolution can be increased
  // from 12 bits up to 16 bits, for the cost of reduced effective sampling
  // rate.

  // To increase the resolution by n bits, 4 n  samples must be accumulated. The
  // result must then be right- shifted by n bits. This right-shift is a
  // combination of the automatic right-shift and the value written to
  // AVGCTRL.ADJRES. To obtain the correct resolution, the ADJRES must be
  // configured. This method will result in n bit extra LSB resolution.

  // Window Monitor

  // The window monitor feature allows the conversion result in the RESULT
  // register to be compared to predefined threshold values. The window mode is
  // selected by setting the Window Monitor Mode bits in the Window Monitor
  // Control register (WINCTRL.WINMODE[2:0]). Threshold values must be written
  // in the Window Monitor Lower Threshold register (WINLT) and Window Monitor
  // Upper Threshold register (WINUT).

  // If differential input is selected, the WINLT and WINUT are evaluated as
  // signed values. Otherwise they are evaluated as unsigned values. The
  // significant WINLT and WINUT bits are given by the precision selected in the
  // Conversion Result Resolution bit group in the Control B register
  // (CTRLB.RESSEL). This means that e.g. in 8-bit mode, only the eight lower
  // bits will be considered. In addition, in differential mode, the eighth bit
  // will be considered as the sign bit, even if the ninth bit is zero.

  // The INTFLAG.WINMON interrupt flag will be set if the conversion result
  // matches the window monitor condition.

  // Offset and Gain Correction

  // Inherent gain and offset errors affect the absolute accuracy of the ADC.
  // The offset error is defined as the deviation of the actual ADC transfer
  // function from an ideal straight line at zero input voltage. The offset
  // error cancellation is handled by the Offset Correction register
  // (OFFSETCORR). The offset correction value is subtracted from the converted
  // data before writing the Result register (RESULT).

  // The gain error is defined as the deviation of the last output step’s
  // midpoint from the ideal straight line, after compensating for offset error.
  // The gain error cancellation is handled by the Gain Correction register
  // (GAINCORR).

  // To correct these two errors, the Digital Correction Logic Enabled bit in
  // the Control B register (CTRLB.CORREN) must be set to ''.

  // Offset and gain error compensation results are both calculated according
  // to: Result=Conversion value+−OFFSETCORR⋅GAINCORR

  // The correction will introduce a latency of 13 CLK_ADC clock cycles. In free
  // running mode this latency is introduced on the first conversion only, since
  // its duration is always less than the propagation delay. In single
  // conversion mode this latency is introduced for each conversion.

  // DMA Operation

  // The ADC generates the following DMA request: Result Conversion Ready
  // (RESRDY): the request is set when a conversion result is available and
  // cleared when the RESULT register is read. When the averaging operation is
  // enabled, the DMA request is set when the averaging is completed and result
  // is available.

  // Interrupts

  // The ADC has the following interrupt sources:
  // * Result Conversion Ready: RESRDY
  // * Window Monitor: WINMON
  // * Overrun: OVERRUN

  // Each interrupt source has an interrupt flag associated with it. The
  // interrupt flag in the Interrupt Flag Status and Clear (INTFLAG) register is
  // set when the interrupt condition occurs. Each interrupt can be individually
  // enabled by writing a one to the corresponding bit in the Interrupt Enable
  // Set (INTENSET) register, and disabled by writing a one to the corresponding
  // bit in the Interrupt Enable Clear (INTENCLR) register. An interrupt request
  // is generated when the interrupt flag is set and the corresponding interrupt
  // is enabled. The interrupt request remains active until the interrupt flag
  // is cleared, the interrupt is disabled, or the ADC is reset. An interrupt
  // flag is cleared by writing a one to the corresponding bit in the INTFLAG
  // register. Each peripheral can have one interrupt request line per interrupt
  // source or one common interrupt request line for all the interrupt sources.
  // This is device dependent.

  // Events

  // The ADC can generate the following output events:
  // * Result Ready (RESRDY): Generated when the conversion is complete and the 
  //   result is available.
  // * Window Monitor (WINMON): Generated when the window monitor condition match.

  // Setting an Event Output bit in the Event Control Register (EVCTRL.xxEO=1)
  // enables the corresponding output event. Clearing this bit disables the
  // corresponding output event.

  // The peripheral can take the following actions on an input event:
  // * Start conversion (START): Start a conversion.
  // * Conversion flush (FLUSH): Flush the conversion.

  // Setting an Event Input bit in the Event Control register (EVCTRL.xxEI=1)
  // enables the corresponding action on input event. Clearing this bit disables
  // the corresponding action on input event. Note: If several events are
  // connected to the ADC, the enabled action will be taken on any of the
  // incoming events. The events must be correctly routed in the Event System.

  // Synchronization

  // Due to asynchronicity between the main clock domain and the peripheral
  // clock domains, some registers need to be synchronized when written or read.
  // When executing an operation that requires synchronization, the
  // Synchronization Busy bit in the Status register (STATUS.SYNCBUSY) will be
  // set immediately, and cleared when synchronization is complete. The
  // Synchronization Ready interrupt can be used to signal when synchronization
  // is complete.

  // If an operation that requires synchronization is executed while
  // STATUS.SYNCBUSY=1, the bus will be stalled. All operations will complete
  // successfully, but the CPU will be stalled and interrupts will be pending as
  // long as the bus is stalled.

  // The following bits are synchronized when written:
  // * Software Reset bit in the Control A register (CTRLA.SWRST)
  // * Enable bit in the Control A register (CTRLA.ENABLE)

  // The following registers are synchronized when written:
  // * Control B (CTRLB)
  // * Software Trigger (SWTRIG)
  // * Window Monitor Control (WINCTRL)
  // * Input Control (INPUTCTRL)
  // * Window Upper/Lower Threshold (WINUT/WINLT)

  // The following registers are synchronized when read:
  // * Software Trigger (SWTRIG)
  // * Input Control (INPUTCTRL)

  // Reference Control
  // Select reference, internal VCC/2
  ADC->REFCTRL.reg |= ADC_REFCTRL_REFSEL_INTVCC1; // VDDANA/2, combine with gain DIV2 for full VCC range
  // Reference Selection: These bits select the reference for the ADC.
  // ADC->REFCTRL.bit.REFSEL = 0x2;  // INTVCC1: 1/2 VDDANA (only for VDDANA > 2.0V)

  // #define ADC_AVGCTRL_ADJRES_Pos      4            /**< \brief (ADC_AVGCTRL) Adjusting Result / Division Coefficient */
  // #define ADC_AVGCTRL_ADJRES_Msk      (0x7u << ADC_AVGCTRL_ADJRES_Pos)
  // #define ADC_AVGCTRL_ADJRES(value)   ((ADC_AVGCTRL_ADJRES_Msk & ((value) << ADC_AVGCTRL_ADJRES_Pos)))


  // Average Control
  // Average control 1 sample, no right-shift
  ADC->AVGCTRL.reg |= ADC_AVGCTRL_ADJRES(0) | ADC_AVGCTRL_SAMPLENUM_1;
  // Adjusting Result / Division Coefficient: 
  //   These bits define the division coefficient in 2n steps.
  //   ADC->REFCTRL.bit.ADJRES = 0;
  // Number of Samples to be Collected:
  //   These bits define how many samples should be added together.The result
  //   will be available in the Result register (RESULT). Note: if the result
  //   width increases, CTRLB.RESSEL must be changed.
  //   ADC->REFCTRL.bit.SAMPLENUM = 0x0;  // 1 sample

  // Sampling Time Control
  //   These bits control the ADC sampling time in number of half CLK_ADC 
  //   cycles, depending of the prescaler value, thus controlling the ADC input
  //   impedance. Sampling time is set according to the equation:
  //     Sampling time= (SAMPLEN+1) * (CLK_ADC / 2)
  ADC->SAMPCTRL.bit.SAMPLEN = 0x2;

  // Input control: set gain to div by two so ADC has measurement range of VCC, no diff measurement so set neg to gnd, pos input set to pin 0 or A0

  // Gain Factor Selection:  These bits set the gain factor of the ADC gain stage.
  //   0x0: 1X (1x)
  //   0x1: 2X (2x)
  //   0x2: 4X (4x)
  //   0x3: 8X (8x)
  //   0x4: 16X (16x)
  //   0xF: DIV2 (1/2x)
  ADC->INPUTCTRL.bit.GAIN = 0xF;  // DIV2 (1/2x)

  // Positive Mux Setting Offset:
  //   The pin scan is enabled when INPUTSCAN != 0. Writing these bits to a
  //   value other than zero causes the first conversion triggered to be
  //   converted using a positive input equal to MUXPOS + INPUTOFFSET. Setting
  //   this register to zero causes the first conversion to use a positive input
  //   equal to MUXPOS. After a conversion, the INPUTOFFSET register will be
  //   incremented by one, causing the next conversion to be done with the
  //   positive input equal to MUXPOS + INPUTOFFSET. The sum of MUXPOS and
  //   INPUTOFFSET gives the input that is actually converted.
  ADC->INPUTCTRL.bit.INPUTOFFSET = 0;

  // Number of Input Channels Included in Scan:
  //   This register gives the number of input sources included in the pin scan.
  //   The number of input sources included is INPUTSCAN + 1. The input channels
  //   included are in the range from MUXPOS + INPUTOFFSET to MUXPOS +
  //   INPUTOFFSET + INPUTSCAN. The range of the scan mode must not exceed the
  //   number of input channels available on the device.
  ADC->INPUTCTRL.bit.INPUTSCAN = 0;

  // Negative Mux Input Selection:
  //   These bits define the Mux selection for the negative ADC input. selections.
  //   0x00: PIN0 (ADC AIN0 pin)
  //   0x01: PIN1 (ADC AIN1 pin)
  //   0x02: PIN2 (ADC AIN2 pin)
  //   0x03: PIN3 (ADC AIN3 pin)
  //   0x04: PIN4 (ADC AIN4 pin)
  //   0x05: PIN5 (ADC AIN5 pin)
  //   0x06: PIN6 (ADC AIN6 pin)
  //   0x07: PIN7 (ADC AIN7 pin)
  //   0x18: GND (Internal ground)
  //   0x19: IOGND (I/O ground)
  ADC->INPUTCTRL.bit.MUXNEG = 0x18;  // GND (Internal ground)

  // Positive Mux Input Selection:
  //   These bits define the Mux selection for the positive ADC input. The
  //   following table shows the possible input selections. If the internal
  //   bandgap voltage or temperature sensor input channel is selected, then the
  //   Sampling Time Length bit group in the SamplingControl register must be
  //   written.
  //   0x00: PIN0 (ADC AIN0 pin)
  //   0x01: PIN1 (ADC AIN1 pin)
  //   0x02: PIN2 (ADC AIN2 pin)
  //   0x03: PIN3 (ADC AIN3 pin)
  //   0x04: PIN4 (ADC AIN4 pin)
  //   0x05: PIN5 (ADC AIN5 pin)
  //   0x06: PIN6 (ADC AIN6 pin)
  //   0x07: PIN7 (ADC AIN7 pin)
  //   0x08: PIN8 (ADC AIN8 pin)
  //   0x09: PIN9 (ADC AIN9 pin)
  //   0x0A: PIN10 (ADC AIN10 pin)
  //   0x0B: PIN11 (ADC AIN11 pin)
  //   0x0C: PIN12 (ADC AIN12 pin)
  //   0x0D: PIN13 (ADC AIN13 pin)
  //   0x0E: PIN14 (ADC AIN14 pin)
  //   0x0F: PIN15 (ADC AIN15 pin)
  //   0x10: PIN16 (ADC AIN16 pin)
  //   0x11: PIN17 (ADC AIN17 pin)
  //   0x12: PIN18 (ADC AIN18 pin)
  //   0x13: PIN19 (ADC AIN19 pin)
  //   0x19: BANDGAP (Bandgap voltage)
  //   0x1A: SCALEDCOREVCC (1/4 scaled core supply)
  //   0x1B: SCALEDIOVCC (1/4 scaled I/O supply)
  //   0x1C: DAC (DAC output)
  ADC->INPUTCTRL.bit.MUXPOS = 0x00;  // PIN0 (ADC AIN0 pin)

  syncADC();

  // Control B

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
  ADC->CTRLB.bit.PRESCALER = 0x3; // DIV16: Peripheral clock divided by 16

  // Conversion Result Resolution:
  //   These bits define whether the ADC completes the conversion at 12-, 10- or
  //   8-bit result resolution.
  //   0x0: 12BIT (12-bit result)
  //   0x1: 16BIT (For averaging mode output
  //   0x2: 10BIT (10-bit result)
  //   0x3: 8BIT (8-bit result)
  ADC->CTRLB.bit.RESSEL = 0x0;  // 8BIT (8-bit result)

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
void setupInterrupt(byte priority) {
  // Interrupt Enable Set: Writing a zero to these bits has no effect.
  // ADC->INTENSET.bit.SYNCRDY = 1;  // Synchronization Ready Interrupt Enable
  // ADC->INTENSET.bit.WINMON = 1;  // Window Monitor Interrupt Enable
  // ADC->INTENSET.bit.OVERRUN = 1;  // Overrun Interrupt Enable
  ADC->INTENSET.bit.RESRDY = 1;  // Result Ready Interrupt Enable
  //ADC->INTENSET.reg |= ADC_INTENSET_RESRDY; // enable Result Ready Interrupt
  syncADC();

  // Nested Vector Interrupt Controller: Interrupt Line Mapping

  // Each of the 28 interrupt lines is connected to one peripheral instance, as
  // shown in the table below. Each peripheral can have one or more interrupt
  // flags, located in the peripheral’s Interrupt Flag Status and Clear
  // (INTFLAG) register. The interrupt flag is set when the interrupt condition
  // occurs. Each interrupt in the peripheral can be individually enabled by
  // writing a one to the corresponding bit in the peripheral’s Interrupt Enable
  // Set (INTENSET) register, and disabled by writing a one to the corresponding
  // bit in the peripheral’s Interrupt Enable Clear (INTENCLR) register. An
  // interrupt request is generated from the peripheral when the interrupt flag
  // is set and the corresponding interrupt is enabled. The interrupt requests
  // for one peripheral are ORed together on system level, generating one
  // interrupt request for each peripheral. An interrupt request will set the
  // corresponding interrupt pending bit in the NVIC interrupt pending registers
  // (SETPEND/CLRPEND bits in ISPR/ICPR). For the NVIC to activate the
  // interrupt, it must be enabled in the NVIC interrupt enable register
  // (SETENA/CLRENA bits in ISER/ICER). The NVIC interrupt priority registers
  // IPR0-IPR7 provide a priority field for each interrupt.

  // Peripheral Source (NVIC Line):
  // * DMAC - Direct Memory Access Controller (6)
  // * ADC – Analog-to-Digital Converter (23)

  NVIC_EnableIRQ(ADC_IRQn); // enable ADC interrupts
  NVIC_SetPriority(ADC_IRQn, priority); //set priority of the interrupt
}



void enableADC() {
  //ADC->CTRLA.reg = 2;  // 2 is binary 010 which is register bit to enable ADC
  ADC->CTRLA.bit.ENABLE = 1;
  
  // Following required for free running mode?
  //ADC->SWTRIG.reg |= ADC_SWTRIG_START;
  ADC->SWTRIG.bit.START = 1;
  // The ADC will start a conversion. The bit is cleared by hardware when the
  // conversion has started. Setting this bit when it is already set has no
  // effect.

}


void disableADC() {
  // ADC->CTRLA.reg = 0;  // 0 disables ADC
  ADC->CTRLA.bit.ENABLE = 0;
}


void stopSampling() {
  NVIC_DisableIRQ(ADC_IRQn);  // turn off interrupt
  disableADC();
}


//this function sets up the ADC and interrupts and starts
extern void startSampling() {
  setupReadPin();
  setupGenericClock();
  setupADC();
  setupInterrupt(0); // with highest priority
  enableADC();
}


void printAllADCBits() {
  Serial.print("ADC->CTRLA.bit.SWRST = "); Serial.println(ADC->CTRLA.bit.SWRST, BIN);
  Serial.print("ADC->CTRLA.bit.ENABLE = "); Serial.println(ADC->CTRLA.bit.ENABLE, BIN);
  Serial.print("ADC->CTRLA.bit.RUNSTDBY = "); Serial.println(ADC->CTRLA.bit.RUNSTDBY, BIN);
  Serial.print("ADC->REFCTRL.bit.REFSEL = "); Serial.println(ADC->REFCTRL.bit.REFSEL, BIN);
  Serial.print("ADC->REFCTRL.bit.REFCOMP = "); Serial.println(ADC->REFCTRL.bit.REFCOMP, BIN);
  Serial.print("ADC->AVGCTRL.bit.SAMPLENUM = "); Serial.println(ADC->AVGCTRL.bit.SAMPLENUM, BIN);
  Serial.print("ADC->AVGCTRL.bit.ADJRES = "); Serial.println(ADC->AVGCTRL.bit.ADJRES, BIN);
  Serial.print("ADC->SAMPCTRL.bit.SAMPLEN = "); Serial.println(ADC->SAMPCTRL.bit.SAMPLEN, BIN);
  Serial.print("ADC->CTRLB.bit.DIFFMODE = "); Serial.println(ADC->CTRLB.bit.DIFFMODE, BIN);
  Serial.print("ADC->CTRLB.bit.LEFTADJ = "); Serial.println(ADC->CTRLB.bit.LEFTADJ, BIN);
  Serial.print("ADC->CTRLB.bit.FREERUN = "); Serial.println(ADC->CTRLB.bit.FREERUN, BIN);
  Serial.print("ADC->CTRLB.bit.CORREN = "); Serial.println(ADC->CTRLB.bit.CORREN, BIN);
  Serial.print("ADC->CTRLB.bit.RESSEL = "); Serial.println(ADC->CTRLB.bit.RESSEL, BIN);
  Serial.print("ADC->CTRLB.bit.PRESCALER = "); Serial.println(ADC->CTRLB.bit.PRESCALER, BIN);
  Serial.print("ADC->WINCTRL.bit.WINMODE = "); Serial.println(ADC->WINCTRL.bit.WINMODE, BIN);
  Serial.print("ADC->SWTRIG.bit.FLUSH = "); Serial.println(ADC->SWTRIG.bit.FLUSH, BIN);
  Serial.print("ADC->SWTRIG.bit.START = "); Serial.println(ADC->SWTRIG.bit.START, BIN);
  Serial.print("ADC->INPUTCTRL.bit.MUXPOS = "); Serial.println(ADC->INPUTCTRL.bit.MUXPOS, BIN);
  Serial.print("ADC->INPUTCTRL.bit.MUXNEG = "); Serial.println(ADC->INPUTCTRL.bit.MUXNEG, BIN);
  Serial.print("ADC->INPUTCTRL.bit.INPUTSCAN = "); Serial.println(ADC->INPUTCTRL.bit.INPUTSCAN, BIN);
  Serial.print("ADC->INPUTCTRL.bit.INPUTOFFSET = "); Serial.println(ADC->INPUTCTRL.bit.INPUTOFFSET, BIN);
  Serial.print("ADC->INPUTCTRL.bit.GAIN = "); Serial.println(ADC->INPUTCTRL.bit.GAIN, BIN);
  Serial.print("ADC->EVCTRL.bit.STARTEI = "); Serial.println(ADC->EVCTRL.bit.STARTEI, BIN);
  Serial.print("ADC->EVCTRL.bit.SYNCEI = "); Serial.println(ADC->EVCTRL.bit.SYNCEI, BIN);
  Serial.print("ADC->EVCTRL.bit.RESRDYEO = "); Serial.println(ADC->EVCTRL.bit.RESRDYEO, BIN);
  Serial.print("ADC->EVCTRL.bit.WINMONEO = "); Serial.println(ADC->EVCTRL.bit.WINMONEO, BIN);
  Serial.print("ADC->INTENCLR.bit.RESRDY = "); Serial.println(ADC->INTENCLR.bit.RESRDY, BIN);
  Serial.print("ADC->INTENCLR.bit.OVERRUN = "); Serial.println(ADC->INTENCLR.bit.OVERRUN, BIN);
  Serial.print("ADC->INTENCLR.bit.WINMON = "); Serial.println(ADC->INTENCLR.bit.WINMON, BIN);
  Serial.print("ADC->INTENCLR.bit.SYNCRDY = "); Serial.println(ADC->INTENCLR.bit.SYNCRDY, BIN);
  Serial.print("ADC->INTENSET.bit.RESRDY = "); Serial.println(ADC->INTENSET.bit.RESRDY, BIN);
  Serial.print("ADC->INTENSET.bit.OVERRUN = "); Serial.println(ADC->INTENSET.bit.OVERRUN, BIN);
  Serial.print("ADC->INTENSET.bit.WINMON = "); Serial.println(ADC->INTENSET.bit.WINMON, BIN);
  Serial.print("ADC->INTENSET.bit.SYNCRDY = "); Serial.println(ADC->INTENSET.bit.SYNCRDY, BIN);
  Serial.print("ADC->INTFLAG.bit.RESRDY = "); Serial.println(ADC->INTFLAG.bit.RESRDY, BIN);
  Serial.print("ADC->INTFLAG.bit.OVERRUN = "); Serial.println(ADC->INTFLAG.bit.OVERRUN, BIN);
  Serial.print("ADC->INTFLAG.bit.WINMON = "); Serial.println(ADC->INTFLAG.bit.WINMON, BIN);
  Serial.print("ADC->INTFLAG.bit.SYNCRDY = "); Serial.println(ADC->INTFLAG.bit.SYNCRDY, BIN);
  Serial.print("ADC->STATUS.bit.SYNCBUSY = "); Serial.println(ADC->STATUS.bit.SYNCBUSY, BIN);
  Serial.print("ADC->RESULT.bit.RESULT = "); Serial.println(ADC->RESULT.bit.RESULT, BIN);
  Serial.print("ADC->WINLT.bit.WINLT = "); Serial.println(ADC->WINLT.bit.WINLT, BIN);
  Serial.print("ADC->WINUT.bit.WINUT = "); Serial.println(ADC->WINUT.bit.WINUT, BIN);
  Serial.print("ADC->GAINCORR.bit.GAINCORR = "); Serial.println(ADC->GAINCORR.bit.GAINCORR, BIN);
  Serial.print("ADC->OFFSETCORR.bit.OFFSETCORR = "); Serial.println(ADC->OFFSETCORR.bit.OFFSETCORR, BIN);
  Serial.print("ADC->CALIB.bit.LINEARITY_CAL = "); Serial.println(ADC->CALIB.bit.LINEARITY_CAL, BIN);
  Serial.print("ADC->CALIB.bit.BIAS_CAL = "); Serial.println(ADC->CALIB.bit.BIAS_CAL, BIN);
  Serial.print("ADC->DBGCTRL.bit.DBGRUN = "); Serial.println(ADC->DBGCTRL.bit.DBGRUN, BIN);  Serial.println("HEYYYYYY");
}
