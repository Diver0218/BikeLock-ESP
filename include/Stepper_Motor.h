#include <Stepper.h>

#include "iMotor.h"

class Stepper_Motor : public iMotor {

    private:
        Stepper *stepper = nullptr;

    public:
        Stepper_Motor(int stepperResolution, int pin1, int pin2, int pin3, int pin4);
        void rotate(int steps) override;
};