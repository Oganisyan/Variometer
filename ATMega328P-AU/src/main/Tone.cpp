// ---------------------------------------------------------------------------
// Created by Tim Eckel - teckel@leethost.com
// Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// ---------------------------------------------------------------------------

#include "Tone.h"


uint8_t _tAC_volume[] = { 255, 223, 191, 159, 127, 63, 31, 15, 4, 2 }; // Duty for linear volume control.




Tone::Tone(int8_t volume) : volume_(volume) {
  
}

void Tone::beep(uint32_t frequency){
  if(abs(this->frequency - frequency) < 80)
    return;
  this->frequency = frequency;  
  if (frequency == NOTONEAC || volume_ == 0) { 
    TCCR1B  = _BV(CS11);                              // Default clock prescaler of 8.
    TCCR1A  = _BV(WGM10);                             // Set to defaults so PWM can work like normal (PWM, phase corrected, 8bit).
    PWMT1PORT &= ~_BV(PWMT1AMASK);                    // Set timer 1 PWM pins to LOW.
  } else {
    PWMT1DREG |= _BV(PWMT1AMASK);                     // Set timer 1 PWM pins to OUTPUT (because analogWrite does it too).
    uint8_t prescaler = _BV(CS10);                    // Try using prescaler 1 first.
    uint32_t top = F_CPU / frequency / 2 - 1;    // Calculate the top.
    if (top > 65535) {                                // If not in the range for prescaler 1, use prescaler 256 (122 Hz and lower @ 16 MHz).
      prescaler = _BV(CS12);                          // Set the 256 prescaler bit.
      top = top / 256 - 1;                            // Calculate the top using prescaler 256.
    }
  
    ICR1   = top;                                      // Set the top.
    if (TCNT1 > top) TCNT1 = top;                      // Counter over the top, put within range.
    TCCR1B = _BV(WGM13)  | prescaler;                  // Set PWM, phase and frequency corrected (top=ICR1) and prescaler.
    OCR1A  = OCR1B = top / _tAC_volume[volume_ - 1];   // Calculate & set the duty cycle (volume).
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(COM1B0);  // Inverted/non-inverted mode (AC).
  }
}

void Tone::setVolume(int8_t volume) {
  volume_ = constrain(volume, 0, 10);
}

int8_t Tone::getVolume() {
  return volume_;
}
