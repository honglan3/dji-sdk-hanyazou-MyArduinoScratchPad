/*
 * http://psyclesam.blogspot.jp/2013/03/hpdl-1414-test.html
 */
 /* Test Sketch for HPDL-1414
 
 Wiring
 HPDL-1414               Arduino
 Pin   Function          Pin
 1     D5 Data Input     D5
 2     D4 Data Input     D4
 3     WR Wrtie          D10
 4     A1 Digit Select   D9
 5     A0 Digit Select   D8
 6     Vcc 5v            5V
 7     GND               GND
 8     D0 Data Input     D0
 9     D1 Data Input     D1
 10    D2 Data Input     D2
 11    D3 Data Input     D3
 12    D6 Data Input     D6
 
 
 */

const int dTime = 25;  // delay beteewn digit writes
const int lTime = 250; // delay between loops
const int cMin = ' '; // 33;   // start Decimal Ascii value 32(space) to 95 (underscore) 
const int cMax = '_'; //95;   // start Decimal Ascii value 32(space) to 95 (underscore)


void setup() {
  DDRD = DDRD | B11111111; // set direction bits for pins 7-0
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop() {
  int asciiDec = cMin;
  while(asciiDec <= cMax)  {
    // loop ascii set
    PORTD = (asciiDec);
    delay(1);                // wait for a ms 
    digitalWrite(8, HIGH);   // 8=A0 9=A1 select digit HH=3 LH=2 HL=1 LL=0
    digitalWrite(9, HIGH);   // 8=A0 9=A1 select digit HH=3 LH=2 HL=1 LL=0
    delay(1);                // wait for a ms    
    digitalWrite(10, LOW);   // Write pin Low
    delay(1);                // wait for a ms
    digitalWrite(10, HIGH);  // Write Pin High
    delay(dTime);            // delay between characters ms seconds

    PORTD = (asciiDec);
    delay(1);                // wait for a ms 
    digitalWrite(8, LOW);    // select digit
    digitalWrite(9, HIGH);   // select digit
    delay(1);                // wait for a ms
    digitalWrite(10, LOW);   // Write pin Low
    delay(1);                // wait for a ms
    digitalWrite(10, HIGH);  // Write Pin High
    delay(dTime);            // delay between characters ms seconds

    PORTD = (asciiDec);
    delay(1);                // wait for a ms 
    digitalWrite(8, HIGH);   // select digit
    digitalWrite(9, LOW);    // select digit
    delay(1);                // wait for a ms
    digitalWrite(10, LOW);   // Write pin Low
    delay(1);                // wait for a ms
    digitalWrite(10, HIGH);  // Write Pin High
    delay(dTime);            // delay between characters ms seconds  

    PORTD = (asciiDec);
    delay(1);                // wait for a ms 
    digitalWrite(8, LOW);    // select digit
    digitalWrite(9, LOW);    // select digit
    delay(1);                // wait for a ms
    digitalWrite(10, LOW);   // Write pin Low
    delay(1);                // wait for a ms
    digitalWrite(10, HIGH);  // Write Pin High
    delay(dTime);            // delay between characters ms seconds

    asciiDec++;              // increment character
    delay(lTime);            // loop delay ms seconds
  }

}

