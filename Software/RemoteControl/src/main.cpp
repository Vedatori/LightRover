#include "TrackJet/TrackJet.h"
#include "NewPing.h"

const uint8_t CONTROL_PERIOD = 50;
uint32_t prevControlTime = 0, prevBlinkTime = 0;
NewPing sonar(5, 17);
uint8_t onboardLED = 2;

void setup() {
    TrackJet.begin();
    TrackJet.startWiFiCaptain("<your_name>");
    pinMode(onboardLED, OUTPUT);
}

void loop() {
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
        
        if(TrackJet.commandGetIndexed(0) == "ahoj") {
            TrackJet.commandDisp("nazdar");
            TrackJet.commandClear();
        }

        uint8_t joystickServoX = map(TrackJet.controlGetX(), -100, 100, 0, 180);
        uint8_t joystickServoY = map(TrackJet.controlGetY(), -100, 100, 0, 180);

        if(sonar.ping_cm() < 10 && joystickServoY > 0) {
            joystickServoY = 105;
        }

        TrackJet.servoSetPosition(1, joystickServoX);
        TrackJet.servoSetPosition(2, joystickServoY);

        //printf("Servo1: %f Servo2: %f Ultrasonic: %u\n", TrackJet.servoGetPosition(1), TrackJet.servoGetPosition(2), sonar.ping_cm());
    }
    if(millis() > prevBlinkTime + 1000) {
        prevBlinkTime = millis();

        static bool light = false;
        light = !light;
        digitalWrite(onboardLED, light);
        //printf("switched %d\n", light);
    }
}
