#include <Wire.h>
#include <mcp23017.h>

#define I2C_ADDR 0x20
#define N 4
mcp23017 mygpio[N];
int nunits = 0;

void setup(){
  while (!Serial);
  Serial.begin(9600);
  Serial.println("HPDL-1414 test...");

  Wire.begin();

  for (int i = 0; i < N; i++){
    Wire.beginTransmission (I2C_ADDR + i);
    if (Wire.endTransmission () == 0) {
      Serial.print("Unit ");
      Serial.print(i);
      Serial.print(" on I2C addr ");
      Serial.println(I2C_ADDR + i);
      mygpio[nunits].postSetup(I2C_ADDR + i);
      mygpio[nunits].begin();
      mygpio[nunits].gpioPinMode(OUTPUT);
      nunits++;
    }
  }
}

void hpdl1414_put(int pos, uint8_t data)
{
  const uint16_t ctrl = 0xfc;
  int unit = nunits-1 - pos/16;
  if (unit < 0) {
    return;
  }
  pos %= 16;
  int _wr = ~(0x4 << (pos / 4));
  pos %= 4;

  mygpio[unit].gpioPort(data, ctrl | pos);
  delayMicroseconds(1);  // tWD   Write Delay Time  20ns 
  mygpio[unit].gpioPort(data, (ctrl & _wr) | pos);
  delayMicroseconds(1);  // tW    Write Time  130ns
  mygpio[unit].gpioPort(data, ctrl | pos);
  delayMicroseconds(1);  // tAH, tDH  Address/Data Hold Time  50ns
}

void loop(){
  for (int c = 0; c < 64 + 16 * nunits; c++) {
    for (int i = 0; i < 16 * nunits; i++) {
      hpdl1414_put(i, (c + 7 - i) + 32);
    }
    delay(50);
  }
}

