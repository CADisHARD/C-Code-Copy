#include "claw_servo_hall.h"

ClawServoHall::ClawServoHall(PinName servo_pin, uint32_t hall_pin, uint32_t micro_switch){

    
    SERVO_PIN = servo_pin;
    HALL_PIN = hall_pin;
    MICRO_SWIRCH = micro_switch;

    pinMode (HALL_PIN, INPUT);
    pinMode (MICRO_SWIRCH, INPUT);
    myservo.attach(SERVO_PIN); //problem with this line
    /*Something wrong with it, i tried to attach the pin to servo by creating a separate function 
    to initialize but still not working*/
    initial_angle = claw_initial;
    myservo.write(initial_angle);
    delay(15);
    

}

ClawServoHall::ClawServoHall(PinName servo_pin, int initial_position){

    SERVO_PIN = servo_pin;
    myservo.attach(SERVO_PIN);
    initial_angle = initial_position;
    myservo.write(initial_angle);
    delay(15);


}

void ClawServoHall::servo_move(int final_angle){

    myservo.write(final_angle);
    delay(15);

}

int ClawServoHall::pick_up_treasure(){
    
    myservo.write(claw_hall);
    delay(50);

    int if_bomb = analogRead(HALL_PIN);
    
    if (if_bomb > 100){
        //if not bomb, pick up the treasure, and return 1 to indicate not bomb
        myservo.write(claw_final);
        delay(50);
        return 1;
    }
    else{
        //if it is bomb, return to the initial angle, and return 0 to indicate it is bomb
        myservo.write(claw_initial);
        delay(50);
        return 0;
    }

}


int ClawServoHall::if_lift_up(){
    lift_up = digitalRead(MICRO_SWIRCH);
    return lift_up;
}

void ClawServoHall::release_treasure(){
    myservo.write(claw_final);
    delay(50);
}