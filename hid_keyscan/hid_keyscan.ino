/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/* This sketch demonstate how to use BLE HID to scan an array of pin
 * and send its keycode. It is essentially an implementation of hid keyboard,
 * useful reference if you want to make an BLE keyboard.
 */

#include <bluefruit.h>

BLEHidAdafruit blehid;

#if 0
// Array of pins and its keycode
// For keycode definition see BLEHidGeneric.h
uint8_t pins[]    = { A0, A1, A2, A3, A4, A5 };
uint8_t hidcode[] = { HID_KEY_A, HID_KEY_B, HID_KEY_C ,HID_KEY_D, HID_KEY_E, HID_KEY_F };

uint8_t pincount = sizeof(pins)/sizeof(pins[0]);

// Modifier keys, only take cares of Shift
// ATL, CTRL, CMD keys are left for user excersie.
uint8_t shiftPin = 11;
#endif

const byte nrows = 4;
const byte ncols = 6;
#if defined ARDUINO_NRF52_FEATHER
byte rowPins[] = {7, 27, 30, 16}; //connect to the row pinouts of the keypad
byte columnPins[] = {5, 4, 3, 2, 12, 14}; //connect to the column pinouts of the keypad
#else // ARDUINO_NRF52_FEATHER
byte rowPins[] = {4, 5, 6, 8}; //connect to the row pinouts of the keypad
byte columnPins[] = {21, 20, 19, 18, 15, 14}; //connect to the column pinouts of the keypad
#endif

#define MOD_LCTRL	(KEYBOARD_MODIFIER_LEFTCTRL << 8)
#define MOD_LSHIFT	(KEYBOARD_MODIFIER_LEFTSHIFT << 8)
#define MOD_LALT	(KEYBOARD_MODIFIER_LEFTALT << 8)
#define MOD_LGUI	(KEYBOARD_MODIFIER_LEFTGUI << 8)
#define MOD_RCTRL	(KEYBOARD_MODIFIER_RIGHTCTRL << 8)
#define MOD_RSHIFT	(KEYBOARD_MODIFIER_RIGHTSHIFT << 8)
#define MOD_RALT	(KEYBOARD_MODIFIER_RIGHTALT << 8)
#define MOD_RGUI	(KEYBOARD_MODIFIER_RIGHTGUI << 8)
#define IS_MODIFIER(s)	((s)&0xff00)
#define GET_MODIFIER(s)	(((s)&0xff00)>>8)

int symbols[nrows][ncols] = {
  HID_KEY_Q, HID_KEY_W, HID_KEY_E, HID_KEY_R, HID_KEY_T, HID_KEY_Y,
  HID_KEY_A, HID_KEY_S, HID_KEY_D, HID_KEY_F, HID_KEY_G, HID_KEY_H,
  HID_KEY_Z, HID_KEY_X, HID_KEY_C, HID_KEY_V, HID_KEY_B, HID_KEY_N,
  HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, MOD_LCTRL, MOD_LSHIFT, HID_KEY_RETURN,
};

void scanKeys(byte scandata[nrows][ncols]) {
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
      scandata[r][c] = digitalRead(rowPins[r]);
      delay(1);
    }

    // Set pin to high impedance input. Effectively ends column pulse.
    digitalWrite(columnPins[c], LOW);
    pinMode(columnPins[c], INPUT);
  }

#if 0
  for (byte c=0; c<ncols; c++) {
    for (byte r=0; r<nrows; r++) {
      Serial.print(scandata[r][c]?"x":"_");
    }
  }
  Serial.println();
#endif
}

void setup() 
{
  Serial.begin(115200);

  Serial.println("Bluefruit52 HID Keyscan Example");
  Serial.println("-------------------------------\n");

  Serial.println();
  Serial.println("Go to your phone's Bluetooth settings to pair your device");
  Serial.println("then open an application that accepts keyboard input");

  Serial.println();
  Serial.println("Wire configured Pin to GND to send key");
  Serial.println("Wire Shift Keky to GND if you want to send it in upper case");
  Serial.println();  

  Bluefruit.begin();
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Hanyaduino nRF52");

  /* Start BLE HID
   * Note: Apple requires BLE device must have min connection interval >= 20m
   * ( The smaller the connection interval the faster we could send data).
   * However for HID and MIDI device, Apple could accept min connection interval 
   * up to 11.25 ms. Therefore BLEHidAdafruit::begin() will try to set the min and max
   * connection interval to 11.25  ms and 15 ms respectively for best performance.
   */
  blehid.begin();

  // Set callback for set LED from central
  blehid.setKeyboardLedCallback(set_keyboard_led);

  /* Set connection interval (min, max) to your perferred value.
   * Note: It is already set by BLEHidAdafruit::begin() to 11.25ms - 15ms
   * min = 9*1.25=11.25 ms, max = 12*1.25= 15 ms 
   */
  /* Bluefruit.setConnInterval(9, 12); */

  // Set up and start advertising
  startAdv();
}

void startAdv(void)
{  
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  
  // Include BLE HID service
  Bluefruit.Advertising.addService(blehid);

  // There is enough room for the dev name in the advertising packet
  Bluefruit.Advertising.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

bool keyPressedPreviously = false;
byte prev_scandata[nrows][ncols];

void loop()
{
  /*-------------- San Pin Array and send report ---------------------*/
  byte scandata[nrows][ncols];
  bool anyKeyPressed = false;
  uint8_t modifier = 0;
  uint8_t count=0;
  uint8_t keycode[6] = { 0 };

  scanKeys(scandata);

  // scan mofidier key (only SHIFT), user implement ATL, CTRL, CMD if needed
  for (byte c=0; c<ncols; c++) {
    for (byte r=0; r<nrows; r++) {
      if (scandata[r][c] && IS_MODIFIER(symbols[r][c])) {
        modifier |= GET_MODIFIER(symbols[r][c]);
      }
    }
  }

  // scan normal key and send report
  for (byte c=0; c<ncols; c++) {
    for (byte r=0; r<nrows; r++) {
      if (scandata[r][c] && !IS_MODIFIER(symbols[r][c])) {
        keycode[count++] = symbols[r][c];

        // used later
        anyKeyPressed = true;
        keyPressedPreviously = true;

        // 6 is max keycode per report
        if ( count == 6) {
          blehid.keyboardReport(modifier, keycode);

          // reset report
          count = 0;
          memset(keycode, 0, 6);
        }
      }
    }    
  }

  // Send any remaining keys (not accumulated up to 6)
  if ( count ) {
    blehid.keyboardReport(modifier, keycode);
  }

  // Send All-zero report to indicate there is no keys pressed
  // Most of the time, it is, though we don't need to send zero report
  // every loop(), only a key is pressed in previous loop() 
  if ( !anyKeyPressed && keyPressedPreviously )
  {
    keyPressedPreviously = false;
    //Serial.println("blehid.keyRelease()");
    blehid.keyRelease();
  }  

  memcpy(prev_scandata, scandata, sizeof(scandata));

  // Poll interval
  delay(10);
}

/**
 * Callback invoked when received Set LED from central.
 * Must be set previously with setKeyboardLedCallback()
 *
 * The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
 *    Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
 */
void set_keyboard_led(uint8_t led_bitmap)
{
  // light up Red Led if any bits is set
  if ( led_bitmap )
  {
    ledOn( LED_RED );
  }
  else
  {
    ledOff( LED_RED );
  }
}
