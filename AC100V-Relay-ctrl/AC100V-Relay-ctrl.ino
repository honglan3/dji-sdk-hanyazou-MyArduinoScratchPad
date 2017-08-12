/*  
 *   Adafruit Thermocouple Sensor w/MAX31855K
 *   https://www.adafruit.com/products/269
 */

#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define AC100RELAY 5
#define FANCTRL 9
#define MAXCS   10
Adafruit_MAX31855 thermocouple(MAXCS);

#define STAT_INIT 0
#define STAT_RAMP 1
#define STAT_SOAK 2
#define STAT_REFLOW 3
#define STAT_REFLOW2 4
#define STAT_COOL 5

int state = STAT_INIT;
int count = 0;
int target = 150;
int target2 = 190;
int target3 = 180;
int upward_inertia = 12;
int soaking_denominator = 2;
int soaking_start;
int soaking_duration = 90;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  //while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc
  Serial.println("Hello!");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(AC100RELAY, OUTPUT);
  pinMode(FANCTRL, OUTPUT);

  // wait for MAX chip to stabilize
  delay(500);

  // basic readout test, just print the current temp
  Serial.print("Internal Temp = ");
  Serial.println(thermocouple.readInternal());

  state = STAT_INIT;
}

// the loop function runs over and over again forever
void loop() {
  bool on = false;
  bool fan_on = false;

  if (500 <= count) {
    delay(1000);
    return;
  }

  count++;
  //Serial.print(count);
  //Serial.print(" "); 

  double c = 0;
  for (int i = 0; i < 10; i++) {
    c += thermocouple.readCelsius();
    delay(100);  
  }
  c /= 10;

  if (isnan(c)) {
    Serial.println("Something wrong with thermocouple!");
    on = false;
  } else {
    switch (state) {
    case STAT_INIT:
      //Serial.print("0 "); 
      if (count < 10) {
        on = false;
      } else {
        on = true;
        state = STAT_RAMP;
      }
      break;
    case STAT_RAMP:
      //Serial.print("10 "); 
      if (c < target - upward_inertia) {
        on = true;
      } else {
        on = false;
        state = STAT_SOAK;
        soaking_start = count;
      }
      break;
    case STAT_SOAK:
      //Serial.print("20 "); 
      on = true;
      if (soaking_start + soaking_duration < count) {
        state = STAT_REFLOW;
      }
      break;
    case STAT_REFLOW:
      //Serial.print("30 "); 
      if (c < target2) {
        on = true;
      } else {
        on = false;
        state = STAT_REFLOW2;
      }
      break;
    case STAT_REFLOW2:
      //Serial.print("30 "); 
      on = false;
      if (c < target3) {
        state = STAT_COOL;
      }
      break;
    case STAT_COOL:
      //Serial.print("40 "); 
      on = false;
      fan_on = true;
      break;
    }
  }

  if (state == STAT_SOAK && on) {
    on = (count % soaking_denominator) == 0 ? true : false;
  }

#if 1
  Serial.print(c);
  Serial.print(" ");
  Serial.print(on ? "15 " : "5 ");
  Serial.print(" ");
  Serial.print(fan_on ? "30 " : "20 ");
  Serial.print(target);
  Serial.print(" ");
  Serial.print(target2);
#endif
#if 0
  Serial.print(" ");
  Serial.print(analogRead(A0)*5.0/1024*10);
  Serial.print(" ");
  Serial.print(analogRead(A1)*5.0/1024*10);
  Serial.print(" ");
  Serial.print(analogRead(A2)*5.0/1024*10);
  Serial.print(" ");
  Serial.print(analogRead(A3)*5.0/1024*10);
#endif
  Serial.println("");

  if (on) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(AC100RELAY, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(AC100RELAY, LOW);
  }

  // XXXX
  fan_on = (count/10)%2;

  if (fan_on) {
    digitalWrite(FANCTRL, HIGH);
  } else {
    digitalWrite(FANCTRL, LOW);
  }
}

