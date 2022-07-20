#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "tape_sensor.h"
#include "ultrasonic_sensor.h"
#include "encoder_motor.h"

//*********************DECLARE OLED*****************************
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//*******************DECLARE TAPE SENSORS**********************
int tape_threshold=38;
TapeSensors tpsens;

//DECLARE EDGE DETECTION
int edge_threshold = 10;
const char *unit = "cm";
long duration; // variable for the duration of sound wave travel
int distance;
#define trigPin PA9
#define echoPin PA10


//********************DECLARE MOTORS**************************

//RIGHT MOTOR
#define ENCA_R PB3
#define ENCB_R PB4
#define PWM2_R PB_8
#define PWM1_R PB_9

EncoderMotor right_motor(ENCA_R, ENCB_R, PWM1_R, PWM2_R);

//LEFT MOTOR

#define ENCA_L PB13
#define ENCB_L PB14
#define PWM2_L PA_8
#define PWM1_L PA_9

EncoderMotor left_motor(ENCA_L, ENCB_L, PWM1_L, PWM2_L);


#define KPKNOB PA5
#define KIKNOB PA6
#define KDKNOB PA7

void right_motor_encoder_wrapper();
void left_motor_encoder_wrapper();

#define FORWARD 1
#define BACKWARD -1

#define INITIAL_TAPE_SPEED 3000.0
#define MAXIMUM_TAPE_SPEED 6000.0

float speed_right=INITIAL_TAPE_SPEED;
float speed_left=INITIAL_TAPE_SPEED;

int position_right=0;
int position_left=0;

void speed_check(float speed_diff);


//*****************DECLARE ULTRASONIC SENSORS (EDGE DETECTION)**********

#define LEFT_EDGE_TRIG PA9
#define LEFT_EDGE_ECHO PA10

UltrasonicSensor left_edge_detector(LEFT_EDGE_TRIG, LEFT_EDGE_ECHO);

#define RIGHT_EDGE_TRIG PA11
#define RIGHT_EDGE_ECHO PA12

UltrasonicSensor right_edge_detector(RIGHT_EDGE_TRIG, RIGHT_EDGE_ECHO);


//****************DECLARE ROBOT PROPERTIES*******************

#define WIDTH 8 //width from wheel center to wheel center in inches
#define WHEEL_RADIUS 1.3 //inches
#define LEFT -1
#define RIGHT 1

void turn(int degrees);

//**************DECLARE CLAW MOTORS*****************************

//Rack and pinion

#define ENCA_RP PB10
#define ENCB_RP PB11
#define PWM2_RP PB_0
#define PWM1_RP PB_1

int position=0;

void rack_read_encoder_wrapper();

EncoderMotor rack_n_pinion_motor(ENCA_RP, ENCB_RP, PWM1_RP, PWM2_RP);


void read_enc(){
  if(digitalRead(ENCB_RP)>0){
    position++;
  }
  else{
    position--;
  }
}


void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  pinMode(ENCA_RP, INPUT);
  pinMode(ENCB_RP, INPUT);
  //Setup Motors

  attachInterrupt(digitalPinToInterrupt(ENCA_R),right_motor_encoder_wrapper,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA_L),left_motor_encoder_wrapper,RISING);


  attachInterrupt(digitalPinToInterrupt(ENCA_RP),read_enc,RISING);

  //pinMode(LEFT_EDGE_TRIG, OUTPUT);
  //pinMode(LEFT_EDGE_ECHO, INPUT);

  //Setup Tape Sensors
  tpsens.initial_reading();
  delay(2000);
}


