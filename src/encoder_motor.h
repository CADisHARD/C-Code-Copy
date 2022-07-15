#ifndef DRIVING_MOTORS_H
#define DRIVING_MOTORS_H

#include "Arduino.h"

class EncoderMotor{

    private:

        uint32_t ENCA;
        uint32_t ENCB;

        PinName PWM1;
        PinName PWM2;
        
        int motor_freq=1000;
        
        int position = 0;
        int PWM_value=2000;
        int direction = 1;

        
        
        

    public:

        EncoderMotor();
    
        EncoderMotor(uint32_t ENCA,uint32_t ENCB, PinName PWM1, PinName PWM2);
        int get_position(){return position;}
        int get_pwm(){return PWM_value;}
        int get_direction(){return direction;}
        void set_position(int pos){position=pos;}
        void set_pwm(int pwm){PWM_value=pwm;}
        void set_direction(int dir){direction=dir;}
        void setMotor();
        void go();
        void stop();
        void read_encoder();
        void go_to_position(int pos);      
};



#endif