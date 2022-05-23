#pragma once

#include <Arduino.h>
#include "Preferences.h"

#include "WiFiCaptain.h"
#include "SemiIntelligentServo.h"

namespace LR {

#define SERVO_COUNT 2
const uint8_t SERVO[SERVO_COUNT] = {18, 19};
const uint8_t SERVO_CHANNEL[SERVO_COUNT] = {0, 1};

const uint8_t LED = 2, SONAR_TRIG = 5, SONAR_ECHO = 17;

const uint16_t encThresholdInit = 1600;

const uint8_t FREQ_PWM_THRESHOLD = 40;
const uint16_t controlPeriod = 10;  // [ms]
const char STORAGE_NAMESPACE[] = "LightRover";
const uint16_t communicationTimeout = 1000;

extern SemiIntelligentServo servo[SERVO_COUNT];
void updatePWM(void * param);
}

class LightRoverClass {
    bool beginCalled = false;
    Preferences preferences;
    uint32_t prevCommunicationTime = 0;
    bool connectionEnabled = false;
    bool connectionActive = false;

public:
    LightRoverClass();
    void begin();

    void ledSet(bool state);

    void servoSetPosition(uint8_t servoID, float position);     // servoID 1, 2, 3; position 0-180 [°]
    void servoSetSpeed(uint8_t servoID, float speed);    // speed 0-600 [°/s]
    float servoGetPosition(uint8_t servoID);
    bool servoMoving(uint8_t servoID);

    unsigned long sonarGetDistance();

    void startWiFiCaptain(String name="", String password="");
    void checkConnection();
    int8_t controlGetX();
    int8_t controlGetY();
    String commandGet();
    String commandGetIndexed(uint8_t index);
    void commandClear();
    void internCommandHandle();
    void commandSend(String type, String text);
    void commandDisp(String text);

    void diagnosticPrint();
};

extern LightRoverClass LightRover;
