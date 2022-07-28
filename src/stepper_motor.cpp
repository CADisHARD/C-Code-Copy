#include "stepper_motor.h"

StepperMotor::StepperMotor(){
    pinMode(STP, OUTPUT);
    pinMode(DIR, OUTPUT);
    pinMode(MS1, OUTPUT);
    pinMode(MS2, OUTPUT);
    pinMode(EN, OUTPUT);

}

void StepperMotor::rise(){
    digitalWrite(DIR,LOW);
    for (int i=0; i<1000;i++){
    digitalWrite(STP,HIGH); //Trigger one step forward
    delayMicroseconds(800);
    digitalWrite(STP,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(800);
  }

}

void StepperMotor::rise(int steps){
    digitalWrite(DIR,LOW);
    for (int i=0; i<steps;i++){
    digitalWrite(STP,HIGH); //Trigger one step forward
    delayMicroseconds(800);
    digitalWrite(STP,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(800);
  }

}
    

void StepperMotor::descend(){
  digitalWrite(DIR,HIGH);
  for (int i=0; i<1000;i++){
    digitalWrite(STP,HIGH); //Trigger one step forward
    delayMicroseconds(800);
    digitalWrite(STP,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(800);
  }

}    
    
void StepperMotor::descend(int steps){
  digitalWrite(DIR,HIGH);
  for (int i=0; i<steps;i++){
    digitalWrite(STP,HIGH); //Trigger one step forward
    delayMicroseconds(800);
    digitalWrite(STP,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(800);
  }

}



void StepperMotor::resetEDPins(){
  digitalWrite(STP, LOW);
  digitalWrite(DIR, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);

}

