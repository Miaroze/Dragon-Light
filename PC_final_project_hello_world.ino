#include <IRremote.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip(15, 8);


//variables
int micVal = 0;
int micPin = A2;
int mappedMicReading;
int remotePin = 7;
//configuring remote
IRrecv irrecv(remotePin);
unsigned long idNumber;
//decoding results from remote
decode_results results;
unsigned long key_value = 0;
//making timer
unsigned long startTime = millis();
unsigned long endTime;
unsigned long startTime1 = millis();
unsigned long endTime1;
long interval = 100;//random(100, 3000);
long interval1 = 20;
//800 - 1022
void setup() {
  Serial.begin(9600);
  //starting remote
  irrecv.enableIRIn();
  irrecv.blink13(true);
  //starting neopixels
  strip.begin();
  strip.clear();
  strip.show();
  strip.setBrightness(20);
  //starting mic
  pinMode(micPin, INPUT);
}

void loop() {
  readMicrophoneTest();
  if (irrecv.decode()) {
    //Serial.println(irrecv.decodedIRData.decodedRawData); // Print the Serial, need this for new version!!!
    results.value = key_value;
    idNumber = irrecv.decodedIRData.decodedRawData;
    Serial.print("new reading: ");
    Serial.println(idNumber);

    key_value = results.value;
    //      //resumes collecting data
    irrecv.resume();
  }

  //starting states
  switch (irrecv.decodedIRData.decodedRawData) {

    //off
    case 3125149440: //button off on remote
      Serial.println("button off");
      strip.clear();
      strip.show();
      break;

    //mic reading
    case 3910598400: //button 0 on remote
      Serial.println("button 0");
      lighting();
      strip.show();
      break; //must have break or multiple cases will run

    //rainbow
    case 4077715200: //button 1 on remote
      Serial.println("button 1");
      if (millis() - startTime1 > interval1) {
        rainbow();
      }
      //endTime1 = millis();
      break;

    //red fire
    case 3877175040: //button 2 on remote
      Serial.println("button 2");
      if (endTime - startTime >= interval) {
        startTime = millis();
        fireLighting();
        strip.show();
      }
      endTime = millis();
      break;

    //blue fire
    case 2707357440: //button 3 on remote
      Serial.println("button 3");
      if (endTime - startTime >= interval) {
        startTime = millis();
        blueLighting();
        strip.show();
      }
      endTime = millis();
      break;

    //lime green fire
    case 4144561920: //button 4 on remote
      Serial.println("button 4");
      if (endTime - startTime >= interval) {
        startTime = millis();
        limeLighting();
        strip.show();
      }
      endTime = millis();
      break;

    //purple fire
    case 3810328320: //button 5 on remote
      Serial.println("button 5");
      if (endTime - startTime >= interval) {
        startTime = millis();
        purpleLighting();
        strip.show();
      }
      endTime = millis();
      break;

    //pink fire
    case 2774204160: //button 6 on remote
      Serial.println("button 6");
      if (endTime - startTime >= interval) {
        startTime = millis();
        pinkLighting();
        strip.show();
      }
      endTime = millis();
      break;

  }
}

void readMicrophoneTest() {
  //test for mic
  micVal = analogRead(micPin);
  //Serial.println(micVal);
}

void lighting() {
  mappedMicReading = map(micVal, 400, 1023, 0, 60000);
  for (int i = 0; i < (strip.numPixels()); i ++) {
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(mappedMicReading)));
  }
  strip.show();

} //end of lighting

void fireLighting() {
  for (int i = 0; i < (strip.numPixels()); i ++) {
    strip.setPixelColor(i, 250, random(0, 100), 0);
  }
  strip.show();
} //end of fireLighting

void blueLighting() {
  for (int i = 0; i < (strip.numPixels()); i ++) {
    strip.setPixelColor(i, 0, random(0, 100), random(0, 250));
  }
  strip.show();
}

void limeLighting() {
  for (int i = 0; i < (strip.numPixels()); i ++) {
    strip.setPixelColor(i, random(0, 50), random(50, 250), random(0, 50));
  }
  strip.show();
}

void purpleLighting() {
  for (int i = 0; i < (strip.numPixels()); i ++) {
    strip.setPixelColor(i, random(0, 150), 0, random(50, 250));
  }
  strip.show();
}

void pinkLighting() {
  for (int i = 0; i < (strip.numPixels()); i ++) {
    strip.setPixelColor(i, random(100, 250), 0, random(0, 150));
  }
  strip.show();
}

//wheel of colors that allows the rainbow to work
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void rainbow() {
  static uint16_t r = 0;
  for (int i = 0; i < (strip.numPixels()); i++) {
    strip.setPixelColor(i, Wheel((i + r) & 255));
  }
  strip.show();
  r++;
  if (r >= 256) {
    r = 0;
  }
  startTime1 = millis(); // time for next change to the display
}//end of rainbow
