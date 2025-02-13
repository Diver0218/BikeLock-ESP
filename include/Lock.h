#include "iLock.h"
#include <Arduino.h>

class Lock : public iLock {

    private:
        int steps_to_lock;

    public:
        Lock(iMotor *lockMotor, iMotor *safetyMotor, int steps_to_lock);
        void toggleLock() override;
        bool isLocked() override;
};