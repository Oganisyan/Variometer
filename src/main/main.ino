#include <Arduino.h>
#include "Tone.h"

#if 0

Tone myTone;


void setup() {
  Serial.begin(9600);
  myTone.beep(1000);
}

void loop() {  // run over and over
  if (Serial.available()) {
    char d = Serial.read();
    if(d == '+') {
      myTone.setVolume(myTone.getVolume()+1);
      Serial.print("Up: ");
      Serial.println(myTone.getVolume());
    } else if(d == '-') {
      myTone.setVolume(myTone.getVolume()-1);
      Serial.print("Dw: ");
      Serial.println(myTone.getVolume());
    } else if(d == 'd') {
      myTone.setVolume(0);
      Serial.print("Off: ");
      Serial.println(myTone.getVolume());
    }
      myTone.beep(1000);
  } 
}



#else

#include "MS5611.h"

// PIN I2C_SCL              A4 // PC4
// PIN I2C_SDA              A5 // PC5
#define BEEP_PIN            A2 // PC2
//#define RX_PIN            10 // PB2 
//#define TX_PIN            9  // PB1


float calibrate(float freq, float downMin, float downMax, float upMin, float upMax ) 
{
  freq -= constrain(freq, downMin, upMin);
  return  constrain(freq, downMax, upMax);
}


float toneFreq, toneFreqLowpass, pressure, lowpassFast, lowpassSlow;
int16_t ddsAcc;



MS5611   ms5611(Wire);
Tone myTone;

void setup() {
  ms5611.begin();
  Serial.begin(9600);
  while(!ms5611.isReady()) {
    ms5611.loop();
    delayMicroseconds(1000);
  }
  pressure = ((float)ms5611.getPressure());
  lowpassFast = lowpassSlow = pressure;
  myTone.setVolume(1);

}


void loop() {  // run over and over
  if (Serial.available()) {
    char d = Serial.read();
    if(d == '+') {
      myTone.setVolume(myTone.getVolume()+1);
      Serial.print("Up: ");
      Serial.println(myTone.getVolume());
    } else if(d == '-') {
      myTone.setVolume(myTone.getVolume()-1);
      Serial.print("Dw: ");
      Serial.println(myTone.getVolume());
    } else if(d == 'd') {
      myTone.setVolume(0);
      Serial.print("Off: ");
      Serial.println(myTone.getVolume());
    }
  } 
  ms5611.loop();
  if(ms5611.isReady()) {
    uint32_t p = ms5611.getPressure();;
    pressure = ((float)p);
    lowpassFast = lowpassFast + (pressure - lowpassFast) * 0.1F;
    lowpassSlow = lowpassSlow + (pressure - lowpassSlow) * 0.05F;
    toneFreq = (lowpassSlow - lowpassFast) * 50;
    toneFreqLowpass = toneFreqLowpass + (toneFreq - toneFreqLowpass) * 0.1;
    toneFreq = calibrate(toneFreqLowpass, -25, -300, 25, 500 );
    ddsAcc += toneFreq * 20 + 400;

    Serial.print(toneFreq);
    Serial.println();
    if ((toneFreq > 1 && ddsAcc > 0) || (toneFreq < -1))  {
      myTone.beep(toneFreq + 610);
    } else {
      myTone.beep(0);
    }
    Serial.print("PRS ");
    Serial.println(p, HEX);
  }
}

#endif
