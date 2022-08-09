
#include <Wire.h>
#include <SPI.h>
#include "images.h"
#include "Adafruit_GFX.h"
#include <Adafruit_SSD1306.h>
#include <NewPing.h>
#include "tape_sensor.h"
#include "ultrasonic_sensor.h"
#include "encoder_motor.h"
#include "claw_servo_hall.h"
#include "Servo.h"
#include "stepper_motor.h"

//*********************DECLARE OLED*****************************
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//*********************DECLARE MOTORS***************************

#define FORWARD 1
#define BACKWARD -1


#define ENCA_RP PB10
#define ENCB_RP PB11
#define PWM2_RP PB_9
#define PWM1_RP PB_8

volatile int cccounter = 0;



#define ENCA_TT PB0
#define ENCB_TT PB1
#define PWM2_TT PA_9
#define PWM1_TT PA_8

#define LEFT_POSITION 100
#define RIGHT_POSITION 100


volatile int inter_counter = 0;
volatile int position_rp=0;
volatile int position_tt=0;


EncoderMotor turn_table_motor(ENCA_TT, ENCB_TT, PWM1_TT, PWM2_TT);


EncoderMotor rack_n_pinion_motor(ENCA_RP, ENCB_RP, PWM1_RP, PWM2_RP);

#define ENCODER_OFFSET 2
#define ENCODER_OFFSET_RP 400


//*****************DECLARE STEPPER*************

#define STP PA12
#define DIR PA11  
#define EN PA15
#define MS1 PA10
#define MS2 PB3



#define STEP_PER_CM 310
#define STEP_PER_IN 788

StepperMotor stepper_motor(STP,DIR,EN,MS1,MS2);

//**********************DECLARE HALL SENSOR**************

#define HALL PA7

//**********DECLARE SONARS*****************************

#define TRIG_L PB12
#define ECHO_L PB13

#define TRIG_R PB14
#define ECHO_R PB15

#define TRIG_C PA1
#define ECHO_C PA2

#define MAXIMUM_DISTANCE 4000
#define CM_TO_RACK_CLICKS 1/3

NewPing treasure_sonar_left(TRIG_L,ECHO_L,MAXIMUM_DISTANCE); //right left as viewed from the top with back at y=0
NewPing treasure_sonar_right(TRIG_R,ECHO_R,MAXIMUM_DISTANCE);
NewPing treasure_sonar_claw(TRIG_C, ECHO_C, MAXIMUM_DISTANCE);


//****************DECLARE MICROSWITCHES******************


#define MS_GRAB PA3
#define MS_HORIZ PA2
#define MS_VERT PA1

//**********************DECLARE SERVO********************

#define SERVO_CLAW PA_0
Servo claw_servo;

//*****************CLAW SERVO ANGLE MEASUREMENT************
#define CLAW_INITIAL 0
#define CLAW_GRAB 60

ClawServoHall claw_system(claw_servo);

#define LEFT -1
#define RIGHT 1

//*********************THRESHOLD VALUES******************
#define TREASURE_ONE 10
#define THEASURE_TWO 10
#define ARCH_DISTANCE 10
#define TREASURE_THREE 10
#define TREASURE_FOUR 10

//*********************COURSE PROPERTIES********************

#define STOP 0
#define TAPE 1
#define ARCH_FIRST_TREASURE 2
#define ARCH_SECOND_TREASURE 3
#define BRIDGE 4
#define DUMP_TREASURE 5
#define GOLDEN_TREASURE 6
#define REVERSE 7

int stage = TAPE;
int treasure_count = 0;

#define COMM1 PA4 //abc -> a
#define COMM2 PA5 //abc -> b
#define COMM3 PA6 //abc -> c


void pick_up_treasure(int side);

#define TREASURE_POSITION_TT 13
#define TREASURE_POSITION_STP 3000
#define TREAUSRE_GRAB_PWM_TT 3200
#define TREASURE_RELEASE_PWM_TT 3200
#define TREASURE_GRAB_PWM_RP 4000
#define TREASURE_RELEASE_PWM_RP 4000

