#ifndef _TONE_H_
#define _TONE_H_
#include <Arduino.h>

#define NOTONEAC 0

#if defined (__AVR_ATmega32U4__) || defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
  #define PWMT1AMASK DDB5
  #define PWMT1BMASK DDB6
  #define PWMT1DREG DDRB
  #define PWMT1PORT PORTB
#elif defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__)
  #define PWMT1AMASK DDD4
  #define PWMT1BMASK DDD5
  #define PWMT1DREG DDRD
  #define PWMT1PORT PORTD
#else
  #define PWMT1AMASK DDB1
  #define PWMT1BMASK DDB2
  #define PWMT1DREG DDRB
  #define PWMT1PORT PORTB
#endif



class Tone {
  
  int8_t volume_;

  public:
    Tone(int8_t volume = NOTONEAC);
    void beep(uint32_t frequency = NOTONEAC);
    void setVolume(int8_t volume);
    int8_t getVolume();

};


#endif //_TONE_H_

