/*  
 *   Adafruit Thermocouple Sensor w/MAX31855K
 *   https://www.adafruit.com/products/269
 */

#include <SPI.h>
#include "Adafruit_MAX31855.h"

//#define DBEUG_SERIAL_MONITOR
//#define DEBUG_ON_OFF
//#define DEBUG_FANCTRL

#define AC100RELAY 5
#define FANCTRL 6
#define MAXCS   7
Adafruit_MAX31855 thermocouple(MAXCS);

enum {
  STAT_START,
  STAT_PRECOOLDOWN,
  STAT_PREHEAT,
  STAT_SOAK,
  STAT_REFLOW,
  STAT_REFLOW2,
  STAT_COOLDOWN,
  STAT_END
};

int state;
int count = 0;
int target0 = 50;
int target1 = 150;
int target2 = 210;
int target3 = 190;
int upward_inertia = 12;
int soaking_denominator = 2;
int soaking_start;
int soaking_duration = 90;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
#ifdef DBEUG_SERIAL_MONITOR
  while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc
  Serial.println("Hello!");
#endif

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(AC100RELAY, OUTPUT);
  pinMode(FANCTRL, OUTPUT);

  // wait for MAX chip to stabilize
  delay(500);

#ifdef DBEUG_SERIAL_MONITOR
  // basic readout test, just print the current temp
  Serial.print("Internal Temp = ");
  Serial.println(thermocouple.readInternal());
#endif

  state = STAT_PRECOOLDOWN;
}

// the loop function runs over and over again forever
void loop() {
  bool heater_on = false;
  bool fan_on = false;

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
    heater_on = false;
  } else {
    switch (state) {
    case STAT_PRECOOLDOWN:
      //Serial.print("0 "); 
      if (c < target0) {
        heater_on = true;
        state = STAT_PREHEAT;
      } else {
        fan_on = true;
      }
      break;
    case STAT_PREHEAT:
      //Serial.print("10 "); 
      if (c < target1 - upward_inertia) {
        heater_on = true;
      } else {
        heater_on = false;
        state = STAT_SOAK;
        soaking_start = count;
      }
      break;
    case STAT_SOAK:
      //Serial.print("20 "); 
      heater_on = true;
      if (soaking_start + soaking_duration < count) {
        state = STAT_REFLOW;
      }
      break;
    case STAT_REFLOW:
      //Serial.print("30 "); 
      if (c < target2) {
        heater_on = true;
      } else {
        heater_on = false;
        state = STAT_REFLOW2;
      }
      break;
    case STAT_REFLOW2:
      //Serial.print("30 "); 
      heater_on = false;
      if (c < target3) {
        state = STAT_COOLDOWN;
      }
      break;
    case STAT_COOLDOWN:
      //Serial.print("40 "); 
      if (c < 50) {
        state = STAT_END;
      } else {
        fan_on = true;
      }
      break;
    case STAT_END:
      // do nothing
      break;
    }
  }

  if (state == STAT_SOAK && heater_on) {
    heater_on = (count % soaking_denominator) == 0 ? true : false;
  }

#ifdef DEBUG_ON_OFF
  fan_on = (count/10)%2;
  heater_on = (count/3)%2;
#endif

  //if (count < 498) 
  {
    Serial.print(c);
    Serial.print(" ");
    Serial.print(heater_on ? "8 " : "3 ");
    Serial.print(" ");
    Serial.print(fan_on ? "15 " : "10 ");
    Serial.print(target1);
    Serial.print(" ");
    Serial.print(target2);
#ifdef DEBUG_FANCTRL
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
  }

  if (heater_on) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(AC100RELAY, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(AC100RELAY, LOW);
  }

  if (fan_on) {
    digitalWrite(FANCTRL, HIGH);
  } else {
    digitalWrite(FANCTRL, LOW);
  }
}

