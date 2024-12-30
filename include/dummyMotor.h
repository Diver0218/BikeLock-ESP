#include "iMotor.h"
#include <Arduino.h>

class dummyMotor : public iMotor {
    public:
        dummyMotor();
        ~dummyMotor();
        void rotate(int degree) override;
};