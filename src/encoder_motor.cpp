#include "encoder_motor.h"
#include <Adafruit_SSD1306.h>


void pwm_start(PinName pin, uint32_t clock_freq, uint32_t value, TimerCompareFormat_t resolution);


EncoderMotor::EncoderMotor(uint32_t enca,uint32_t encb, PinName pwm1, PinName pwm2, int degrees_per_click, int enc_offset){

    ENCA=enca;
    ENCB=encb;
    PWM1=pwm1;
    PWM2=pwm2;
    DEGREES_PER_CLICK=degrees_per_click;
    ENCODER_OFFSET=enc_offset;
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


void EncoderMotor::count_positions(){

    if(digitalRead(ENCA)==HIGH&&digitalRead(ENCB)==HIGH){
        position++;
    }

}



void EncoderMotor::go_to_position(int pos, Adafruit_SSD1306 display){

    if(pos-ENCODER_OFFSET<position&&position<pos+ENCODER_OFFSET){
        stop();
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(position);
        display.display();
    }

    else if(pos>position){

        set_direction(1);
        while(position<pos){
            display.clearDisplay();
            display.setCursor(0,0);
            display.println(get_position());
            display.display();
            go();
            delayMicroseconds(5);
        }
        stop();
        delay(100);

    }
    else if(pos-ENCODER_OFFSET<get_position()&&get_position()<pos+ENCODER_OFFSET){
       stop();
    }
    else if(pos<get_position()){

        //check the direction irl
        set_direction(-1);
        while(get_position()>pos){
            display.clearDisplay();
            display.setCursor(0,0);
            display.println(get_position());
            display.display();
            go();
            delayMicroseconds(5);//do we need this delay?
        }
        stop();
        delay(100);

    }
    else if(pos-ENCODER_OFFSET<get_position()&&get_position()<pos+ENCODER_OFFSET){
       stop();
       display.clearDisplay();
            display.setCursor(0,0);
       display.println(get_position());
        display.display();
    }


}

/*void EncoderMotor::turn(int degrees, int dir){
    
    int new_pos=0;
    int pos_diff = degrees/DEGREES_PER_CLICK;

    //left (aka cw) is positive position for yellow motors convention: cw is negative ccw is positive
    if(dir==-1){
        new_pos=position+pos_diff;
    }
    else if(dir==1){
        new_pos=position-pos_diff;
    }
    go_to_position(new_pos, display);
}*/

void EncoderMotor::turn_positions(int desired_position, int dir, Adafruit_SSD1306 display){

    if(dir==-1){ //left aka cw

        //double check this in real life or switch pwm pins
        set_direction(-1);
    }
    else if(dir==1){ //right aka ccw
        set_direction(1);
    }

    while(position<=desired_position){
        go();
        count_positions();
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(position);
        display.display();
    }

    stop();

}


void EncoderMotor::go_distance(float distance, int dir){ //in inches

    //distance = pi*diameter

    int angle = 360.0*distance/(PI*(float)WHEEL_DIAMETER);
    turn(angle, dir);
    
}


void EncoderMotor::claw_go(float distance, int dir){

    int angle = 360.0*distance/(PI*(float)CLAW_GEAR_DIAMETER);
    turn(angle, dir);

}