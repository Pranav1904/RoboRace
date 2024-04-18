#pragma once

#include <Arduino.h>

class CytronMD {

    int dirPin, pwmPin;
    public:
    
    float currentPower;

    void initialize(int dirPin, int pwmPin);
    void setPower(float val);
    void driveRPM();
};
