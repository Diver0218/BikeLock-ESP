#include "dummyMotor.h"

dummyMotor::dummyMotor() {
    Serial.println("dummyMotor constructor");
}

dummyMotor::~dummyMotor() {
    Serial.println("dummyMotor destructor");
}

void dummyMotor::rotate(int degree) {
    Serial.println("dummyMotor rotate; degrees: " + String(degree));
}