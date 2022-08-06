/*
#include <Wire.h>
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


EncoderMotor turn_table_motor(ENCA_TT, ENCB_TT, PWM1_TT, PWM2_TT, 24, 5);
void turn_read_encoder_wrapper();
void turn_read_encoder_wrapper(){
    turn_table_motor.read_encoder();
}

EncoderMotor rack_n_pinion_motor(ENCA_RP, ENCB_RP, PWM1_RP, PWM2_RP, 24, 5);
void rack_read_encoder_wrapper();
void rack_read_encoder_wrapper(){
    rack_n_pinion_motor.read_encoder();
}


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

#define MAXIMUM_DISTANCE 4000

NewPing treasure_sonar_left(TRIG_L,ECHO_L,MAXIMUM_DISTANCE); //right left as viewed from the top with back at y=0
NewPing treasure_sonar_right(TRIG_R,ECHO_R,MAXIMUM_DISTANCE);


//****************DECLARE MICROSWITCHES******************


#define MS_GRAB PA3
#define MS_HORIZ PA2
#define MS_VERT PA1

//**********************DECLARE SERVO********************

#define SERVO_CLAW PA_0
Servo claw_servo;

//*****************CLAW SERVO ANGLE MEASUREMENT************
#define CLAW_INITIAL 75
#define CLAW_HALL 45
#define CLAW_GRAB 0

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




int rack_has_gone = 0;

void read_encoder_turn_table();
void read_encoder_rack_pinion();
void go_to_position_tt(int pos);
void go_to_position_rp(int pos);


void setup() {

  //setup for the screen
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("STARTING SEQUENCE...");
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

  

  //setup for claw components
  pinMode(SERVO_CLAW, OUTPUT);
  pinMode(HALL, INPUT);
  pinMode(MS_GRAB, INPUT);
  

  //setup for limit switches
  pinMode(MS_HORIZ,INPUT);
  pinMode(MS_VERT,INPUT);
  

  //setup for stepper motor
  stepper_motor.resetEDPins();
  digitalWrite(EN, LOW);
  delay(300);

  int stage = 0;
  int output_signal = 1;


  claw_servo.attach(PA0);
  claw_servo.write(CLAW_HALL);
  delay(200);

  int initial_position = claw_servo.read();

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

  //Reset Display
  
  display.clearDisplay();
  display.setCursor(0,0);

  delay(30);
  
  //get sonar sensor values
  int left_distance = treasure_sonar_left.ping_cm();
  int right_distance = treasure_sonar_right.ping_cm();

  display.println(left_distance);
  display.println(right_distance);
  display.display();


  
  if (stage == TAPE){
    if (left_distance <= 5 && right_distance <= 5){
      stage++;
      delay(20);
    }
    else if (right_distance <= 10){
      send_message(STOP);
      //pick up treasure
    }
    else{
      send_message(stage);
    }
  }
  else if (stage==ARCH_FIRST_TREASURE){
    if (left_distance <= 10){
      send_message(STOP);
      //pick up treasure
      
      
      stage++;
    }
    else{
      send_message(stage);
    }

  }
  else if (stage==ARCH_SECOND_TREASURE){
    if (right_distance <= 10){
      send_message(STOP);
      //pick up treasure
      
      
      stage++;
    }
    else{
      send_message(stage);
    }

  }
  else if (stage==BRIDGE){
    if (left_distance <= 10){
      send_message(STOP);
      //pick up treasure
      
      
      stage++;
    }
    else{
      send_message(stage);
    }

  }
  else if (stage==DUMP_TREASURE){
    if (left_distance <= 10){
      send_message(STOP);
      //pick up treasure
      
      
      stage++;
    }
    else{
      send_message(stage);
    }
  }
  else if (stage==GOLDEN_TREASURE){
    send_message(stage);
  }

  
  
  //  display.println("Finished moving rack");
  // display.println(rack_n_pinion_motor.position);

  // display.display();

  


  /*
  display.println(analogRead(HALL));
  display.display();
  claw_system.grab_treasure();
  delay(3000);
  claw_system.release_treasure();
  delay(1000);
  */



