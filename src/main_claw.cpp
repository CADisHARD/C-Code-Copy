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

//*********************DECLARE MOTORS***************************

#define FORWARD 1
#define BACKWARD -1


#define ENCA_RP PB12
#define ENCB_RP PB13
#define PWM2_RP PB_9
#define PWM1_RP PB_8

EncoderMotor rack_n_pinion_motor(ENCA_RP, ENCB_RP, PWM1_RP, PWM2_RP);
void rack_read_encoder_wrapper();
void rack_read_encoder_wrapper(){
    rack_n_pinion_motor.read_encoder();
}

#define ENCA_TT PB14
#define ENCB_TT PB15
#define PWM2_TT PA_9
#define PWM1_TT PA_8

EncoderMotor turn_table_motor(ENCA_TT, ENCB_TT, PWM1_TT, PWM2_TT);
void turn_read_encoder_wrapper();
void turn_read_encoder_wrapper(){
    turn_table_motor.read_encoder();
}


//*****************DECLARE STEPPER*************

#define STP PA10
#define DIR PA11
#define EN PA12
#define MS1 PA15
#define MS2 PB3

//**********************DECLARE HALL SENSOR**************

#define HALL PB5

//**********DECLARE SONARS*****************************

#define TRIG_L PB11
#define ECHO_L PB10

#define TRIG_R PB1
#define ECHO_R PB0

//*****************DECLARE BP**************************

#define COMMIN PA5
#define COMMOUT PA4



//****************DECLARE MICROSWITCHES******************


#define MS_GRAB PA3
#define MS_HORIZ PA2
#define MS_VERT PA1

//**********************DECLARE SERVO********************

#define SERVO_CLAW PA0

void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  attachInterrupt(digitalPinToInterrupt(ENCA_RP),rack_read_encoder_wrapper,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA_TT),turn_read_encoder_wrapper,RISING);

  delay(2000);
}


void loop() {

  //Reset Display
  display.clearDisplay();
  display.setCursor(0,0);

  rack_n_pinion_motor.set_direction(BACKWARD);
  rack_n_pinion_motor.set_pwm(3500);
  rack_n_pinion_motor.go();
  display.println(rack_n_pinion_motor.position);

  //Display and delay
  display.display();
  delay(1);
  
}
