#include "iMotor.h"

class iLock {

    protected:
        iMotor *lockMotor;
        iMotor *safetyMotor;
        bool locked;

    public:
        virtual void toggleLock() = 0;
        virtual bool isLocked() = 0;

};
