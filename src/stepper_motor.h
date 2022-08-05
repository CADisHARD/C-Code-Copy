#ifndef STEPPER_MOTOR
#define STEPPER_MOTOR


#include "Arduino.h"
#include "Wire.h"

class StepperMotor{
    private:
    uint32_t STP = PA10;
    uint32_t DIR = PA11;
    uint32_t EN = PA12;
    uint32_t MS1 = PA15;
    uint32_t MS2 = PB3;

    public:
    
    StepperMotor(uint32_t stp,uint32_t dir,uint32_t en,uint32_t ms1,uint32_t ms2);
    void rise();
    void rise(int steps);
    void descend();
    void descend(int steps);
    void resetEDPins();



};



#endif