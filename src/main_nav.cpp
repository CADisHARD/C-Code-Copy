#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "tape_sensor.h"
#include "ultrasonic_sensor.h"
#include "encoder_motor.h"
#include "Adafruit_MPU6050.h"
#include "images.h"

//*********************DECLARE OLED*****************************
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//*******************DECLARE TAPE SENSORS**********************
TapeSensors tpsens;

//********************DECLARE MOTORS**************************

//RIGHT MOTOR
#define ENCA_R PB13
#define ENCB_R PB12
#define PWM2_R PB_8
#define PWM1_R PB_9

EncoderMotor right_motor(ENCA_R, ENCB_R, PWM1_R, PWM2_R, 145, 5);

//LEFT MOTOR

#define ENCA_L PB15
#define ENCB_L PB14
#define PWM2_L PA_8
#define PWM1_L PA_9

EncoderMotor left_motor(ENCA_L, ENCB_L, PWM1_L, PWM2_L, 145, 5);

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


//************************DECLARE GYROSCOPE**********************
Adafruit_MPU6050 mpu;

float gyro_angle = 0;
float prev_gyro_angle=0;
float angular_vel=0;
float del=100;

long previous_time=0;
long current_time=0;

#define RIGHT 1
#define LEFT -1

void gyro_turn(int angle, int direction);
void get_gyro_angle();
float gyro_pid_correction();


//***************************DECLARE BP COMMUNICATION**********************
#define STOP 0
#define TAPE 1 
#define ARCH_FIRST_TREASURE 2
#define ARCH_SECOND_TREASURE 3 
#define BRIDGE 4
#define DUMP_TREASURE 5
#define GOLDEN_TREASURE 6
#define REVERSE 7

#define IN1 PA4
#define IN2 PA5
#define IN3 PA6

int read_claw_message();

//***************************COURSE PROPERTIES*****************************

int treasures_picked_up = 0;

//**************************SETUP******************************************


void oled_setup();
void gyro_setup();
void setup() {


  oled_setup();
  attachInterrupt(digitalPinToInterrupt(ENCA_R),right_motor_encoder_wrapper,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA_L),left_motor_encoder_wrapper,RISING);
  gyro_setup();
  tpsens.initial_reading();

  display.drawBitmap(0, 0, compass_bitmap, 128, 64, WHITE);
  display.display();
  delay(2000);
}

//*************************LOOP*********************************************


void loop(){

  //TAPE FOLLOWING
  //Reset Display

  //HAL_GPIO_ReadPin()

  int message = TAPE;

  if(message==STOP){
    right_motor.stop();
    left_motor.stop();
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("STOP");
    display.display();
    delay(2000);
  }
  else if(message==TAPE){

    display.clearDisplay();
    display.setCursor(0,0);

    right_motor.set_direction(FORWARD);
    left_motor.set_direction(FORWARD);

    right_motor.set_pwm(speed_right);
    left_motor.set_pwm(speed_left);

    left_motor.go();
    right_motor.go();
    tpsens.read_tape();

    if(tpsens.raw_L_val>200||tpsens.raw_M_val>200||tpsens.raw_R_val>200){ //on chicken wire
      right_motor.stop();
      left_motor.stop();
    }

    display.println(tpsens.raw_L_val);
    display.println(tpsens.raw_M_val);
    display.println(tpsens.raw_R_val);

    display.println(tpsens.position);
    display.println(tpsens.error);

    float speed_diff = tpsens.follow_tape_speed_correction();
    speed_check(speed_diff);

    

    display.println(speed_diff);
    display.println(speed_right);
    display.println(speed_left);
      
      
    display.display();
    delayMicroseconds(5);

  }

  else if(message==REVERSE){

    display.clearDisplay();
    display.setCursor(0,0);
    display.println("REVERSE");
    display.display();

    right_motor.set_direction(BACKWARD);
    left_motor.set_direction(BACKWARD);

    right_motor.set_pwm(3000);
    left_motor.set_pwm(3000);

    right_motor.go();
    left_motor.go();
  }

  /*display.clearDisplay();
  display.setCursor(0,0);

  get_gyro_angle();
  display.println(gyro_angle);

  //display.drawBitmap(0, 0, compass_bitmap, 128, 64, WHITE);
  display.display();
  delayMicroseconds(5);*/
  
  
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

    speed_right=speed_right+speed_diff;
    speed_left=speed_left-speed_diff;

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



int read_claw_message(){

  int in1=digitalRead(IN1);
  int in2=digitalRead(IN2);
  int in3=digitalRead(IN3);

  int value = in3+2*in2+4*in1;
  return value;

}

void gyro_setup(){

    //Try to initialize!
  if (!mpu.begin()) {
    display.println("Failed to find MPU6050 chip");
    display.display();
    while (1) {
      delay(10);
    }
  }
  //display.println("MPU6050 Found!");
  //setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
  previous_time=micros();

}


void oled_setup(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
}

void get_gyro_angle(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float z=g.gyro.z;
  if(abs(z)<0.1){
    z=0;
  }
  current_time=micros();
  float dt=(current_time-previous_time)/1000000.0;
  float dtheta=z*dt;
  gyro_angle=gyro_angle+dtheta*180.0/PI;
  previous_time=micros();
}

void gyro_turn(int angle, int direction){

  gyro_angle = 0;

  if(direction==RIGHT){

    right_motor.set_direction(BACKWARD);
    left_motor.set_direction(FORWARD);

  }
  else if(direction==LEFT){
    right_motor.set_direction(FORWARD);
    left_motor.set_direction(BACKWARD);
  }

  while(gyro_angle<angle){
    
    right_motor.go();
    left_motor.go();
    get_gyro_angle();
    display.println(gyro_angle);
    delayMicroseconds(5);

  }
  right_motor.stop();
  left_motor.stop();

}

float gyro_pid_correction(){

  float kp=100;
  float ki=0.0005;
  float kd=1000;


  float motorspeed=0;
  get_gyro_angle();
  float P = gyro_angle;
  float I = I + gyro_angle;
  float D = (gyro_angle-prev_gyro_angle)/((current_time-previous_time)/1000000.0);
  prev_gyro_angle = gyro_angle;
  motorspeed = P*kp + I*ki + D*kd;

  return motorspeed;

}