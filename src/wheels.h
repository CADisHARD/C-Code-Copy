#ifndef WHEELS_H
#define WHEELS_H


#include "Arduino.h"
#include "encoder_motor.h"


class Wheels{


    private:

        EncoderMotor left_motor;
        EncoderMotor right_motor;

    public:

        Wheels(EncoderMotor lmot, EncoderMotor rmot){left_motor=lmot;right_motor=rmot;}

        void steer_left(int amount); //for now, "amount" is the velocity decrease on the wheel about which we are turning.
        void steer_right(int amount); //This is only for slight steering
        void stop(); 
        void go();


};



#endif