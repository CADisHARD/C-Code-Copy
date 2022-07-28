#ifndef CLAW_SERVO_HALL
#define CLAW_SERVO_HALL
#define CLAW_INITIAL 90
#define CLAW_HALL 45
#define CLAW_GRAB 5

#include "Arduino.h"
#include "Wire.h"
#include "Servo.h"

class ClawServoHall{
    private:

        uint32_t HALL_PIN = PA6;
        uint32_t MICRO_SWIRCH = PA3;
        Servo myservo;




    public:

        ClawServoHall(Servo &input_servo);
        int grab_treasure();
        void grab_large_treasure();
        int if_lift_up();
        void release_treasure();

    
};


#endif