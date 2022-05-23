// Include LightRover library source code
#include "LightRover/LightRover.h"

// Create numeric variables
int CONTROL_PERIOD = 50;
int prevControlTime = 0;

// Executed once at start
void setup() {
    // Initiate LightRover library and enable remote control
    LightRover.begin();
    LightRover.startWiFiCaptain("<your_name>");
}

// Executed repeatedly 
void loop() {
    // Control robot in defined low frequency
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
        
        // Return measured sonar distance after "sonar" command
        if(LightRover.commandGetIndexed(0) == "sonar") {
            LightRover.commandDisp(String(LightRover.sonarGetDistance()));
            LightRover.commandClear();
        }

        // Get and rescale joystick position
        int joystickServoX = map(LightRover.controlGetX(), -100, 100, 0, 180);
        int joystickServoY = map(LightRover.controlGetY(), -100, 100, 0, 180);

        // Stop if obstacle detected
        if(LightRover.sonarGetDistance() < 10 && joystickServoY > 90) {
            joystickServoY = 90;
        }

        // Control servos
        LightRover.servoSetPosition(1, joystickServoX);
        LightRover.servoSetPosition(2, joystickServoY);
    }
}
