#include "gyro.h"
#include "Arduino.h"
#include "Adafruit_MPU6050.h"


Gyroscope::Gyroscope(){

    if (!mpu.begin()) {
        while (1) {
            delay(10);
        }
    }
    //setupt motion detection
    mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
    mpu.setMotionDetectionThreshold(1);
    mpu.setMotionDetectionDuration(20);
    mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
    mpu.setInterruptPinPolarity(true);
    mpu.setMotionInterrupt(true);

}

void Gyroscope::get_values(){

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    gyroX=g.gyro.x;
    gyroY=g.gyro.y;
    gyroZ=g.gyro.z;

    accX=a.acceleration.x;
    accY=a.acceleration.y;
    accZ=a.acceleration.z;
}


void Gyroscope::measure_angle_z(){


    


}
