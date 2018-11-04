#include <Arduino.h>

const byte nrows = 4;
const byte ncols = 6;
#if defined ARDUINO_NRF52_FEATHER
byte rowPins[] = {7, 27, 30, 16}; //connect to the row pinouts of the keypad
byte columnPins[] = {5, 4, 3, 2, 12, 14}; //connect to the column pinouts of the keypad
#else // ARDUINO_NRF52_FEATHER
byte rowPins[] = {4, 5, 6, 8}; //connect to the row pinouts of the keypad
byte columnPins[] = {21, 20, 19, 18, 15, 14}; //connect to the column pinouts of the keypad
#endif

byte inputs[nrows][ncols];
char symbols[nrows][ncols] = {
  'q', 'w', 'e', 'r', 't', 'y',
  'a', 's', 'd', 'f', 'g', 'h',
  'z', 'x', 'c', 'v', 'b', 'n',
  '1', '2', '3', '4', '5', '6',
};

void scanKeys() {
  // Re-intialize the row pins. Allows sharing these pins with other hardware.
  for (byte r=0; r<nrows; r++) {
    pinMode(rowPins[r], INPUT_PULLDOWN);
    digitalWrite(rowPins[r], LOW);
  }

  // bitMap stores ALL the keys that are being pressed.
  for (byte c=0; c<ncols; c++) {
    pinMode(columnPins[c], OUTPUT);
    digitalWrite(columnPins[c], HIGH);  // Begin column pulse output.
    for (byte r=0; r<nrows; r++) {
      inputs[r][c] = digitalRead(rowPins[r]);
      delay(1);
    }

    // Set pin to high impedance input. Effectively ends column pulse.
    digitalWrite(columnPins[c], LOW);
    pinMode(columnPins[c], INPUT);
  }

#if 1
  for (byte c=0; c<ncols; c++) {
    for (byte r=0; r<nrows; r++) {
      if (inputs[r][c]) {
        Serial.print(symbols[r][c]);
        if (symbols[r][c] == '6') {
          Serial.print('\n');
        }
      }
    }
  }
#endif
#if 0
  for (byte c=0; c<ncols; c++) {
    for (byte r=0; r<nrows; r++) {
      Serial.print(inputs[r][c]?"x":"_");
    }
  }
  Serial.println();
#endif
}

void setup() {
  Serial.begin(9600);
  while (!Serial)
      ;
  Serial.println("Hello Keypad!");
}

void loop() {
  // put your main code here, to run repeatedly:
  scanKeys();
  delay(100);
}
