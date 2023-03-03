#include <Arduino.h>
#include <Wire.h>
#include "Tone.h"
#include "Reader.h"
#include <avr/wdt.h>

#include "MS5611.h"

// PIN I2C_SCL              A4 // PC4
// PIN I2C_SDA              A5 // PC5
#define BEEP_PIN            A2 // PC2
//#define RX_PIN            10 // PB2 
//#define TX_PIN            9  // PB1
#define RESET_PIN           2

#define EEPROM_ADDRESE_VALUE  0



float   toneFreqLowpass, pressure, lowpassFast, lowpassSlow, upBarrier, upMax, dwBarrier, dwMax, sim = 0.F;
int16_t ddsAcc;

MS5611 ms5611(Wire);
Tone   myTone;
Reader reader(Serial);


void printBr() {
  Serial.print("UpBr: ");
  Serial.print(upBarrier);
  Serial.print(" UpMax: ");
  Serial.print(upMax);
  Serial.print(" DwBr: ");
  Serial.print(dwBarrier);
  Serial.print(" DwMax ");
  Serial.println(dwMax);
}

// up barrier m/s*100
void setUpBarrier(int up_100) {
  up_100 = constrain(up_100, 0, 400);
  upBarrier=max(25.F, 1.6F*up_100);
  upMax=upBarrier+600.F;
}
// down barrier m/s*100
void setDownBarrier(int down_100) {
  down_100 = constrain(down_100, 0, 400);
  dwBarrier=min(-25.F, -1.6F*down_100);
  dwMax=dwBarrier-200.F;
}

float calibrate(float freq)
{
  freq = constrain(freq, dwMax, upMax);
  freq -= constrain(freq, dwBarrier, upBarrier);
  return  freq;
}



void hardwareReset(uint32_t milis) {
  Serial.end();
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(0,LOW);
  digitalWrite(1,LOW);
  delay(milis);
  digitalWrite(RESET_PIN,LOW);
}

void setup() {
  digitalWrite(RESET_PIN,HIGH);
  Serial.begin(57600);
  ms5611.begin();
  
  while(!ms5611.isReady()) {
    ms5611.loop();
    Serial.print(".");
    delay(1000);
  }

  pressure = ((double)ms5611.getPressure());
  lowpassFast = lowpassSlow = pressure;
  toneFreqLowpass = 0;
  myTone.setVolume(0);
  setUpBarrier(25);
  setDownBarrier(100);
  

}


void loop() {  // run over and over
  reader.loop();
  if(reader.hasLine()) {
    String cmd = reader.getLine();
    if(cmd.startsWith("$BVL") && cmd.indexOf('*') > 0 ) {
      int startIndex = 4;
      while(!isgraph(cmd[startIndex])) startIndex++;
      int value = cmd.substring(startIndex, cmd.indexOf('*')).toInt();
      myTone.setVolume(value);
    } else if(cmd.startsWith("$BUP") && cmd.indexOf('*') > 0 ) {
      int startIndex = 4;
      while(!isgraph(cmd[startIndex])) startIndex++;
      int value = cmd.substring(startIndex, cmd.indexOf('*')).toInt();
      setUpBarrier(value);
    } else if(cmd.startsWith("$BDW") && cmd.indexOf('*') > 0 ) {
      int startIndex = 4;
      while(!isgraph(cmd[startIndex])) startIndex++;
      int value = cmd.substring(startIndex, cmd.indexOf('*')).toInt();
      setDownBarrier(value);
    } else if(cmd.startsWith("$RST") && cmd.indexOf('*') > 0 ) {
      int startIndex = 4;
      while(!isgraph(cmd[startIndex])) startIndex++;
      int value = cmd.substring(startIndex, cmd.indexOf('*')).toInt();
      hardwareReset(value*1000);
    } else if(cmd.startsWith("$SIM") && cmd.indexOf('*') > 0 ) {
      int startIndex = 4;
      while(!isgraph(cmd[startIndex])) startIndex++;
      sim = cmd.substring(startIndex, cmd.indexOf('*')).toInt()*1.6F;
    }
  }
  
  ms5611.loop();

  
  if(ms5611.isReady()) {
    uint32_t p = ms5611.getPressure();
    pressure = (double) p;
    lowpassFast = lowpassFast + (pressure - lowpassFast) * 0.1F;
    lowpassSlow = lowpassSlow + (pressure - lowpassSlow) * 0.05F;
    float toneFreq = (lowpassSlow - lowpassFast) * 50.F;
    toneFreqLowpass = toneFreqLowpass + (toneFreq - toneFreqLowpass) * 0.1F;
    toneFreq = calibrate(toneFreqLowpass + sim);
    
    ddsAcc += toneFreq * 10 + 1000;

    if ((toneFreq > 1.F && ddsAcc > 0) || (toneFreq < -1.F))  {
      toneFreq += 500;
      myTone.beep(toneFreq);
    } else {
      myTone.beep(0);
    }
    Serial.print("PRS ");
    Serial.println(p, HEX);
  }
}


