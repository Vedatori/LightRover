#include <Arduino.h>

#include "SemiIntelligentServo.h"

SemiIntelligentServo::SemiIntelligentServo(uint8_t aPwmPin, uint8_t aPwmChannel) {
    pwmPin = aPwmPin;
    pwmChannel = aPwmChannel;
    ledcSetup(pwmChannel, servoFreqPWM, servoResolPWM);
    ledcAttachPin(pwmPin, pwmChannel);
    speed = 600;            // default speed
    targetAchieved = true;  // at start stay in original position by not sending PWM
    currentPosition = 90;   // initial servo moving position
    targetPosition = 90;
    prevUpdateTime = 0;
}
void SemiIntelligentServo::setPWM(float aPosition) {
    if(aPosition < 0)
        aPosition = 0;
    else if(aPosition > 180)
        aPosition = 180;
    uint16_t dutyCycleServo = aPosition/180*(servoCountHigh - servoCountLow) + servoCountLow;
    ledcWrite(pwmChannel, dutyCycleServo);
}
void SemiIntelligentServo::setPosition(float aPosition) {
    if(aPosition < 0)
        aPosition = 0;
    else if(aPosition > 180)
        aPosition = 180;
    targetPosition = aPosition;
    targetAchieved = false;
}
float SemiIntelligentServo::getPosition() {
    return currentPosition;
}
bool SemiIntelligentServo::moving() {
    return !targetAchieved;
}
void SemiIntelligentServo::setSpeed(float aSpeed) {
    if(aSpeed < 0)
        aSpeed = 0;
    else if(aSpeed > 600)
        aSpeed = 600;
    speed = aSpeed;
}
void SemiIntelligentServo::updatePWM() {
    
    float step = speed*(millis() - prevUpdateTime)/1000.0;
    if(targetPosition > currentPosition + step)
        currentPosition = currentPosition + step;
    else if(targetPosition < currentPosition - step)
        currentPosition = currentPosition - step;
    else
        currentPosition = targetPosition;
    
    if(targetAchieved == false)
        setPWM(currentPosition);
    if(currentPosition == targetPosition)
        targetAchieved = true;
    

    prevUpdateTime = millis();
}