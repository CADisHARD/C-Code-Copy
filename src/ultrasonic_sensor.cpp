#include "ultrasonic_sensor.h"
#include "stdint.h"
#include "Arduino.h"
#include "encoder_motor.h"

UltrasonicSensor::UltrasonicSensor(uint32_t trigger,uint32_t echo){

    TRIGGER=trigger;
    ECHO=echo;
    pinMode(TRIGGER, OUTPUT);
    pinMode(TRIGGER, INPUT);

}

int UltrasonicSensor::routine(){

    int is_detected=0;

    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);

    digitalWrite(ECHO, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);

    long duration = pulseIn(ECHO, HIGH);

    if(unit=="cm"){
        distance = duration * SOS_CM / 2.0;
    }
    else if(unit=="mm"){
        distance = duration * SOS_MM / 2.0;
    }
    else if(unit=="in"){
        distance = duration * SOS_IN / 2.0;
    }

    if(type=="edge"){

        if(distance>=threshold){
            is_detected=1;
        }
        else{
            is_detected=0;
        }
    }
    else if(type=="treasure"){

        if(distance<=threshold){
            is_detected=1;
        }
        else{
            is_detected=0;
        }
    }

    
    return is_detected;
    
}


/**********Edge Detection*********/

const char *edge_detection(int thresh, const char *unit){

    int left=0;
    int right=0;
    const char *message="blank edge detection message";

    uint32_t TRIGGER_L=PA9;
    uint32_t ECHO_L=PA10;
    uint32_t TRIGGER_R=PA8;
    uint32_t ECHO_R=PA7;
    UltrasonicSensor left_sensor(TRIGGER_L, ECHO_L);
    UltrasonicSensor right_sensor(TRIGGER_R, ECHO_R);

    left_sensor.set_type("edge");
    left_sensor.set_unit(unit);
    left_sensor.set_threshold(thresh);

    right_sensor.set_type("edge");
    right_sensor.set_unit(unit);
    right_sensor.set_threshold(thresh);

    left=left_sensor.routine();
    right=right_sensor.routine();

    if(left==1){

        message="edge detected!";
        //turn_right();
    }    
    else if(right==1){
        message="edge detected!";
        //turn_left();
    }
    else{
        message="nothing detected!";
    }

    return message;

}