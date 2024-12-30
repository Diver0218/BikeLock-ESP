#include "iMotor.h"

class iLock {

    private:
        iMotor *lockMotor;
        iMotor *safetyMotor;

    public:
        iLock(iMotor *lockMotor, iMotor *safetyMotor);
        ~iLock();
        virtual void toggleLock() = 0;

};
