#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include "stdint.h"
#include "Arduino.h"

class UltrasonicSensor{

    private:

        uint32_t TRIGGER = 0;
        uint32_t ECHO= 0;
        double SOS_CM = 0.0343;
        double SOS_IN=0.0135039;
        double SOS_MM = 0.343;

        const char *unit="in"; //options: in, cm, mm
        const char *type="edge"; //options: edge, treasure

        long distance=1;
        double threshold=2.7;

    public:

        UltrasonicSensor(uint32_t trigger,uint32_t echo);
        void set_threshold(int thresh){threshold=thresh;}
        void set_unit(const char *u){unit=u;}
        void set_type(const char *t){type=t;}
        int get_threshold(){return threshold;}
        const char *get_type(){return type;}
        double get_distance(){return distance;}
        const char *get_unit(){return unit;}
        int routine();


};

const char *edge_detection(UltrasonicSensor left_sensor, UltrasonicSensor right_sensor, int thresh, const char *unit);


#endif