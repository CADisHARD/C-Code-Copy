/*
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "tape_sensor.h"
#include "ultrasonic_sensor.h"
#include "encoder_motor.h"
#include "Adafruit_MPU6050.h"

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
#define ENCA_R PB12
#define ENCB_R PB13
#define PWM2_R PB_8
#define PWM1_R PB_9

EncoderMotor right_motor(ENCA_R, ENCB_R, PWM1_R, PWM2_R, 145, 5);

//LEFT MOTOR

#define ENCA_L PB14
#define ENCB_L PB15
#define PWM2_L PA_8
#define PWM1_L PA_9

EncoderMotor left_motor(ENCA_L, ENCB_L, PWM1_L, PWM2_L, 145, 5);


#define KPKNOB PA5
#define KIKNOB PA6
#define KDKNOB PA7

void right_motor_encoder_wrapper();

void left_motor_encoder_wrapper();


#define FORWARD 1
#define BACKWARD -1

#define INITIAL_TAPE_SPEED 4000.0
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

Adafruit_MPU6050 mpu;

float angle = 0;
float angular_vel=0;
float del=100;

long previous_time=0;
long current_time=0;

void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  attachInterrupt(digitalPinToInterrupt(ENCA_R),right_motor_encoder_wrapper,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA_L),left_motor_encoder_wrapper,RISING);

  display.display();
  

  tpsens.initial_reading();
  delay(2000);
}


void loop(){

  //Reset Display
  display.clearDisplay();
  display.setCursor(0,0);

  //TAPE FOLLOWING
  right_motor.set_direction(FORWARD);
  left_motor.set_direction(FORWARD);

  right_motor.set_pwm(speed_right);
  left_motor.set_pwm(speed_left);

  right_motor.go();
  left_motor.go();


  tpsens.read_tape();

  //display.println(tpsens.raw_L_val);
  //display.println(tpsens.raw_M_val);
  //display.println(tpsens.raw_R_val);
  
  display.println(tpsens.get_norm_L_val());
  display.println(tpsens.get_norm_M_val());
  display.println(tpsens.get_norm_R_val());

  display.println(tpsens.position);
  display.println(tpsens.error);

  float speed_diff = tpsens.follow_tape_speed_correction();
  speed_check(speed_diff);

  display.println(speed_diff);
  display.println(speed_right);
  display.println(speed_left);
  

  //Display and delay
  display.display();
  delayMicroseconds(5);
  //delay(500);
  
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

    speed_right=speed_right-speed_diff;
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

*/
