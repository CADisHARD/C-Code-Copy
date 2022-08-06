#ifndef ENCODER_MOTOR_H
#define ENCODER_MOTOR_H

#include "Arduino.h"
#include <Adafruit_SSD1306.h>

class EncoderMotor{

    private:

        uint32_t ENCA;
        uint32_t ENCB;

        PinName PWM1;
        PinName PWM2;
        
        int motor_freq=1000;
        
        int PWM_value=3500;
        int direction = 1;

        int CLAW_GEAR_DIAMETER = 5;
        
        

    public:

        EncoderMotor();
        volatile int position = 0;
        EncoderMotor(uint32_t ENCA,uint32_t ENCB, PinName PWM1, PinName PWM2);
        int get_position(){return position;}
        int get_pwm(){return PWM_value;}
        int get_direction(){return direction;}
        void set_position(int pos){position=pos;}
        void set_pwm(int pwm){PWM_value=pwm;}
        void set_direction(int dir){direction=dir;}
        void go();
        void stop();
        
};




#endif
