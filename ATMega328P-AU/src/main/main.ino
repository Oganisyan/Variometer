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



float   toneFreqLowpass, pressure, lowpassFast, lowpassSlow, upBarrier, upMax, dwBarrier, dwMax, sim = 0.F, P1, P2, P3, P4;

int16_t ddsAcc;

MS5611 ms5611(Wire);
Tone   myTone;
Reader reader(Serial);


void sensitivity (int p) {
  p =  constrain(p, 0, 5); 
  P1 = (5.F+ p)/50;
  P2 = P1 / 2;
  P3 = 500.F * P1;
  P4 = P3 * P1;
}

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

void sendPressure(uint32_t p) {
  static char buffer[32];
  int len = sprintf(buffer, "PRS %lX\n", p);
  Serial.write(buffer, len);
}

void setup() {
  digitalWrite(RESET_PIN,HIGH);
  Serial.begin(57600);
  ms5611.begin();
  sensitivity(3);
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
  static boolean run = true;
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
    } else if(cmd.startsWith("$SEN") && cmd.indexOf('*') > 0 ) {
      int startIndex = 4;
      while(!isgraph(cmd[startIndex])) startIndex++;
      int value = cmd.substring(startIndex, cmd.indexOf('*')).toInt();
      sensitivity(value);
    } else if(cmd.startsWith("$SIM") && cmd.indexOf('*') > 0 ) {
      int startIndex = 4;
      while(!isgraph(cmd[startIndex])) startIndex++;
      sim = cmd.substring(startIndex, cmd.indexOf('*')).toInt()*1.6F;
    } else if(cmd.startsWith("$?")) {
      static const char *info =
    		  "$BVL value*   - set beep value (0 <-> 10)\n"\
    		  "$BUP barrier* - set up barrier (0=0,25m/s <-> 400=7m/s)\n"\
    		  "$BDW barrier* - set down barrier (0=0,25m/s <-> 400=7m/s)\n"\
    		  "$SEN sensy*   - set sensitivity (0 <-> 5)\n"\
    		  "$SIM sim*     - simulate set down barrier (-600 <-> 600)\n"\
    		  "$?            - print cmds\n\n"\
	          "send again: $? to run\n";
	  if(run)
		Serial.write(info);
	  run = !run;
    }
  }
  
  ms5611.loop();

  
  if(ms5611.isReady()) {
    uint32_t p = ms5611.getPressure();
    pressure = (double) p;
        
    lowpassFast = lowpassFast + (pressure - lowpassFast) * P1;
    lowpassSlow = lowpassSlow + (pressure - lowpassSlow) * P2;
    float toneFreq = (lowpassSlow - lowpassFast) * P3;
    toneFreqLowpass = toneFreqLowpass + (toneFreq - toneFreqLowpass) * P1;
    toneFreq = calibrate(toneFreqLowpass + sim);
    
    ddsAcc += toneFreq * 10 + 1000;

    if ((toneFreq > P4 && ddsAcc > 0) || (toneFreq < -P4))  {
      toneFreq += toneFreq > 0 ? 550 : 450;
      myTone.beep(toneFreq);
    } else {
      myTone.beep(0);
    }
    if(run)
      sendPressure(p);
  }
}


