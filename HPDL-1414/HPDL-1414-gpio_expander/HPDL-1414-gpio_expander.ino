#include <Wire.h>
#include <mcp23017.h>

#define I2C_ADDR 0x20
mcp23017 mygpio;

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

  mygpio.postSetup(I2C_ADDR);
  mygpio.begin();
  mygpio.gpioPinMode(OUTPUT);
}

void hpdl1414_put(int pos, uint8_t data)
{
  const uint16_t ctrl = 0xfc;
  int _wr = ~(0x4 << (pos / 4));
  pos %= 4;

  mygpio.gpioPort(data, ctrl | pos);
  delayMicroseconds(1);  // tWD   Write Delay Time  20ns 
  mygpio.gpioPort(data, (ctrl & _wr) | pos);
  delayMicroseconds(1);  // tW    Write Time  130ns
  mygpio.gpioPort(data, ctrl | pos);
  delayMicroseconds(1);  // tAH, tDH  Address/Data Hold Time  50ns
}

void loop(){
  for (int c = 0; c < 64; c++) {
    for (int i = 0; i < 8; i++) {
      hpdl1414_put(i, (c + 7 - i) % 64 + 32);
    }
    delay(50);
  }
}

