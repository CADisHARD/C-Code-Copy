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

        

        int DEGREES_PER_CLICK = 4.5;   
        int WHEEL_DIAMETER = 2.6;     

        //int TABLE_PER_ROTATION = 4;
        int CLAW_GEAR_DIAMETER = 5;
        int ENCODER_OFFSET=0;

        
        

    public:

        EncoderMotor();
        int position = 0;
    
        EncoderMotor(uint32_t ENCA,uint32_t ENCB, PinName PWM1, PinName PWM2, int degrees_per_click, int enc_offset);
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
        void turn_positions(int desired_position, int dir, Adafruit_SSD1306 display);
        void turn(int degrees, int dir);
        void go_distance(float distance, int dir);
        void go_to_position(int pos, Adafruit_SSD1306 display); 
        void claw_go(float distance, int dir);
        void count_positions();
        
};




#endif
