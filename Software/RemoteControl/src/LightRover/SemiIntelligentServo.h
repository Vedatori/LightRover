#ifndef _SEMI_INTELLIGENT_SERVO_H_
#define _SEMI_INTELLIGENT_SERVO_H_

#include <Arduino.h>

class SemiIntelligentServo {
    static const uint8_t servoFreqPWM = 50;
    static const uint8_t servoResolPWM = 16;
    static const uint16_t servoCountLow = 2700; //1707
    static const uint16_t servoCountHigh = 8187;
    uint8_t pwmPin;
    uint8_t pwmChannel;
    float currentPosition;
    float targetPosition;
    float speed;
    bool targetAchieved;
    uint32_t prevUpdateTime;

public:
    SemiIntelligentServo(uint8_t aPwmPin, uint8_t aPwmChannel);
    void setPWM(float aPosition);
    void setPosition(float aPosition);
    float getPosition();
    bool moving();
    void setSpeed(float aSpeed);
    void updatePWM();
};

#endif  // _SEMI_INTELLIGENT_SERVO_H_