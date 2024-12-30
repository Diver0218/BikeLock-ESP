#include "iLock.h"
#include <Arduino.h>

class dummyLock : iLock {

    public:
        dummyLock(iMotor *lockMotor, iMotor *safetyMotor);
        ~dummyLock();
        void toggleLock() override;
        bool isLocked() override;
};