#include "claw_servo_hall.h"

ClawServoHall::ClawServoHall(Servo &input_servo){

    myservo = input_servo;
    delay(15);
    
}


int ClawServoHall::grab_treasure(){
    
    int initial_position = myservo.read();
    while (initial_position > CLAW_HALL){
        myservo.write(initial_position);
        delay(20);
        initial_position--;
    }
    while (initial_position < CLAW_HALL){
        myservo.write(initial_position);
        delay(20);
        initial_position++;
    }

    int if_bomb = analogRead(HALL_PIN);
    delay(500);
    
    if (if_bomb > 50){
        //if not bomb, pick up the treasure, and return 1 to indicate not bomb
        int current_position = myservo.read();
        while (current_position > CLAW_GRAB){
            myservo.write(current_position);
            delay(20);
            current_position--;
        }
        while (current_position < CLAW_GRAB){
            myservo.write(current_position);
            delay(20);
            current_position++;
        }
        return 1;
    }
    else{
        //if it is bomb, return to the initial angle, and return 0 to indicate it is bomb
        release_treasure();
        return 0;
    }

}

void ClawServoHall::grab_large_treasure(){
    int initial_position = myservo.read();
    while (initial_position > CLAW_GRAB){
        myservo.write(initial_position);
        delay(20);
        initial_position--;
    }
    while (initial_position < CLAW_GRAB){
        myservo.write(initial_position);
        delay(20);
        initial_position++;
    }
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
    int initial_position = myservo.read();
    while (initial_position > CLAW_INITIAL){
        myservo.write(initial_position);
        delay(20);
        initial_position--;
    }
    while (initial_position < CLAW_INITIAL){
        myservo.write(initial_position);
        delay(20);
        initial_position++;
    }
}