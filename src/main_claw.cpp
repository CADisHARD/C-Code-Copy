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

#define STEP_PER_CM 310
#define STEP_PER_IN 788

//**********************DECLARE HALL SENSOR**************

#define HALL PA6

//**********DECLARE SONARS*****************************

#define TRIG_L PB11
#define ECHO_L PB10

#define TRIG_R PB1
#define ECHO_R PB0
#define MAXIMUM_DISTANCE 4000

NewPing treasure_sonar_left(TRIG_L,ECHO_L,MAXIMUM_DISTANCE);
NewPing treasure_sonar_right(TRIG_R,ECHO_R,MAXIMUM_DISTANCE);

//*****************DECLARE BP**************************

#define COMMIN PA5
#define COMMOUT PA4



//****************DECLARE MICROSWITCHES******************


#define MS_GRAB PA3
#define MS_HORIZ PA2
#define MS_VERT PA1

//**********************DECLARE SERVO********************

#define SERVO_CLAW PA_0
Servo claw_servo;

//*****************CLAW SERVO ANGLE MEASUREMENT************
#define CLAW_INITIAL 90
#define CLAW_HALL 45
#define CLAW_GRAB 5


ClawServoHall claw_system(claw_servo);
StepperMotor stepper_motor;


void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Done");
  display.display();

  //setup for claw components
  pinMode(SERVO_CLAW, OUTPUT);
  pinMode (HALL, INPUT);
  pinMode (MS_GRAB, INPUT);

  //setup for stepper motor
  stepper_motor.resetEDPins();
  digitalWrite(EN, LOW);
  delay(300);


  claw_servo.attach(PA0);
  claw_servo.write(CLAW_INITIAL);
  

  attachInterrupt(digitalPinToInterrupt(ENCA_RP),rack_read_encoder_wrapper,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA_TT),turn_read_encoder_wrapper,RISING);

  delay(2000);
}


void loop() {

  //Reset Display
  display.clearDisplay();
  display.setCursor(0,0);

  /*
  delay(50);
  int distance = treasure_sonar_left.ping_cm();

  display.println("Distance: ");
  
  display.println(distance);

  if (distance <= 15)
  {
    display.println("Treasure detected! :D ");
  }
  else
  {
    display.println("No treasure detected :o ");
  }
  display.display();
  */

  
  delay(500);
  stepper_motor.descend(788*3);
  delay(5000);
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
  
}