void read_encoder_turn_table();
void read_encoder_rack_pinion();
void go_to_position_tt(int pos);
void go_to_position_rp(int pos);
void send_message(int value);
void go_time_rp(int time_millis, int direction);

void setup() {

  //setup for the screen
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.drawBitmap(0, 0, claw_bitmap, 128, 64, WHITE);
  display.display();


  //setup for bluepill communication
  pinMode(COMM1,OUTPUT);
  pinMode(COMM2,OUTPUT);
  pinMode(COMM3,OUTPUT);

  
  //setup for sonar sensor
  pinMode(TRIG_L,OUTPUT);
  pinMode(ECHO_L,INPUT);
  pinMode(TRIG_R,OUTPUT);
  pinMode(ECHO_R,INPUT);
  pinMode(TRIG_C,OUTPUT);
  pinMode(ECHO_C,INPUT);

  

  //setup for claw components
  pinMode(SERVO_CLAW, OUTPUT);
  pinMode(HALL, INPUT);
  pinMode(MS_GRAB, INPUT);
  

  //setup for stepper motor
  stepper_motor.resetEDPins();
  digitalWrite(EN, LOW);
  delay(300);

  //initialize the claw
  
  claw_servo.attach(PA0);
  claw_servo.write((CLAW_GRAB+CLAW_INITIAL)/2);
  delay(100);

  int initial_position = claw_servo.read();
  display.println(initial_position);
  display.display();
  while (initial_position > CLAW_INITIAL){
    claw_servo.write(initial_position);
    delay(50);
    initial_position--;
  }
  while (initial_position < CLAW_INITIAL){
    claw_servo.write(initial_position);
    delay(50);
    initial_position++;
  }
  

  attachInterrupt(digitalPinToInterrupt(ENCA_RP),read_encoder_rack_pinion,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA_TT),read_encoder_turn_table,RISING);

  delay(200);
  
}

int counter =0;

void loop() {

  
  /*display.clearDisplay();
  display.setCursor(0,0);
  rack_n_pinion_motor.set_pwm(4000);
  rack_n_pinion_motor.set_direction(BACKWARD);
  //go_to_position_rp(-1000);
  rack_n_pinion_motor.go();*/


  
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(0,0);
  display.println(analogRead(HALL));
  display.println("sonar distance");
  display.println(treasure_sonar_claw.ping_cm());
  display.println(treasure_sonar_left.ping_cm());
  display.println(treasure_sonar_right.ping_cm());
  
  display.display();

  //get sonar sensor values
  int left_distance = treasure_sonar_left.ping_cm();
  int right_distance = treasure_sonar_right.ping_cm();
  int claw_distance = treasure_sonar_claw.ping_cm();
  
  display.println(left_distance);
  display.println(right_distance);
  display.println(claw_distance);
  display.display();  

  if(treasure_count<2){

    if (10<=right_distance&&right_distance<= 30){
      send_message(STOP);
      delay(400);
      display.println("TREASURE DETECTED. NAV STOPPED.");
      display.display();
      delay(400);
      rack_n_pinion_motor.set_pwm(TREASURE_GRAB_PWM_RP);
      turn_table_motor.set_pwm(TREAUSRE_GRAB_PWM_TT);

      

      go_to_position_tt(13);
      display.println("finished turn table");
      display.display();
      delay(2000);

      go_to_position_rp(800);
      display.println("finished extending");
      display.display();
      delay(2000);

      /*stepper_motor.descend(TREASURE_POSITION_STP);
      display.println("finished descending");
      display.display();
      delay(2000);*/

      /*if(treasure_sonar_claw.ping_cm()>14){

        display.println("can't see treasure. Will use base sonar.");

      }
      else{
        while(treasure_sonar_claw.ping_cm()>7){
          rack_n_pinion_motor.set_direction(FORWARD);
          rack_n_pinion_motor.go();
          display.clearDisplay();
          display.setCursor(0,0);
          display.println("sonar distance");
          display.println(treasure_sonar_claw.ping_cm());
          display.display();
          delayMicroseconds(5);
        }
        rack_n_pinion_motor.stop();
        delay(400);
      }*/


      if (claw_system.if_bomb()==0){
        claw_system.grab_treasure();
        delay(2000);
        display.println("finished grabbing claw");
        display.display();

      }

      /*stepper_motor.rise(TREASURE_POSITION_STP);
      display.println("finished rising");
      display.display();
      delay(2000);*/

      rack_n_pinion_motor.set_pwm(TREASURE_RELEASE_PWM_RP);
      go_to_position_rp(0);
      display.println("finished retracting rp");
      display.display();
      delay(2000);

      turn_table_motor.set_pwm(TREASURE_RELEASE_PWM_TT);
      go_to_position_tt(0);
      display.println("finished turn table");
      display.display();
      delay(2000);

      claw_system.release_treasure();
      display.println("released treasure");
      display.display();
      delay(2000);



      send_message(TAPE);
      treasure_count++;

      delay(3000);
    }
    else{
      send_message(TAPE);
      display.println("TAPE");
      display.display();

    }
  

  }
  else if(treasure_count==2){
    send_message(ARCH_FIRST_TREASURE);

    delay(10000);

    if(10<=left_distance&&left_distance<= 30){
      pick_up_treasure(LEFT);
    }

  }
  else{
    send_message(STOP);
  }
  

  delay(30);
  
 
  
}

