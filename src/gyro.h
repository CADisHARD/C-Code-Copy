#ifndef GYRO_H
#define GYRO_H


#include "Arduino.h"
#include "Adafruit_MPU6050.h"

class Gyroscope{


    private:

        Adafruit_MPU6050 mpu;

        float gyroX=0.0;
        float gyroY=0.0;
        float gyroZ=0.0;

        float accX=0.0;
        float accY=0.0;
        float accZ=0.0;

        float angleX=0.0;
        float angleY=0.0;
        float angleZ=0.0;

        void get_values();

    public:

    Gyroscope();
    float get_angle_x();
    float get_angle_y();
    float get_angle_z();
    void measure_angle_z(); //angle about the vertical axis that we care about the most: the robot rotates flat on the ground

};


#endif