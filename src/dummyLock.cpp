#include "dummyLock.h"

dummyLock::dummyLock(iMotor *lockMotor, iMotor *safetyMotor){
    this->lockMotor = lockMotor;
    this->safetyMotor = safetyMotor;
    locked = false;
}

dummyLock::~dummyLock(){
    delete this->lockMotor;
    delete this->safetyMotor;
}

void dummyLock::toggleLock(){
    if (locked) {
        safetyMotor->rotate(-180);
        delay(500);
        lockMotor->rotate(-90);
        delay(500);
        locked = false;
        Serial.println("Unlocked");
    } else {
        lockMotor->rotate(90);
        delay(500);
        safetyMotor->rotate(180);
        delay(500);
        locked = true;
        Serial.println("Locked");
    }
}

bool dummyLock::isLocked(){
    Serial.println("isLocked: " + String(locked));
    return locked;
}