/*  
 *   Adafruit Thermocouple Sensor w/MAX31855K
 *   https://www.adafruit.com/products/269
 */

#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define AC100RELAY 5
#define MAXCS   10
Adafruit_MAX31855 thermocouple(MAXCS);

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc
  Serial.println("Hello!");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(AC100RELAY, OUTPUT);

  // wait for MAX chip to stabilize
  delay(500);
}

// the loop function runs over and over again forever
void loop() {
  // basic readout test, just print the current temp
  Serial.print("Internal Temp = ");
  Serial.println(thermocouple.readInternal());

  double c = thermocouple.readCelsius();
  if (isnan(c)) {
    Serial.println("Something wrong with thermocouple!");
  } else {
    Serial.print("C = "); 
    Serial.println(c);
  }

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(AC100RELAY, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(AC100RELAY, LOW);
  delay(5000);
}

