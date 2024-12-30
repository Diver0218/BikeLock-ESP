#pragma once

#include "iMotor.h"

class iLock {

    protected:
        iMotor *lockMotor;
        iMotor *safetyMotor;
        bool locked;

    public:
        virtual bool isLocked() = 0;
        virtual void toggleLock() = 0;

};