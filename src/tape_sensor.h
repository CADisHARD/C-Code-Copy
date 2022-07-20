#ifndef TAPE_SENSOR_H
#define TAPE_SENSOR_H

#include "stdint.h"
#include "Arduino.h"

class TapeSensors{

    private:

        uint32_t REFLECTANCE_L =PA0;
        uint32_t REFLECTANCE_M =PA1;
        uint32_t REFLECTANCE_R =PA2;

        int raw_L_val=0;
        int raw_M_val=0;
        int raw_R_val=0;

        int normalized_L_val=0;
        int normalized_M_val=0;
        int normalized_R_val=0;

        int previous_sensor_values[3] = {0,0,0};
        int current_sensor_values[3]={0,0,0};       

        int MAX_TAPE_FOLLOWING_PWM=6000;
        int IDEAL_POSITION=3;

        
        const char *prev_message_000="hello";
        
    public:

        //temporary for debugging only

        float P=0;
        float I=0;
        float D=0;
        float kp=4;
        float ki=0.0005;
        float kd = 90;
        int position=0;
        int error=0;
        int last_error=0;
        int last_position=0;

        TapeSensors();
        void read_tape();
        int get_norm_L_val();
        int get_norm_M_val();
        int get_norm_R_val();
        void initial_reading();

        const char *get_error();
        float follow_tape_speed_correction();
        

};

#endif