#include "dummyMotor.h"

dummyMotor::dummyMotor() {
    Serial.println("dummyMotor constructor");
}

dummyMotor::~dummyMotor() {
    Serial.println("dummyMotor destructor");
}

void dummyMotor::rotate(int steps) {
    Serial.println("dummyMotor rotate; degrees: " + String(steps));
}