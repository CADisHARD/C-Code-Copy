#include "wheels.h"
#include "Arduino.h"
#include "encoder_motor.h"


void Wheels::go(){


    //left_motor.go();
    //right_motor.go();


}
void Wheels::stop(){

    left_motor.stop();
    right_motor.stop();

} 
void Wheels::steer_left(int amount){

    int right_pwm=right_motor.get_pwm();
    int left_pwm = right_pwm-amount;

    left_motor.set_pwm(left_pwm);

} //for now, "amount" is the velocity decrease on the wheel about which we are turning.
void Wheels::steer_right(int amount){

    int left_pwm=left_motor.get_pwm();
    int right_pwm = left_pwm-amount;

    right_motor.set_pwm(right_pwm);

} //This is only for slight steering
