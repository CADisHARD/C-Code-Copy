#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "tape_sensor.h"
#include "ultrasonic_sensor.h"
#include "encoder_motor.h"

//DECLARE OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//DECLARE TAPE SENSORS
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

void right_motor_encoder_wrapper(){
  right_motor.read_encoder();
}

void left_motor_encoder_wrapper(){
  left_motor.read_encoder();
}


#define FORWARD 1
#define BACKWARD -1

#define INITIAL_TAPE_SPEED 3000.0
#define MAXIMUM_TAPE_SPEED 6000.0

float speed_right=INITIAL_TAPE_SPEED;
float speed_left=INITIAL_TAPE_SPEED;


void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  pinMode(KPKNOB, INPUT);
  pinMode(KIKNOB, INPUT);
  pinMode(KDKNOB, INPUT);

  //Setup Motors

  attachInterrupt(digitalPinToInterrupt(ENCA_R),right_motor_encoder_wrapper,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA_L),left_motor_encoder_wrapper,RISING);

  //Setup Tape Sensors
  tpsens.initial_reading();

  delay(2000);
}


void loop() {

  //Reset Display
  display.clearDisplay();
  display.setCursor(0,0);
  
  //Tape Following

  right_motor.set_direction(FORWARD);
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


  float kpval=analogRead(KPKNOB)/100.0;
  float kival = analogRead(KIKNOB)/100.0;
  float kdval = analogRead(KDKNOB)/100.0;

  tpsens.kp=kpval;
  tpsens.ki=kival;
  tpsens.kd=kdval;

  //display.println(kpval);
  //display.println(kival);
  //display.println(kdval);

  display.println(speed_right);
  display.println(speed_left);
  display.println(speed_diff);
  
  display.display();
  delay(200);
  
}

void speed_check(float speed_diff){

    if(speed_diff==0){
      speed_right=INITIAL_TAPE_SPEED;
      speed_left=INITIAL_TAPE_SPEED;
    }

    /*if(speed_diff>=0){
      speed_right=speed_right-(speed_diff/10.0);
    }
    else if(speed_diff<0){
      speed_right=speed_right-(speed_diff*10.0);
    }*/

    //speed_right=speed_right-speed_diff*0.05;
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
