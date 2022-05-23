#include "LightRover.h"
#include "Preferences.h"
#include "NewPing.h"

#include "WiFiCaptain.h"
#include "SemiIntelligentServo.h"

SemiIntelligentServo LR::servo[] = {SemiIntelligentServo(LR::SERVO[0], SERVO_CHANNEL[0]),
                                    SemiIntelligentServo(LR::SERVO[1], SERVO_CHANNEL[1])};

NewPing sonar(LR::SONAR_TRIG, LR::SONAR_ECHO);

void LR::updatePWM(void * param) {
    for(;;) {
        LightRover.checkConnection();

        for(uint8_t servoID = 0; servoID < SERVO_COUNT; ++servoID) {
            LR::servo[servoID].updatePWM();
        }

        LightRover.internCommandHandle();

        vTaskDelay(LR::controlPeriod);
    }
}

LightRoverClass::LightRoverClass(void) {
    ;
}

void LightRoverClass::begin() {
    beginCalled = true;
    Serial.begin(115200);
    Serial.printf("Mac address: %s\n", WiFi.macAddress().c_str());

    pinMode(LR::LED, OUTPUT);

    xTaskCreatePinnedToCore(LR::updatePWM, "updatePWM", 10000 , (void*) 0, 1, NULL, 1);
}

void LightRoverClass::ledSet(bool state) {
    digitalWrite(LR::LED, state);
}

void LightRoverClass::servoSetPosition(uint8_t servoID, float position) {
    if(servoID >= 1 && servoID <= SERVO_COUNT)
        LR::servo[servoID - 1].setPosition(position);
}
void LightRoverClass::servoSetSpeed(uint8_t servoID, float speed) {
    if(servoID >= 1 && servoID <= SERVO_COUNT)
        LR::servo[servoID - 1].setSpeed(speed);
}
float LightRoverClass::servoGetPosition(uint8_t servoID) {
    if(servoID >= 1 && servoID <= SERVO_COUNT)
        return LR::servo[servoID - 1].getPosition();
    else
        return 0;
}
bool LightRoverClass::servoMoving(uint8_t servoID) {
    if(servoID >= 1 && servoID <= SERVO_COUNT)
        return LR::servo[servoID - 1].moving();
    else
        return false;
}
unsigned long LightRoverClass::sonarGetDistance() {
    return sonar.ping_cm();
}
void LightRoverClass::startWiFiCaptain(String name, String password) {
    if(!beginCalled) {
        begin();
    }

    String ssid_final = "LightRover-";
    if(name.isEmpty() || name == "<your_name>") {
        ssid_final += WiFi.macAddress();
    }
    else {
        ssid_final += name;
    }
    setApCredentials(ssid_final, password);
    wifiCaptInit();
    connectionEnabled = true;
}

void LightRoverClass::checkConnection() {
    if(!connectionEnabled) {
        return;
    }
    if(millis() > prevCommunicationTime + LR::communicationTimeout) {
        connectionActive = false;
    }
    else {
        connectionActive = true;
    }
}

int8_t LightRoverClass::controlGetX() {
    return getControl().joystickX;
}

int8_t LightRoverClass::controlGetY() {
    return getControl().joystickY;
}

String LightRoverClass::commandGet() {
    String command = String(commandGetCaptain());
    command.toLowerCase();
    return command;
}

String LightRoverClass::commandGetIndexed(uint8_t index) {
    char commandBuffer[64];
    sprintf(commandBuffer, commandGetCaptain());
    const char delimiter[2] = " ";
    char *token;
    token = strtok((char *)commandBuffer, delimiter);
    for(uint8_t i = 0; i < index; ++i) {
        token = strtok(NULL, delimiter);
    }
    String command = String(token);
    command.toLowerCase();
    return command;
}

void LightRoverClass::commandClear() {
    commandClearCaptain();
}

void LightRoverClass::internCommandHandle() {
    static uint8_t counter = 0;
    if(counter < 20) {
        counter++;
        return;
    }
    else {
        counter = 0;
    }
    if(LightRover.commandGetIndexed(0) == "reset" || LightRover.commandGetIndexed(0) == "restart") {
        ESP.restart();
    }
}

void LightRoverClass::commandSend(String type, String text) {
    commandSendCaptain(type, text);
}
void LightRoverClass::commandDisp(String text) {
    commandSend("commandDisp", text);
}

LightRoverClass LightRover;
