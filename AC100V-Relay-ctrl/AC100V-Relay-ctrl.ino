/*  
 *   Adafruit Thermocouple Sensor w/MAX31855K
 *   https://www.adafruit.com/products/269
 */

#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define AC100RELAY 5
#define MAXCS   10
Adafruit_MAX31855 thermocouple(MAXCS);

#define STAT_INIT 0
#define STAT_RAMP 1
#define STAT_SOAK 2
#define STAT_REFLOW 3
#define STAT_COOL 4

int state = STAT_INIT;
int count = 0;
int target = 150;
int target2 = 190;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  //while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc
  Serial.println("Hello!");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(AC100RELAY, OUTPUT);

  // wait for MAX chip to stabilize
  delay(500);

  // basic readout test, just print the current temp
  Serial.print("Internal Temp = ");
  Serial.println(thermocouple.readInternal());

  state = STAT_RAMP;
}

// the loop function runs over and over again forever
void loop() {
  bool on;

  if (500 <= count)
    return;
  count++;
  //Serial.print(count);
  //Serial.print(" "); 

  double c = thermocouple.readCelsius();
  if (isnan(c)) {
    Serial.println("Something wrong with thermocouple!");
    on = false;
  } else {
    switch (state) {
    case STAT_RAMP:
      //Serial.print("10 "); 
      if (c < target) {
        on = true;
      } else {
        on = false;
        state = STAT_SOAK;
      }
      break;
    case STAT_SOAK:
      //Serial.print("20 "); 
      if (c < target) {
        on = true;
        state = STAT_REFLOW;
      } else {
        on = false;
      }
      break;
    case STAT_REFLOW:
      //Serial.print("30 "); 
      if (c < target2) {
        on = true;
      } else {
        on = false;
        state = STAT_COOL;
      }
      break;
    case STAT_COOL:
      //Serial.print("40 "); 
      on = false;
      break;
    }
  }
#if 0
  Serial.print("C = "); 
  Serial.print(c);
  Serial.print(" "); 
  Serial.print(on ? "ON" : "OFF");
  Serial.println(""); 
#else
  Serial.print(c);
  Serial.print(" ");
  Serial.print(on ? "15 " : "5 ");
  Serial.print(target);
  Serial.print(" ");
  Serial.print(target2);
  Serial.println("");
#endif

  if (on) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(AC100RELAY, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(AC100RELAY, LOW);
  }

  delay(1000);
}

