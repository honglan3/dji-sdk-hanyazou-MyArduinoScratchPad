#include <Wire.h>
#include <i2cEncoderLibV2.h>

/*
 * This is a basic example for using the I2C Encoder V2
 * The counter is set to work between +10 to -10, at every encoder click the counter value is
 * printed on the terminal.
 * It's also printet when the push button is pressed, or released or double pushed.
 *
 * https://github.com/Fattoresaimon/I2CEncoderV2
 */

const int IntPin = -1; /* Definition of the interrupt pin (negative value means polling) */
byte addrs[] = { 0x14, 0x15 };
#define NADDRS (sizeof(addrs)/sizeof(*addrs))
uint32_t colors[NADDRS] = {0x04A9FC, 0xD2EE841 };

i2cEncoderLibV2 *encoders[NADDRS];
int nencoders = 0;

void setup(void)
{
  if (0 <= IntPin)
    pinMode(IntPin, INPUT);

  Serial.begin(115200);
  while (!Serial);
  Serial.println("**** I2C Encoder V2 basic example ****");
  /*
   * INT_DATA= The register are considered integer.
   * WRAP_DISABLE= The WRAP option is disabled
   * IPUP_ENABLE= INT pin have the pull-up enabled.
   * RMOD_X1= Encoder configured as X1.
   * RGB_ENCODER= type of encoder is RGB
   */

  Wire.begin();
  for (int i = 0; i < NADDRS; i++) {
    Wire.beginTransmission(addrs[i]);
    if (Wire.endTransmission() != 0) {
      continue;
    }
    Serial.print("Encoder");
    Serial.print(nencoders);
    Serial.print(" at ");
    Serial.print(addrs[i], DEC);
    Serial.print(" (0x");
    Serial.print(addrs[i], HEX);
    Serial.println(")");

    encoders[nencoders] = new i2cEncoderLibV2(addrs[i]);
    encoders[nencoders]->begin(INT_DATA | WRAP_DISABLE | IPUP_ENABLE | RMOD_X1 | RGB_ENCODER);
    encoders[nencoders]->writeCounter((int32_t)0); /* Reset the counter value */
    encoders[nencoders]->writeMax((int32_t)10); /* Set the maximum threshold*/
    encoders[nencoders]->writeMin((int32_t)-10); /* Set the minimum threshold */
    encoders[nencoders]->writeStep((int32_t)1); /* Set the step to 1*/
    encoders[nencoders]->writeInterruptConfig(0xff); /* Enable all the interrupt */
    encoders[nencoders]->writeAntibouncingPeriod(20);  /* Set an anti-bouncing of 200ms */
    encoders[nencoders]->writeDoublePushPeriod(50);  /*Set a period for the double push of 500ms */

    encoders[nencoders]->writeFadeRGB(2); // Fade enabled with 2ms step
    encoders[nencoders]->writeRGBCode(colors[i]);  // Update the encoder with the RGB code
    nencoders++;
  }
}

void loop() {
  if (IntPin < 0 || digitalRead(IntPin) == LOW) {
    for (int i = 0; i < nencoders; i++) {
      if ( encoders[i] == NULL ) {
        continue;
      }

      if ( encoders[i]->updateStatus()) {
        /*
	 * Counter
	 */
        if ( encoders[i]->readStatus(RINC) ||
             encoders[i]->readStatus(RDEC) ||
             encoders[i]->readStatus(RMAX) |
             encoders[i]->readStatus(RMIN)) {
          Serial.print("Encoder");
          Serial.print(i);
          Serial.print(": ");
          if ( encoders[i]->readStatus(RINC)) {
            Serial.print("Increment ");
          }
          if ( encoders[i]->readStatus(RDEC)) {
            Serial.print("Decrement ");
          }

          if ( encoders[i]->readStatus(RMAX)) {
            Serial.print("Maximum threshold ");
          }

          if ( encoders[i]->readStatus(RMIN)) {
            Serial.print("Minimum threshold ");
	  }
          Serial.println(encoders[i]->readCounterByte());
        }

        /*
	 * Push button
	 */
        if ( encoders[i]->readStatus(PUSHR) ||
             encoders[i]->readStatus(PUSHP) ||
             encoders[i]->readStatus(PUSHD)) {
          Serial.print("Encoder");
          Serial.print(i);
          Serial.print(": Push button ");
          if ( encoders[i]->readStatus(PUSHP)) {
            Serial.print("Pressed ");
          }
          if ( encoders[i]->readStatus(PUSHR)) {
            Serial.print("Released ");
          }
          if ( encoders[i]->readStatus(PUSHD)) {
            Serial.print("Double push! ");
          }
          Serial.println();
	}
      }
    }
  }

  /* give some delay if you use polling */
  if (IntPin < 0)
    delay(10);
}