void loop() {

  //Reset Display
  display.clearDisplay();
  display.setCursor(0,0);

  rack_n_pinion_motor.set_direction(FORWARD);
  rack_n_pinion_motor.set_pwm(3500);
  rack_n_pinion_motor.go();


  //display.println(right_motor.position);
  //display.println(left_motor.position);

  /*if(desired_position<right_motor.position){

    //check the direction irl
    right_motor.set_direction(1);
    left_motor.set_direction(-1);
    right_motor.go();
    left_motor.go();
    if(right_motor.position<desired_position){
      display.println(right_motor.position);
      display.println(left_motor.position);
      display.display();
      //delay(1);//do we need this delay?
    }
    else{
      right_motor.stop();
      left_motor.stop();
      delay(2000);
    }
    //delay(10);

  }
  else if(desired_position>right_motor.position){

    //check the direction irl
    right_motor.set_direction(-1);
    left_motor.set_direction(1);
    right_motor.go();
    left_motor.go();
    if(right_motor.position<desired_position){
      display.println(right_motor.position);
      display.println(left_motor.position);
      display.display();
      //delay(1);//do we need this delay?
    }
    else{
      right_motor.stop();
      left_motor.stop();
      delay(2000);
    }
  }
  else{
    display.println(right_motor.position);
    display.display();
    right_motor.stop();
    left_motor.stop();
    delay(10);
  }*/
  //delay(1000);

  


  //right_motor.turn(90, FORWARD);
  //right_motor.go();
  
  
  //Tape Following

  /*right_motor.set_direction(FORWARD);
  left_motor.set_direction(FORWARD);

  right_motor.set_pwm(speed_right);
  left_motor.set_pwm(speed_left);

  right_motor.go();
  left_motor.go();

  tpsens.read_tape();

  const char *message2=tpsens.get_error();
  int L_val=tpsens.get_norm_L_val();
  int M_val = tpsens.get_norm_M_val();
  int R_val=tpsens.get_norm_R_val();

  display.println(L_val);
  display.println(M_val);
  display.println(R_val);

  display.println(tpsens.position);
  display.println(tpsens.error);

  float speed_diff=tpsens.follow_tape_speed_correction();
  speed_check(speed_diff);

  display.println(speed_right);
  display.println(speed_left);
  display.println(speed_dif;f);*/


  //********************IR PORTION CODE****************************
  //GO STRAIGHT FOR TWO FEET

  /*display.println("the display is working :)");

  right_motor.set_direction(FORWARD);
  left_motor.set_direction(FORWARD);

  right_motor.set_pwm(speed_right);
  left_motor.set_pwm(speed_left);

  right_motor.go_distance(24,FORWARD);
  left_motor.go_distance(24, FORWARD);
 
  //SEES TREASURE: BACK UP, TURN TOWARDS IT

  right_motor.stop();
  left_motor.stop();

  delay(500);

  right_motor.go_distance(4, BACKWARD);
  left_motor.go_distance(4, BACKWARD);

  right_motor.stop();
  left_motor.stop();

  delay(500);

  turn(20, LEFT);
  delay(500);

  right_motor.go_distance(6, FORWARD);
  left_motor.go_distance(6, FORWARD);

  delay(1000);

  //GO BACK THE SAME WAY IT CAME

  right_motor.go_distance(6, BACKWARD);
  left_motor.go_distance(6, BACKWARD);

  delay(500);
  turn(20, RIGHT);
  delay(500);

  right_motor.stop();
  left_motor.stop();

  //CONTINUE FORWARD

  right_motor.go_distance(12,FORWARD);
  left_motor.go_distance(12, FORWARD);

  //TURN RIGHT

  right_motor.stop();
  left_motor.stop();

  delay(500);

  turn(90, RIGHT);

  delay(500);*/
  
  //******************************************************
  
  display.display();
  delay(1);
  
}

void speed_check(float speed_diff){

    if(speed_diff<=2&&speed_diff>=0){
      speed_diff=0;
      speed_right=INITIAL_TAPE_SPEED;
      speed_left=INITIAL_TAPE_SPEED;
    }
    if(speed_diff>=-2&&speed_diff<=0){
      speed_diff=0;
      speed_right=INITIAL_TAPE_SPEED;
      speed_left=INITIAL_TAPE_SPEED;
    }

    /*if(speed_diff>=0){
      speed_right=speed_right-(speed_diff/10.0);
    }
    else if(speed_diff<0){
      speed_right=speed_right-(speed_diff*10.0);
    }*/

    speed_right=speed_right-speed_diff*0.05;
    speed_left=speed_left+speed_diff;

    if(speed_right>MAXIMUM_TAPE_SPEED){
      speed_right=MAXIMUM_TAPE_SPEED;
    }
    if(speed_left>MAXIMUM_TAPE_SPEED){
      speed_left=MAXIMUM_TAPE_SPEED;
    }
    if(speed_right<0){
      speed_right=0;
    }
    if(speed_left<0){
      speed_left=0;
    }

}

void right_motor_encoder_wrapper(){
  right_motor.read_encoder();
}

void left_motor_encoder_wrapper(){
  left_motor.read_encoder();
}

void turn(int degrees, int direction){

  int wheel_deg = (WIDTH/2*degrees)/(WHEEL_RADIUS);
  
  if(direction==RIGHT){

    right_motor.turn(wheel_deg, BACKWARD);
    left_motor.turn(wheel_deg, FORWARD);

  } 
  else if(direction==LEFT){
    right_motor.turn(wheel_deg, FORWARD);
    left_motor.turn(wheel_deg, BACKWARD);
  }

}
