#include "claw_servo_hall.h"

ClawServoHall::ClawServoHall(Servo &input_servo){

    myservo = input_servo;
    delay(15);
    
}

int ClawServoHall::if_bomb(){
    int if_bomb = analogRead(HALL_PIN);
    delay(30);

    if (if_bomb < 200){
        return 1;
    }
    else{
        return 0;
    }

}

void ClawServoHall::grab_treasure(){
    int initial_position = myservo.read();
    while (initial_position > CLAW_GRAB){
        myservo.write(initial_position);
        delay(1);
        initial_position--;
    }
    while (initial_position < CLAW_GRAB){
        myservo.write(initial_position);
        delay(1);
        initial_position++;
    }
}


void ClawServoHall::release_treasure(){
    int initial_position = myservo.read();
    while (initial_position > CLAW_INITIAL){
        myservo.write(initial_position);
        delay(1);
        initial_position--;
    }
    while (initial_position < CLAW_INITIAL){
        myservo.write(initial_position);
        delay(1);
        initial_position++;
    }
}