void pick_up_treasure(int side){
  send_message(STOP);
  delay(400);
  display.println("TREASURE DETECTED. NAV STOPPED.");
  display.display();
  delay(400);
  rack_n_pinion_motor.set_pwm(TREASURE_GRAB_PWM_RP);
  turn_table_motor.set_pwm(TREAUSRE_GRAB_PWM_TT);

  /*go_to_position_rp(800);
  display.println("finished extending");
  display.display();
  delay(2000);*/

  go_to_position_tt(13);
  display.println("finished turn table");
  display.display();
  delay(2000);

  /*stepper_motor.descend(TREASURE_POSITION_STP);
  display.println("finished descending");
  display.display();
  delay(2000);*/




  /*if(treasure_sonar_claw.ping_cm()>14){

    display.println("can't see treasure. Will use base sonar.");

  }
  else{
    while(treasure_sonar_claw.ping_cm()>7){
      rack_n_pinion_motor.set_direction(FORWARD);
      rack_n_pinion_motor.go();
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("sonar distance");
      display.println(treasure_sonar_claw.ping_cm());
      display.display();
      delayMicroseconds(5);
    }
    rack_n_pinion_motor.stop();
    delay(400);
  }*/


  /*if (claw_system.if_bomb()==0){
    claw_system.grab_treasure();
    delay(2000);
    display.println("finished grabbing claw");
    display.display();

  }*/

  rack_n_pinion_motor.set_pwm(TREASURE_RELEASE_PWM_RP);
  go_to_position_rp(-1000);
  display.println("finished retracting rp");
  display.display();
  delay(2000);

  /*stepper_motor.rise(TREASURE_POSITION_STP);
  display.println("finished rising");
  display.display();
  delay(2000);*/




  turn_table_motor.set_pwm(TREASURE_RELEASE_PWM_TT);
  go_to_position_tt(0);
  display.println("finished turn table");
  display.display();
  delay(2000);

  /*claw_system.release_treasure();
  display.println("released treasure");
  display.display();
  delay(2000);*/



  send_message(TAPE);
  treasure_count++;

  delay(3000);
      
      
}

