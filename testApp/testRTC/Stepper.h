/* created by JaschM
    BYJ48 Stepper motor code
   Connect :
   IN1 >> D8
   IN2 >> D9
   IN3 >> D10
   IN4 >> D11
   VCC ... 5V Prefer to use external 5V Source
*/
#ifndef STEPPER_H
#define STEPPER_H

#include "Arduino.h"

class Stepper {
  public:
    Stepper(const uint8_t stepPin, const uint8_t dirPin, const uint8_t delayStep) {
        _delayStep = delayStep;
        _stepPin = stepPin;
        _dirPin = dirPin;
    }
    
    void Init() {
        pinMode(_stepPin, OUTPUT);  
        pinMode(_dirPin, OUTPUT);  
    }
    
    void DoStep() {
      digitalWrite(_stepPin, LOW);
      digitalWrite(_stepPin, HIGH);
      delay(_delayStep);
    }
    
    void SetDirection(boolean direction) {
      digitalWrite(_dirPin, direction);
    }
    
  private:
    uint8_t _dirPin, _stepPin, _delayStep;
};

#endif
