#ifndef CLAW_SERVO_HALL
#define CLAW_SERVO_HALL
#define CLAW_INITIAL 0
#define CLAW_GRAB 60

#include "Arduino.h"
#include "Wire.h"
#include "Servo.h"

class ClawServoHall{
    private:

        uint32_t HALL_PIN = PA7;
        Servo myservo;




    public:

        ClawServoHall(Servo &input_servo);
        void grab_treasure();
        int if_bomb();
        void release_treasure();

    
};


#endif