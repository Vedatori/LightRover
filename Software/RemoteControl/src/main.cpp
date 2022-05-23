#include "LightRover/LightRover.h"

const uint8_t CONTROL_PERIOD = 50;
uint32_t prevControlTime = 0;

void setup() {
    LightRover.begin();
    LightRover.startWiFiCaptain("<your_name>");
}

void loop() {
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
        
        if(LightRover.commandGetIndexed(0) == "sonar") {
            LightRover.commandDisp(String(LightRover.sonarGetDistance()));
            LightRover.commandClear();
        }

        uint8_t joystickServoX = map(LightRover.controlGetX(), -100, 100, 0, 180);
        uint8_t joystickServoY = map(LightRover.controlGetY(), -100, 100, 0, 180);

        if(LightRover.sonarGetDistance() < 10 && joystickServoY > 90) {
            joystickServoY = 90;
        }

        LightRover.servoSetPosition(1, joystickServoX);
        LightRover.servoSetPosition(2, joystickServoY);

        //printf("Servo1: %f Servo2: %f Ultrasonic: %u\n", LightRover.servoGetPosition(1), LightRover.servoGetPosition(2), sonar.ping_cm());
    }
}
