#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
}


int test(int i){
  if(i==1) { return 0; }
  if(i==2) { return 0; }
  if(i==3) { return 0; }
  if(i==4) { return 0; }
  if(i==5) { return 1; }
  if(i==6) { return 1; }
  if(i==7) { return 1; }
  if(i==8) { return 2; }
  if(i==9) { return 2; }
  if(i==10) { return 3; }
  if(i==11) { return 4; }
  if(i==12) { return 4; }
  if(i==13) { return 5; }
  if(i==14) { return 6; }
  if(i==15) { return 7; }
  if(i==16) { return 8; }
  if(i==17) { return 9; }
  if(i==18) { return 11; }
  if(i==19) { return 12; }
  if(i==20) { return 13; }
  if(i==21) { return 15; }
  if(i==22) { return 16; }
  if(i==23) { return 18; }
  if(i==24) { return 20; }
  if(i==25) { return 21; }
  if(i==26) { return 23; }
  if(i==27) { return 25; }
  if(i==28) { return 27; }
  if(i==29) { return 29; }
  if(i==30) { return 31; }
  if(i==31) { return 33; }
  if(i==32) { return 35; }
  if(i==33) { return 37; }
  if(i==34) { return 39; }
  if(i==35) { return 42; }
  if(i==36) { return 44; }
  if(i==37) { return 46; }
  if(i==38) { return 49; }
  if(i==39) { return 51; }
  if(i==40) { return 54; }
  if(i==41) { return 56; }
  if(i==42) { return 59; }
  if(i==43) { return 62; }
  if(i==44) { return 64; }
  if(i==45) { return 67; }
  if(i==46) { return 70; }
  if(i==47) { return 73; }
  if(i==48) { return 76; }
  if(i==49) { return 79; }
  if(i==50) { return 81; }
  if(i==51) { return 84; }
  if(i==52) { return 87; }
  if(i==53) { return 90; }
  if(i==54) { return 93; }
  if(i==55) { return 96; }
  if(i==56) { return 99; }
  if(i==57) { return 103; }
  if(i==58) { return 106; }
  if(i==59) { return 109; }
  if(i==60) { return 112; }
  if(i==61) { return 115; }
  if(i==62) { return 118; }
  if(i==63) { return 121; }
  if(i==64) { return 124; }
  if(i==65) { return 128; }
  if(i==66) { return 131; }
  if(i==67) { return 131; }
  if(i==68) { return 134; }
  if(i==69) { return 137; }
  if(i==70) { return 140; }
  if(i==71) { return 143; }
  if(i==72) { return 146; }
  if(i==73) { return 149; }
  if(i==74) { return 152; }
  if(i==75) { return 156; }
  if(i==76) { return 159; }
  if(i==77) { return 162; }
  if(i==78) { return 165; }
  if(i==79) { return 168; }
  if(i==80) { return 171; }
  if(i==81) { return 174; }
  if(i==82) { return 176; }
  if(i==83) { return 179; }
  if(i==84) { return 182; }
  if(i==85) { return 185; }
  if(i==86) { return 188; }
  if(i==87) { return 191; }
  if(i==88) { return 193; }
  if(i==89) { return 196; }
  if(i==90) { return 199; }
  if(i==91) { return 201; }
  if(i==92) { return 204; }
  if(i==93) { return 206; }
  if(i==94) { return 209; }
  if(i==95) { return 211; }
  if(i==96) { return 213; }
  if(i==97) { return 216; }
  if(i==98) { return 218; }
  if(i==99) { return 220; }
  if(i==100) { return 222; }
  if(i==101) { return 224; }
  if(i==102) { return 226; }
  if(i==103) { return 228; }
  if(i==104) { return 230; }
  if(i==105) { return 232; }
  if(i==106) { return 234; }
  if(i==107) { return 235; }
  if(i==108) { return 237; }
  if(i==109) { return 239; }
  if(i==110) { return 240; }
  if(i==111) { return 242; }
  if(i==112) { return 243; }
  if(i==113) { return 244; }
  if(i==114) { return 246; }
  if(i==115) { return 247; }
  if(i==116) { return 248; }
  if(i==117) { return 249; }
  if(i==118) { return 250; }
  if(i==119) { return 251; }
  if(i==120) { return 251; }
  if(i==121) { return 252; }
  if(i==122) { return 253; }
  if(i==123) { return 253; }
  if(i==124) { return 254; }
  if(i==125) { return 254; }
  if(i==126) { return 254; }
  if(i==127) { return 255; }
  if(i==128) { return 255; }
  if(i==129) { return 255; }
  if(i==130) { return 255; }
  if(i==131) { return 255; }
  if(i==132) { return 255; }
  if(i==133) { return 255; }
  if(i==134) { return 254; }
  if(i==135) { return 254; }
  if(i==136) { return 253; }
  if(i==137) { return 253; }
  if(i==138) { return 252; }
  if(i==139) { return 252; }
  if(i==140) { return 251; }
  if(i==141) { return 250; }
  if(i==142) { return 249; }
  if(i==143) { return 248; }
  if(i==144) { return 247; }
  if(i==145) { return 246; }
  if(i==146) { return 245; }
  if(i==147) { return 244; }
  if(i==148) { return 242; }
  if(i==149) { return 241; }
  if(i==150) { return 239; }
  if(i==151) { return 238; }
  if(i==152) { return 236; }
  if(i==153) { return 235; }
  if(i==154) { return 233; }
  if(i==155) { return 231; }
  if(i==156) { return 229; }
  if(i==157) { return 227; }
  if(i==158) { return 225; }
  if(i==159) { return 223; }
  if(i==160) { return 221; }
  if(i==161) { return 219; }
  if(i==162) { return 217; }
  if(i==163) { return 215; }
  if(i==164) { return 212; }
  if(i==165) { return 210; }
  if(i==166) { return 207; }
  if(i==167) { return 205; }
  if(i==168) { return 202; }
  if(i==169) { return 200; }
  if(i==170) { return 197; }
  if(i==171) { return 195; }
  if(i==172) { return 192; }
  if(i==173) { return 189; }
  if(i==174) { return 186; }
  if(i==175) { return 184; }
  if(i==176) { return 181; }
  if(i==177) { return 178; }
  if(i==178) { return 175; }
  if(i==179) { return 172; }
  if(i==180) { return 169; }
  if(i==181) { return 166; }
  if(i==182) { return 163; }
  if(i==183) { return 160; }
  if(i==184) { return 157; }
  if(i==185) { return 154; }
  if(i==186) { return 151; }
  if(i==187) { return 148; }
  if(i==188) { return 145; }
  if(i==189) { return 142; }
  if(i==190) { return 138; }
  if(i==191) { return 135; }
  if(i==192) { return 132; }
  if(i==193) { return 129; }
  if(i==194) { return 126; }
  if(i==195) { return 123; }
  if(i==196) { return 120; }
  if(i==197) { return 117; }
  if(i==198) { return 113; }
  if(i==199) { return 110; }
  if(i==200) { return 107; }
  if(i==201) { return 104; }
  if(i==202) { return 101; }
  if(i==203) { return 98; }
  if(i==204) { return 95; }
  if(i==205) { return 92; }
  if(i==206) { return 89; }
  if(i==207) { return 86; }
  if(i==208) { return 83; }
  if(i==209) { return 80; }
  if(i==210) { return 77; }
  if(i==211) { return 74; }
  if(i==212) { return 71; }
  if(i==213) { return 69; }
  if(i==214) { return 66; }
  if(i==215) { return 63; }
  if(i==216) { return 60; }
  if(i==217) { return 58; }
  if(i==218) { return 55; }
  if(i==219) { return 53; }
  if(i==220) { return 50; }
  if(i==221) { return 48; }
  if(i==222) { return 45; }
  if(i==223) { return 43; }
  if(i==224) { return 40; }
  if(i==225) { return 38; }
  if(i==226) { return 36; }
  if(i==227) { return 34; }
  if(i==228) { return 32; }
  if(i==229) { return 30; }
  if(i==230) { return 28; }
  if(i==231) { return 26; }
  if(i==232) { return 24; }
  if(i==233) { return 22; }
  if(i==234) { return 20; }
  if(i==235) { return 19; }
  if(i==236) { return 17; }
  if(i==237) { return 16; }
  if(i==238) { return 14; }
  if(i==239) { return 13; }
  if(i==240) { return 11; }
  if(i==241) { return 10; }
  if(i==242) { return 9; }
  if(i==243) { return 8; }
  if(i==244) { return 7; }
  if(i==245) { return 6; }
  if(i==246) { return 5; }
  if(i==247) { return 4; }
  if(i==248) { return 3; }
  if(i==249) { return 3; }
  if(i==250) { return 2; }
  if(i==251) { return 2; }
  if(i==252) { return 1; }
  if(i==253) { return 1; }
  if(i==254) { return 0; }
  if(i==255) { return 0; }
  if(i==256) { return 0; }
}

