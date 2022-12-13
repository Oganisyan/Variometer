#include <Arduino.h>
#include "MS5611.h"





MS5611::MS5611(TwoWire &wire) : wire_(wire), address_(0x77),
  callOrderTime_(millis()+500), callOrderCode_ (0x00)
{
  for (size_t i=0; i <6;i++)
    C[i] = 0;
}

bool MS5611::pressureIsOk(uint32_t pressure) {
  return pressure > 30000 && pressure < 125000;  
}

bool MS5611::isReady() {
 bool rv = callOrderCode_ == 0x00 && pressureIsOk(pressure_);
 return rv;
}
    
void MS5611::begin()
{

  wire_.beginTransmission(address_);
  wire_.write(0x1E); // reset
  wire_.endTransmission();
  delayMicroseconds(20000);


  for (uint8_t i=0; i<6  ; i++) {
    wire_.beginTransmission(address_);
    wire_.write(0xA2 + (i * 2));
    wire_.endTransmission();

    wire_.beginTransmission(address_);
    wire_.requestFrom((uint8_t) address_, (uint8_t) 0x06);
    delayMicroseconds(1000);
    if(wire_.available()) {
      C[i] = ((uint16_t) wire_.read()) << 8 | (uint16_t) wire_.read();
    }
  }
}

void  MS5611::orderValue(char code) {
  wire_.beginTransmission(address_);
  wire_.write(code);
  wire_.endTransmission();
  callOrderTime_ = millis() + 10;
  callOrderCode_ = code;
}

uint32_t  MS5611::getValue() 
{
  uint32_t ret = 0;
  callOrderCode_ = 0x00;
  // start read sequence
  wire_.beginTransmission(address_);
  wire_.write((char) 0x00);
  wire_.endTransmission();
  wire_.beginTransmission(address_);
  wire_.requestFrom((uint8_t)address_, (uint8_t) 0x03);
  
  if(wire_.available() >= 3) {
    ret = ((uint32_t) wire_.read()) << 16 | 
          ((uint32_t) wire_.read()) << 8  | 
          ((uint32_t) wire_.read());
  } else {
    ret = INVALID_UNIT32;
  }
  wire_.endTransmission();
  return ret;
}

void MS5611::loop()
{
  if(callOrderTime_ > millis()) {
     return;
  }
  switch(callOrderCode_) {
    default: 
       orderValue(0x58); // Order temperature
       break;     
    case 0x58:  // wait for raw temperature
       if((rawT_= getValue()) == INVALID_UNIT32){
          orderValue(callOrderCode_); // Repeat order temperature
       } else {
          orderValue(0x48); // Order pressure
       }
       break;     
    case 0x48:          // wait for raw pressure
       if((rawP_= getValue()) == INVALID_UNIT32){
          orderValue(callOrderCode_); // Repeat order pressure
       } else {       
          scaleValues();
       }
       break;     
    }
}

void MS5611::scaleValues(){
  
  int32_t dT = rawT_ - (uint32_t)C[4] * 256;
  
  int64_t OFF  = (int64_t)C[1] * 65536 + (int64_t)C[3] * dT / 128;
  int64_t SENS = (int64_t)C[0] * 32768 + (int64_t)C[2] * dT / 256;

  int32_t TEMP = 2000 + ((int64_t) dT * C[5]) / 8388608;

  int64_t OFF2 = 0;
  int64_t SENS2 = 0;

  if (TEMP < 2000) {
    OFF2 =  5 * (TEMP - 2000) * (TEMP - 2000) / 2;
    SENS2 = 5 * (TEMP - 2000) * (TEMP - 2000) / 4;
    if (TEMP < -1500){
        OFF2 =  OFF2  + 7  * (TEMP + 1500) * (TEMP + 1500);
        SENS2 = SENS2 + 11 * (TEMP + 1500) * (TEMP + 1500) / 2;
    }
  }
  OFF = OFF - OFF2;
  SENS = SENS - SENS2;
  
  int32_t T2 = dT/32768 * dT/32768 / 2;
  TEMP = TEMP - T2;
  
  temperature_ = (float) TEMP/ 100.F;  
  uint32_t pressure =  ( rawP_ * SENS / 2097152 - OFF) / 32768;

  if(pressureIsOk(pressure)) {
    pressure_ = pressure;
  }

}


int32_t MS5611::getTemperature()
{
  return temperature_;
}


uint32_t  MS5611::getPressure()
{
  return pressure_;
}
