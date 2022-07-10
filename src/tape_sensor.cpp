#include "tape_sensor.h"
#include "driving_motor.h"


TapeSensors::TapeSensors(int thresh){
    set_threshold(thresh);
    pinMode(REFLECTANCE_L, INPUT);
    pinMode(REFLECTANCE_M, INPUT);
    pinMode(REFLECTANCE_R, INPUT);
}

void TapeSensors::set_threshold(int thresh){

    threshold=thresh;
}

const char *TapeSensors::routine(){

    const char *message="All sensors are on the tape.";
    
    L_val=analogRead(REFLECTANCE_L);
    M_val =analogRead(REFLECTANCE_M);
    R_val=analogRead(REFLECTANCE_R);

    if(L_val<=threshold){
        message="Left sensor is off the tape.";
        turn_right(); 
    }
    else if(R_val<=threshold){
        message="Right sensor is off the tape.";
        turn_left();
    }
    else if(M_val<=threshold){
        message="Middle sensor is off the tape.";
        stop();
    }

    return message;
}







