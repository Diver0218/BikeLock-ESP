#include <freertos/FreeRTOS.h> // FreeRTOS muss zuerst eingebunden werden
#include "Lock.h"

Lock::Lock(iMotor *lockMotor, iMotor *safetyMotor, int steps_to_lock){
    this->lockMotor = lockMotor;
    this->safetyMotor = safetyMotor;
    locked = isLocked_global;
    this->steps_to_lock = steps_to_lock;
}

void Lock::toggleLock(){
    if (locked) {
        safetyMotor->rotate(-1 * steps_to_lock);
        delay(500);
        locked = false;
        isLocked_global = false;
        lockMotor->rotate(-1 * steps_to_lock);
        Serial.println("Unlocked");
    } else {
        lockMotor->rotate(steps_to_lock);
        delay(500);
        locked = true;
        isLocked_global = true;
        Serial.println("Locked");
    }
}

bool Lock::isLocked(){
    Serial.println("isLocked: " + String(locked));
    return locked;
}