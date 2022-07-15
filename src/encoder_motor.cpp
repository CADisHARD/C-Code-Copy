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

    digitalWrite(PWM1,LOW);
    digitalWrite(PWM2,LOW);

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




}

