#ifndef TAPE_SENSOR_H
#define TAPE_SENSOR_H

#include "encoder_motor.h"
#include "stdint.h"
#include "Arduino.h"

class TapeSensors{

    private:

        uint32_t REFLECTANCE_L =PA0;
        uint32_t REFLECTANCE_M =PA1;
        uint32_t REFLECTANCE_R =PA2;

        int L_val=0;
        int M_val=0;
        int R_val=0;
        int threshold=30;

    public:


        TapeSensors(int thresh);
        const char *routine();
        void set_threshold(int thresh);
        int get_threshold(){return threshold;}
        int get_L_val(){return L_val;}
        int get_M_val(){return M_val;}
        int get_R_val(){return R_val;}

};

#endif