#include "TrackJet/TrackJet.h"

const uint8_t CONTROL_PERIOD = 100;
uint32_t prevControlTime = 0;

void setup() {
    TrackJet.begin();
    TrackJet.startWiFiCaptain("<your_name>");
    TrackJet.servoSetSpeed(1, 60);
}

void loop() {
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
        
        if(TrackJet.commandGetIndexed(0) == "ahoj") {
            TrackJet.commandDisp("nazdar");
            TrackJet.commandClear();
        }
        printf("Servo1: %f Servo2: %f", TrackJet.servoGetPosition(1), TrackJet.servoGetPosition(2));
    }
}
