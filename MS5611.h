#ifndef _MS5611_H_
#define _MS5611_H_
#include <Wire.h>



#ifndef INVALID_UNIT32
#define  INVALID_UNIT32 0xFFFFFFFF
#endif


class MS5611 // @suppress("Class has a virtual method and non-virtual destructor")
{
protected:  
    TwoWire   &wire_;
    uint8_t     address_;
    uint16_t C[7];
    uint32_t callOrderTime_;
    uint8_t  callOrderCode_;

    uint32_t rawT_;
    uint32_t rawP_;
    
    int32_t  temperature_;
    uint32_t pressure_;
  
    uint32_t  getValue();
    void scaleValues();
    void orderValue(char code);
    bool pressureIsOk(uint32_t pressure);

  public:
    MS5611(TwoWire &wire);


    virtual void loop();
    virtual void begin();
    int32_t   getTemperature();
    uint32_t  getPressure();
    bool    isReady();
};



#endif //_MS5611_H_
