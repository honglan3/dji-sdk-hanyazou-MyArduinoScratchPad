#include <Wire.h>
#include <mcp23017.h>
#include <HPDL1414.h>

#define I2C_ADDR 0x20


HPDL1414 disp(4,I2C_ADDR);

void setup(){
  Serial.begin(38400);
  Serial.println("HPDL-1414 test...");

  Wire.begin();
  Wire.beginTransmission (I2C_ADDR);
  if (Wire.endTransmission () != 0) {
    Serial.print("ERROR: I2C addr ");
    Serial.print(I2C_ADDR);
    Serial.println(" not responding");
  }

  disp.begin(2);
  /*
  disp.setCursor(2);
  disp.writeChar('A',true);
  delay(1000);
  disp.fillDigitsBetween(2,4,0x58);
  */
  //disp.printString("HARUCHAN",20);
  disp.scroll("HPDL1414 DISPLAY TEST, srolling....",100);
  //disp.print("HARUCHAN");
  disp.printString("HARUCHAN",20);
  delay(1000);
  disp.blankFromTo(4,7);
}
uint8_t flip = 1;
void loop(){
  disp.trigBlank(flip);
  if (flip == 1){
    flip = 0;
  } else {
    flip = 1;
  }
  delay(200);
}
