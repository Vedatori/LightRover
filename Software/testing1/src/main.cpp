#include <Arduino.h>

const uint8_t servoOut0 = 18, servoOut1 = 19, trigPin = 5, echoPin = 17, irPin = 13;
const uint16_t freqServo = 50;
const uint8_t channel0 = 0, channel1 = 1;
const uint8_t resolutionServo = 16;
const uint16_t countLow = 1634, countHigh = 8187;
const float minServo = 0, maxServo = 180, stepServo = 18;

uint16_t loopDelay = 100;
float duration_us, distance_cm;

void setup() {
    Serial.begin(115200);
    ledcSetup(channel0, freqServo, resolutionServo);
    ledcSetup(channel1, freqServo, resolutionServo);
    ledcAttachPin(servoOut0, channel0);
    ledcAttachPin(servoOut1, channel1);

    pinMode(trigPin, OUTPUT);
}

void loop() {
    static float positionServo = minServo;
    static bool directionServo = 0;
    if(directionServo) {
        if(positionServo < maxServo) {
        positionServo += stepServo;
        }
        else {
        directionServo = !directionServo;
        }
    }
    else {
        if(positionServo > minServo) {
        positionServo -= stepServo;
        }
        else {
        directionServo = !directionServo;
        }
    }
    if(positionServo > maxServo) {
        positionServo = maxServo;
    }
    else if(positionServo < minServo) {
        positionServo = minServo;
    }

    uint16_t dutyCycleServo = positionServo/maxServo*(countHigh - countLow) + countLow;
    ledcWrite(channel0, dutyCycleServo);
    ledcWrite(channel1, dutyCycleServo);

    // generate 10-microsecond pulse to TRIG pin
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // measure duration of pulse from ECHO pin
    duration_us = pulseIn(echoPin, HIGH);

    // calculate the distance
    distance_cm = 0.017 * duration_us;

    printf("%f %f %d\n", positionServo, distance_cm, digitalRead(irPin));

    delay(loopDelay);
}