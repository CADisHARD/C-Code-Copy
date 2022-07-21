#ifndef CLAW_SERVO_HALL
#define CLAW_SERVO_HALL

#include "Arduino.h"
#include "Wire.h"
#include "Servo.h"

class ClawServoHall{
    private:

        uint32_t HALL_PIN = 0;
        uint32_t MICRO_SWIRCH = 0;
        PinName SERVO_PIN;
        Servo myservo;

        int initial_angle = 0;

        int claw_initial = 5;
        int claw_hall = 45;
        int claw_final = 90;
        int lift_up;


    public:

        ClawServoHall(PinName servo_pin, uint32_t hall_pin, uint32_t micro_switch);
        ClawServoHall(PinName servo_pin, int initial_position);
        void servo_move(int final_angle);
        int pick_up_treasure();
        int if_lift_up();
        void release_treasure();

    
};


#endif