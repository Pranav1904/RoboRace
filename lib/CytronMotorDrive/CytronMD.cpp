#include <CytronMD.h>

void CytronMD::initialize(int dirPin, int pwmPin) {
    this->dirPin = dirPin;
    this->pwmPin = pwmPin;
    this->currentPower = 0.0;
    
    pinMode(dirPin, OUTPUT);
    pinMode(pwmPin, OUTPUT);
    analogWrite(pwmPin, 0);
}

void CytronMD::setPower(float val) {

    if (val > 0) {
      digitalWrite(this->dirPin, HIGH);
    } else {
      digitalWrite(dirPin, LOW);
    }

    analogWrite(pwmPin, abs(val) * 255);

    this->currentPower = val;

}
void CytronMD::driveRPM() {
    uint32_t currentMicros = micros();

    if (currentMicros - lastRPMDriveMicros < 1000) { return; }

    float controllerOutput = this->controller.calculateOutput(
        this->targetRPM,
        this->currentRPM,
        this->lastPollMicrosDelta
    );
    // Serial.print(controllerOutput);
    // Serial.print("\t");
    controllerOutput /= this->maxRPM;
    this->lastRPMDriveMicros = currentMicros;
    this->setPower(max(-1.0f, min(1.0f,this->currentPower + controllerOutput)));
}