void send_message(int value){
  int first = 0;
  int second = 0;
  int third = 0;
  int output_value = value;

  if (output_value/4 != 0){
    output_value = output_value - 4;
    first = 1;
  }
  if (output_value/2 != 0){
    output_value = output_value -2;
    second = 1;
  }
  if (output_value != 0){
    third = 1;
  }

  digitalWrite(COMM1,first);
  digitalWrite(COMM2,second);
  digitalWrite(COMM3,third);

  

}


  
  



void go_time_rp(int time_millis, int direction){

    

    rack_n_pinion_motor.set_direction(direction);
    rack_n_pinion_motor.set_pwm(4000);
    long start_time=millis();
    //long currenet_time=millis();
    while(millis()-start_time<time_millis){

      rack_n_pinion_motor.go();
      delay(1);
    }
    rack_n_pinion_motor.stop();
}


  

void read_encoder_rack_pinion(){

  int b = digitalRead(ENCB_RP);
    if(b == HIGH){
        position_rp--;
    }
    else{
        position_rp++;
    }   

}

void read_encoder_turn_table(){

  int b = digitalRead(ENCB_TT);
    if(b == HIGH){
        position_tt++;
    }
    else{
        position_tt--;
    }  

}



void go_to_position_tt(int pos){

    if(pos-ENCODER_OFFSET<position_tt&&position_tt<pos+ENCODER_OFFSET){
      
        turn_table_motor.stop();
        display.clearDisplay();
         display.setCursor(0,0);
         display.println(position_tt);
         display.display();

    }
    else if(pos>position_tt){
     

        turn_table_motor.set_direction(-1);
        while(position_tt<pos){
       
             display.clearDisplay();
             display.setCursor(0,0);
             display.println(position_tt);
             display.display();
            turn_table_motor.go();
            delayMicroseconds(5);
        }
        turn_table_motor.stop();
        delay(200);

    }
    else if(pos-ENCODER_OFFSET<position_tt&&position_tt<pos+ENCODER_OFFSET){
   
       turn_table_motor.stop();
    }
    else if(pos<position_tt){
 

        //check the direction irl
        turn_table_motor.set_direction(1);
        while(position_tt>pos){
  
             display.clearDisplay();
             display.setCursor(0,0);
             display.println(position_tt);
             display.display();
            turn_table_motor.go();
            delayMicroseconds(5);//do we need this delay?
        }

        turn_table_motor.stop();
        delay(100);

    }
    else if(pos-ENCODER_OFFSET<position_tt&&position_tt<pos+ENCODER_OFFSET){

       turn_table_motor.stop();
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(position_tt);
        display.display();
    }


}

void go_to_position_rp(int pos){

    if(pos-ENCODER_OFFSET_RP<position_rp&&position_rp<pos+ENCODER_OFFSET_RP){
      rack_n_pinion_motor.stop();
      display.clearDisplay();
        display.setCursor(0,0);
        display.println(position_rp);
        display.display();
    }
    else if(pos>position_rp){

      rack_n_pinion_motor.set_direction(1);
      while(position_rp<pos){
            display.clearDisplay();
            display.setCursor(0,0);
            display.println(position_rp);
            display.display();
          rack_n_pinion_motor.go();
          delayMicroseconds(5);
      }
      rack_n_pinion_motor.stop();
      delay(200);

    }
    else if(pos-ENCODER_OFFSET_RP<position_rp&&position_rp<pos+ENCODER_OFFSET_RP){
       rack_n_pinion_motor.stop();
    }
    else if(pos<position_rp){

        //check the direction irl
        rack_n_pinion_motor.set_direction(-1);
        while(position_rp>pos){
             display.clearDisplay();
             display.setCursor(0,0);
             display.println(position_rp);
             display.display();
            rack_n_pinion_motor.go();
            delayMicroseconds(5);//do we need this delay?
        }
        rack_n_pinion_motor.stop();
        delay(200);

    }
    else if(pos-ENCODER_OFFSET_RP<position_rp&&position_rp<pos+ENCODER_OFFSET_RP){
       rack_n_pinion_motor.stop();
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(position_rp);
        display.display();
    }


}




