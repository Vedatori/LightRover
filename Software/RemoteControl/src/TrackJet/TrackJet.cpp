#include "TrackJet.h"
#include "Preferences.h"

#include "WiFiCaptain.h"
#include "SemiIntelligentServo.h"

SemiIntelligentServo TJ::servo[] = {SemiIntelligentServo(TJ::SERVO[0], SERVO_CHANNEL[0]),
                                    SemiIntelligentServo(TJ::SERVO[1], SERVO_CHANNEL[1])};

void TJ::updatePWM(void * param) {
    for(;;) {
        TrackJet.checkConnection();

        for(uint8_t servoID = 0; servoID < SERVO_COUNT; ++servoID) {
            TJ::servo[servoID].updatePWM();
        }

        TrackJet.internCommandHandle();

        vTaskDelay(TJ::controlPeriod);
    }
}

TrackJetClass::TrackJetClass(void) {
    ;
}

void TrackJetClass::begin() {
    beginCalled = true;
    Serial.begin(115200);
    Serial.printf("Mac address: %s\n", WiFi.macAddress().c_str());

    xTaskCreatePinnedToCore(TJ::updatePWM, "updatePWM", 10000 , (void*) 0, 1, NULL, 1);
}

void TrackJetClass::servoSetPosition(uint8_t servoID, float position) {
    if(servoID >= 1 && servoID <= SERVO_COUNT)
        TJ::servo[servoID - 1].setPosition(position);
}
void TrackJetClass::servoSetSpeed(uint8_t servoID, float speed) {
    if(servoID >= 1 && servoID <= SERVO_COUNT)
        TJ::servo[servoID - 1].setSpeed(speed);
}
float TrackJetClass::servoGetPosition(uint8_t servoID) {
    if(servoID >= 1 && servoID <= SERVO_COUNT)
        return TJ::servo[servoID - 1].getPosition();
    else
        return 0;
}
bool TrackJetClass::servoMoving(uint8_t servoID) {
    if(servoID >= 1 && servoID <= SERVO_COUNT)
        return TJ::servo[servoID - 1].moving();
    else
        return false;
}
void TrackJetClass::startWiFiCaptain(String name, String password) {
    if(!beginCalled) {
        begin();
    }

    String ssid_final = "TrackJet-";
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

void TrackJetClass::checkConnection() {
    if(!connectionEnabled) {
        return;
    }
    if(millis() > prevCommunicationTime + TJ::communicationTimeout) {
        connectionActive = false;
    }
    else {
        connectionActive = true;
    }
}

int8_t TrackJetClass::controlGetX() {
    return getControl().joystickX;
}

int8_t TrackJetClass::controlGetY() {
    return getControl().joystickY;
}

String TrackJetClass::commandGet() {
    String command = String(commandGetCaptain());
    command.toLowerCase();
    return command;
}

String TrackJetClass::commandGetIndexed(uint8_t index) {
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

void TrackJetClass::commandClear() {
    commandClearCaptain();
}

void TrackJetClass::internCommandHandle() {
    static uint8_t counter = 0;
    if(counter < 20) {
        counter++;
        return;
    }
    else {
        counter = 0;
    }
    if(TrackJet.commandGetIndexed(0) == "reset" || TrackJet.commandGetIndexed(0) == "restart") {
        ESP.restart();
    }
}

void TrackJetClass::commandSend(String type, String text) {
    commandSendCaptain(type, text);
}
void TrackJetClass::commandDisp(String text) {
    commandSend("commandDisp", text);
}

TrackJetClass TrackJet;
