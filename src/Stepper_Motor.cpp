#include "Stepper_Motor.h"

Stepper_Motor::Stepper_Motor(int stepperResolution, int pin1, int pin2, int pin3, int pin4)
{
    stepper = new Stepper(stepperResolution, pin1, pin2, pin3, pin4);
    stepper->setSpeed(60);
}

void Stepper_Motor::rotate(int steps)
{
    stepper->step(steps);
}
