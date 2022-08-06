#include "tape_sensor.h"


TapeSensors::TapeSensors(){
    pinMode(REFLECTANCE_L, INPUT);
    pinMode(REFLECTANCE_M, INPUT);
    pinMode(REFLECTANCE_R, INPUT);
}


void TapeSensors::initial_reading(){

    int L=analogRead(REFLECTANCE_L);
    int M=analogRead(REFLECTANCE_M);
    int R=analogRead(REFLECTANCE_R);


    previous_sensor_values[0]=get_norm_L_val();
    previous_sensor_values[1]=get_norm_M_val();
    previous_sensor_values[2]=get_norm_R_val();

    get_error();


}


void TapeSensors::read_tape(){

    raw_L_val=analogRead(REFLECTANCE_L);
    raw_M_val =analogRead(REFLECTANCE_M);
    raw_R_val=analogRead(REFLECTANCE_R);

    current_sensor_values[0]=get_norm_L_val();
    current_sensor_values[1]=get_norm_M_val();
    current_sensor_values[2]=get_norm_R_val();

    if

    get_error();

}


const char *TapeSensors::get_error(){

    const char *message="blank";

    int prev_L=previous_sensor_values[0];
    int prev_M=previous_sensor_values[1];
    int prev_R=previous_sensor_values[2];
    
    int curr_L=current_sensor_values[0];
    int curr_M=current_sensor_values[1];
    int curr_R=current_sensor_values[2];


    if(curr_L==0 && curr_M==1 && curr_R==0){
        message="on target";
        prev_message_000="whatever";
        position = 3;
        error=IDEAL_POSITION-position;
    }
    else if(curr_L==1 && curr_M==1 && curr_R==1){
        message="on target";
        prev_message_000="whatever";
        position = 3;
        error=IDEAL_POSITION-position;
    }
    
    else if(curr_L==1 && curr_M==1 && curr_R==0){
        message="slight right";
        prev_message_000="whatever";
        position = 2;
        error=IDEAL_POSITION-position;
    }
    else if(curr_L==1 && curr_M==0 && curr_R==0){
        message="medium right";
        prev_message_000="whatever";
        position = 1;
        error=IDEAL_POSITION-position;
    }
    else if(curr_L==0 && curr_M==1 && curr_R==1){
        message="slight left";
        prev_message_000="whatever";
        position = 4;
        error=IDEAL_POSITION-position;

    }
    else if(curr_L==0 && curr_M==0 && curr_R==1){
        message="medium left";
        prev_message_000="whatever";
        position = 5;
        error=IDEAL_POSITION-position;
    }

    

    else if(curr_L==0 && curr_M==0 && curr_R==0){

        if(prev_L==1){
            message="big right";
            prev_message_000=message;
            position = 0;
            last_position=position;
            error=IDEAL_POSITION-position;
            
        }
        else if(prev_R==1){
            message="big left";
            prev_message_000=message;
            position = 6;
            last_position=position;
            error=IDEAL_POSITION-position;
        }
        else if(curr_L==0 && curr_M==0 && curr_R==0){
            message=prev_message_000;
            if(last_position==0){
                position = 0;
                error=IDEAL_POSITION-position;
            }
            else if(last_position==6){
                position = 6;
                error=IDEAL_POSITION-position;
            }
        }

    }


    previous_sensor_values[0]=current_sensor_values[0];
    previous_sensor_values[1]=current_sensor_values[1];
    previous_sensor_values[2]=current_sensor_values[2];


    return message;
}


int TapeSensors::get_norm_L_val(){
   
   /*if(raw_L_val<50){
    normalized_L_val=0; //off the tape
   }
   else if(raw_L_val>=50){
    normalized_L_val=1; //on the tape
   }*/

   if(raw_L_val<=40){
    normalized_L_val=0;
   }
   else if(raw_L_val>40){
    normalized_L_val=1;
   }

   return normalized_L_val;

}
int TapeSensors::get_norm_M_val(){

   if(raw_M_val<=40){
    normalized_M_val=0;
   }
   else if(raw_M_val>40){
    normalized_M_val=1;
   }

   return normalized_M_val;

}
int TapeSensors::get_norm_R_val(){

    if(raw_R_val<=37){
    normalized_R_val=0;
   }
   else if(raw_R_val>37){
    normalized_R_val=1;
   }

   return normalized_R_val;

}


float TapeSensors::follow_tape_speed_correction(){
    float motorspeed=0;
    get_error();
    P = (float)error;
    I = I + (float)error;
    D = (float)error - (float)last_error;
    last_error = error;
    motorspeed = P*kp + I*ki + D*kd;

    return motorspeed;

}




