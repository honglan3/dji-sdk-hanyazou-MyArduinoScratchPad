/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
  
  modified 8 Sep 2016
  by Colby Newman
*/

//#define HAVE_SERIAL1
//#define HAVE_SERIAL2
#if 1
#undef LED_BUILTIN
#define LED_BUILTIN 2  // for SAM D11C
//#define LED_BUILTIN 3  // for Hanyaduino
//#define LED_BUILTIN 27  // for SAMD21E breakout Rev 0.5
#endif

// the setup function runs once when you press reset or power the board
void setup() {
  //delay(1000);
  Serial.begin(9600);
  //Serial.begin();
  while (!Serial);
  Serial.println("Hello!");
  Serial.print("LED_BUILTIN=");
  Serial.println(LED_BUILTIN);
#ifdef HAVE_SERIAL1
  Serial1.begin(9600);
  Serial1.println("Hello!");
#endif
#ifdef HAVE_SERIAL2
  Serial2.begin(9600);
  Serial2.println("Hello!");
#endif

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("running... on serial 0");
#ifdef HAVE_SERIAL1
  Serial1.println("running... on serial 1");
#endif
#ifdef HAVE_SERIAL2
  Serial2.println("running... on serial 2");
#endif
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