void pulse(uint32_t c, int w) {
  for(uint16_t j=32; j<255; j++) {
    strip.setBrightness(j-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(w);
  }
  delay(w*10+250);
  for(uint16_t j=255; j>32; j--) {
    strip.setBrightness(j-1);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(w);
  }
  delay(w*10+250);
}


void breathe(uint32_t c, uint8_t w = 1) {

  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
  
  for(uint16_t i=1; i<256; i=i+4 ) {
    strip.setBrightness(i);
    strip.show();
    delay( test(i) * w );
  }
  for(uint16_t i=255; i>0; i=i-4 ) {
    strip.setBrightness(i);
    strip.show();
    delay( int(test(i) / 10) * w );
  }


//  pi sin x i
//  2  -1  0 0
//  3  0   1 127
//  4  1   2 255
//  5  0   1 127
//  6  -1  0 0
  
}

void loop() {
  //breathe(strip.Color(0, 255, 0), 1);
  pulse(strip.Color(0, 255, 0), 0);
}






void whiteFlash(uint8_t w) {

  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 255, 255));
  }
  strip.show();
  delay(w);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  delay(w);


}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void setPixel(int i, uint32_t c){
  strip.setPixelColor(i, c);
  strip.setPixelColor(strip.numPixels()-1-i , c);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=255; j>0; j--) {
    for(i=0; i<strip.numPixels()/2; i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
