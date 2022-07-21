#include "claw_servo_hall.h"

ClawServoHall::ClawServoHall(Servo &input_servo){

    myservo = input_servo;
    delay(15);
    
}





int ClawServoHall::grab_treasure(){
    
    myservo.write(CLAW_HALL);
    delay(500);

    int if_bomb = analogRead(HALL_PIN);
    
    if (if_bomb > 50){
        //if not bomb, pick up the treasure, and return 1 to indicate not bomb
        myservo.write(CLAW_GRAB);
        delay(50);
        return 1;
    }
    else{
        //if it is bomb, return to the initial angle, and return 0 to indicate it is bomb
        myservo.write(CLAW_INITIAL);
        delay(50);
        return 0;
    }

}

void ClawServoHall::grab_large_treasure(){
    myservo.write(CLAW_GRAB);
    delay(50);
}

int ClawServoHall::if_lift_up(){
    int lift_up = digitalRead(MICRO_SWIRCH);
    if (lift_up == 1){
        return 0;
    }
    else{
        return 1;
    }
}

void ClawServoHall::release_treasure(){
    myservo.write(CLAW_INITIAL);
    delay(50);
}