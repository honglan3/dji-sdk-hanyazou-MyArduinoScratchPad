//
// sbus decode program using Arduino Zero Native port and FrSky XM
//
// wiring:  5V, GND and sbus signal -> D0 (RX)
//
// https://github.com/mikeshub/FUTABA_SBUS
//

#include <FUTABA_SBUS.h>

FUTABA_SBUS sBus;

#define Serial SerialUSB

void setup(){
  sBus.begin();
  Serial.begin(115200);
  while (!Serial);
  Serial.print("start...");
}

void loop(){
  sBus.FeedLine();
  if (sBus.toChannels == 1){
    sBus.UpdateServos();
    sBus.UpdateChannels();
    sBus.toChannels = 0;
    Serial.print(sBus.channels[0]);
    Serial.print(",");
    Serial.print(sBus.channels[1]);
    Serial.print(",");
    Serial.print(sBus.channels[2]);
    Serial.print(",");
    Serial.print(sBus.channels[3]);
    Serial.println();
  }
}
