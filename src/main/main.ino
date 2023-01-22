#include <Arduino.h>
#include <Wire.h>
#include "Tone.h"
#include "Reader.h"

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
Reader reader(Serial);



void setup() {
  Serial.begin(57600);
  ms5611.begin();
  
  while(!ms5611.isReady()) {
    ms5611.loop();
    Serial.print(".");
    delay(1000);
  }

  pressure = ((float)ms5611.getPressure());
  lowpassFast = lowpassSlow = pressure;
  myTone.setVolume(0);
  

}


void loop() {  // run over and over

  reader.loop();
  if(reader.hasLine()) {
    String cmd = reader.getLine();
    if(cmd.startsWith("$BVL ") && cmd.indexOf('*') > 0 ) {
      int value = cmd.substring(5, cmd.indexOf('*')).toInt();
      myTone.setVolume(value);
    }
  }
  
  ms5611.loop();
  if(ms5611.isReady()) {
    uint32_t p = ms5611.getPressure();
    pressure = (float) p;
    lowpassFast = lowpassFast + (pressure - lowpassFast) * 0.1F;
    lowpassSlow = lowpassSlow + (pressure - lowpassSlow) * 0.05F;
    toneFreq = (lowpassSlow - lowpassFast) * 50.F;
    toneFreqLowpass = toneFreqLowpass + (toneFreq - toneFreqLowpass) * 0.1F;
    toneFreq = calibrate(toneFreqLowpass, -25.F, -300.F, 25.F, 500.F );
    
    ddsAcc += toneFreq * 20 + 400;

    if ((toneFreq > 1 && ddsAcc > 0) || (toneFreq < -1))  {
      myTone.beep(toneFreq + 610);
    } else {
      myTone.beep(0);
    }
    Serial.print("PRS ");
    Serial.println(p, HEX); 
  }
}

