#include "iMotor.h"
#include <Arduino.h>

class dummyMotor : iMotor {
    public:
        dummyMotor();
        ~dummyMotor();
        void rotate(int degree) override;
};