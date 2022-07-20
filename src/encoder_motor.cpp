#include "encoder_motor.h"


void pwm_start(PinName pin, uint32_t clock_freq, uint32_t value, TimerCompareFormat_t resolution);


EncoderMotor::EncoderMotor(uint32_t enca,uint32_t encb, PinName pwm1, PinName pwm2){

    ENCA=enca;
    ENCB=encb;
    PWM1=pwm1;
    PWM2=pwm2;
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    pinMode(PWM1, OUTPUT);
    pinMode(PWM2, OUTPUT);
}

EncoderMotor::EncoderMotor(){

}


void EncoderMotor::go(){


    if(direction == 1){
        pwm_start(PWM1, motor_freq, PWM_value, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(PWM2, motor_freq, 0, RESOLUTION_12B_COMPARE_FORMAT);
    }
    else if(direction == -1){
        pwm_start(PWM1, motor_freq, 0, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(PWM2, motor_freq, PWM_value, RESOLUTION_12B_COMPARE_FORMAT);
    }
    else{
        pwm_start(PWM1, motor_freq, 0, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(PWM2, motor_freq, 0, RESOLUTION_12B_COMPARE_FORMAT);
    }


}

void EncoderMotor::stop(){

    pwm_start(PWM1, motor_freq, 0, RESOLUTION_12B_COMPARE_FORMAT);
    pwm_start(PWM2, motor_freq, 0, RESOLUTION_12B_COMPARE_FORMAT);

}


void EncoderMotor::read_encoder(){

    int b = digitalRead(ENCB);
    if(b > 0){
        position++;
    }
    else{
        position--;
    }    


}

void EncoderMotor::go_to_position(int pos){

    //pwm_start(PWM1, motor_freq, PWM_value, RESOLUTION_12B_COMPARE_FORMAT);

    if(pos>position){

        //check the direction irl
        set_direction(-1);
        while(position!=pos){
            read_encoder();
            go();
            delay(1);//do we need this delay?
        }
        delay(10);

    }
    else if(pos<position){

        //check the direction irl
        set_direction(1);
        while(position!=pos){ //why does position have to be negative
            read_encoder();
            go();
            delay(1);//do we need this delay?
        }
        delay(10);

    }
    else{
        stop();
    }

}

void EncoderMotor::turn(int degrees, int dir){
    int new_pos=0;

    int pos_diff = POSITIONS_PER_ROTATION*degrees/360;

    //check this irl
    if(dir==1){
        new_pos=position+pos_diff;
    }
    else if(dir==-1){
        new_pos=position-pos_diff;
    }

    go_to_position(new_pos);
}


void EncoderMotor::go_distance(float distance, int dir){ //in inches

    //distance = pi*diameter

    int angle = 360.0*distance/(PI*(float)WHEEL_DIAMETER);
    turn(angle, dir);
    
}

