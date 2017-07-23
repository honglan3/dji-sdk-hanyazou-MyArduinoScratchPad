/*
** Rotary Encoder Example
** Use the Sparkfun Rotary Encoder to vary brightness of LED
**
** Sample the encoder at 200Hz using the millis() function
* 
* http://www.hobbytronics.co.uk/arduino-tutorial6-rotary-encoder
*/

int brightness = 120;    // how bright the LED is, start at half brightness
int fadeAmount = 10;    // how many points to fade the LED by
unsigned long currentTime;
unsigned long loopTime;
const int pin_A = 3;
const int pin_B = 2;
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define RGBLED_PIN 8
#define NUM_RGBLEDS 3
#define BRIGHTNESS 50
Adafruit_NeoPixel rgbled = Adafruit_NeoPixel(NUM_RGBLEDS, RGBLED_PIN, NEO_GRB + NEO_KHZ800);

void setup()  {
  Serial.begin(9600);

  // declare pin 9 to be an output:
  pinMode(9, OUTPUT);
  pinMode(pin_A, INPUT_PULLUP);
  pinMode(pin_B, INPUT_PULLUP);
  currentTime = millis();
  loopTime = currentTime;

  rgbled.setBrightness(BRIGHTNESS);
  rgbled.begin();
  rgbled.show(); // Initialize all pixels to 'off'

  //rgbled.setPixelColor(0, 0xffffff); // Red
  //rgbled.setPixelColor(1, 0xffffff); // Red
  //rgbled.setPixelColor(2, 0xffffff); // Red
  rgbled.setPixelColor(0, rgbled.Color(255, 0, 0)); // red
  rgbled.setPixelColor(1, rgbled.Color(0, 255, 0)); // green
  rgbled.setPixelColor(2, rgbled.Color(0, 0, 255)); // blue
  rgbled.show(); // Initialize all pixels to 'off'
  delay(1000);
  Serial.println("ready.");
}

#define a(v) ((v)%255)

void set_rgbled(uint32_t color)
{
  int r = color;
  rgbled.setPixelColor(0, rgbled.Color(a(r+170), a(r+85), a(r+0)));
  rgbled.setPixelColor(1, rgbled.Color(a(r+85), a(r+0), a(r+170)));
  rgbled.setPixelColor(2, rgbled.Color(a(r+0), a(r+170), a(r+85)));

  rgbled.show();
}

void loop()  {
  // get the current elapsed time
  currentTime = millis();
  if(currentTime >= (loopTime + 5)){
    // 5ms since last check of encoder = 200Hz  
    encoder_A = digitalRead(pin_A);    // Read encoder pins
    encoder_B = digitalRead(pin_B);   
    if((!encoder_A) && (encoder_A_prev)){
      // A has gone from high to low 
      if(encoder_B) {
        // B is high so clockwise
        // increase the brightness, dont go over 255
        if(brightness + fadeAmount <= 255) brightness += fadeAmount;               
      }   
      else {
        // B is low so counter-clockwise      
        // decrease the brightness, dont go below 0
        if(brightness - fadeAmount >= 0) brightness -= fadeAmount;               
      }
    }   
    encoder_A_prev = encoder_A;     // Store value of A for next time    
    
    // set the brightness of pin 9:
    //analogWrite(9, brightness);   
    Serial.println(brightness);
    set_rgbled(brightness);

    loopTime = currentTime;  // Updates loopTime
  }
  // Other processing can be done here
                           
}