/*


  if (right_distance <= 30){
    digitalWrite(COMM1,0);
    digitalWrite(COMM2,0);
    digitalWrite(COMM3,0);
    display.println("STOP!!!!!!!!");
        display.display();
    delay(500);
    rack_n_pinion_motor.set_pwm(3500);
    rack_n_pinion_motor.set_direction(FORWARD);
    rack_n_pinion_motor.go();



  }
  else{
    digitalWrite(COMM1,0);
    digitalWrite(COMM2,0);
    digitalWrite(COMM3,1);
    display.println("TAPE");
    display.display();

  }
  delay(30);
  */
  
/*
  claw_system.grab_large_treasure();

  delay(3000);
  claw_system.release_treasure();

   delay(3000);
   
   */

  //offsets: 3 for turn table 

  //turn_table_motor.set_pwm(3000);
  //rack_n_pinion_motor.set_pwm(4000);

  //turn_table_motor.go_to_position(-20, display);
  
  //claw_system.grab_large_treasure();
  
  /*display.display();
  delayMicroseconds(5);*/
  /*
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


  



  
  
  /*int distance = treasure_sonar_right.ping_cm();

  display.println("Distance: ");
  
  display.println(distance);
  display.display();
  //delay(50);

  if (distance <= 15)
  {
    display.println("Treasure detected! :D ");
    display.display();
    int if_grab = claw_system.grab_treasure();
    
    //delay(5000);
  }
  else
  {
    display.println("No treasure detected :o ");
    display.display();
  }
  display.display();
  delay(300);*/

  
  

  /*
  delay(500);
  stepper_motor.descend(788*3);
  delay(5000);
  */

  /*
  while(true){
    stepper_motor.rise(800);
    delay(500);
    stepper_motor.descend(800);
    delay(500);
  }
  */
  
  

  /*
  int if_grab = claw_system.grab_treasure();
  display.print(analogRead(HALL));
  display.print("\n");
  display.print(if_grab);
  display.print("\n");
  
  display.print(claw_system.if_lift_up());
  display.display();
  delay(50);
  */


  /*
  rack_n_pinion_motor.set_direction(BACKWARD);
  rack_n_pinion_motor.set_pwm(3500);
  rack_n_pinion_motor.go();
  display.println("The loop is working");
  display.display();
  delay(5000);
  claw_system.release_treasure();
  delay(2000);
  */

 //*****************CODE ATTEMPT************
/*
 int left_sonar_value = treasure_sonar_left.ping_cm();
 int rignt_sonar_value = treasure_sonar_right.ping_cm();

 if (stage == 0){
  if (left_sonar_value < TREASURE_ONE){
      pick_up_left(left_sonar_value);
  }
  else{
    analogWrite(COMMOUT,output_signal);
  }
  
*/


/*
void read_encoder_rack_pinion(){

  int b = digitalRead(ENCB_RP);
    if(b == HIGH){
        position_rp++;
    }
    else{
        position_rp--;
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

#define ENCODER_OFFSET 5

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
        delay(100);

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

    if(pos-ENCODER_OFFSET<position_rp&&position_rp<pos+ENCODER_OFFSET){
        rack_n_pinion_motor.stop();
        display.clearDisplay();
         display.setCursor(0,0);
         display.println(position_rp);
         display.display();
    }
    else if(pos>position_rp){

        rack_n_pinion_motor.set_direction(-1);
        while(position_rp<pos){
             display.clearDisplay();
             display.setCursor(0,0);
             display.println(position_rp);
             display.display();
            rack_n_pinion_motor.go();
            delayMicroseconds(5);
        }
        rack_n_pinion_motor.stop();
        delay(100);

    }
    else if(pos-ENCODER_OFFSET<position_rp&&position_rp<pos+ENCODER_OFFSET){
       rack_n_pinion_motor.stop();
    }
    else if(pos<position_rp){

        //check the direction irl
        rack_n_pinion_motor.set_direction(1);
        while(position_rp>pos){
             display.clearDisplay();
             display.setCursor(0,0);
             display.println(position_rp);
             display.display();
            rack_n_pinion_motor.go();
            delayMicroseconds(5);//do we need this delay?
        }
        rack_n_pinion_motor.stop();
        delay(100);

    }
    else if(pos-ENCODER_OFFSET<position_rp&&position_rp<pos+ENCODER_OFFSET){
       rack_n_pinion_motor.stop();
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(position_rp);
        display.display();
    }


}

*/


