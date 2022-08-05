#include "stepper_motor.h"

StepperMotor::StepperMotor(uint32_t stp,uint32_t dir,uint32_t en,uint32_t ms1,uint32_t ms2){

    STP = stp;
    DIR = dir;
    EN = en;
    MS1 = ms1;
    MS2 = ms2;


    pinMode(STP, OUTPUT);
    pinMode(DIR, OUTPUT);
    pinMode(MS1, OUTPUT);
    pinMode(MS2, OUTPUT);
    pinMode(EN, OUTPUT);
    resetEDPins();
    digitalWrite(EN,LOW);
    

}

void StepperMotor::rise(){
    digitalWrite(DIR,LOW);
    for (int i=0; i<1000;i++){
      digitalWrite(STP,HIGH); //Trigger one step forward
      delayMicroseconds(700);
      digitalWrite(STP,LOW); //Pull step pin low so it can be triggered again
      delayMicroseconds(700);
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

