#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "tape_sensor.h"
#include "ultrasonic_sensor.h"
#include "wheels.h"

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


//DECLARE MOTORS

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

#define FORWARD 1
#define BACKWARD -1


#define KPKNOB PB1
#define KIKNOB PB0
#define KDKNOB PA4

void right_motor_encoder_wrapper();

void right_motor_encoder_wrapper(){
  right_motor.read_encoder();
}

void left_motor_encoder_wrapper(){
  left_motor.read_encoder();
}

  int speed_right=3000;
  int speed_left=3000;


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

  attachInterrupt(digitalPinToInterrupt(ENCA_R),right_motor_encoder_wrapper,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA_L),left_motor_encoder_wrapper,RISING);

  tpsens.initial_reading();

  delay(2000);
}


void loop() {

  //Reset Display
  display.clearDisplay();
  display.setCursor(0,0);
  
  //STAGE 1: Tape-following

  /*tpsens.kp = analogRead(KPKNOB);
  tpsens.ki = analogRead(KIKNOB);
  tpsens.kd = analogRead(KDKNOB);

  display.println(tpsens.kp);
  display.println(tpsens.ki);
  display.println(tpsens.kd);*/
  

  //TAPE FOLLOWING

  /*right_motor.set_direction(FORWARD);
  left_motor.set_direction(FORWARD);

  right_motor.set_pwm(speed_right);
  left_motor.set_pwm(speed_left);

  right_motor.go();
  left_motor.go();*/

  tpsens.read_tape();

  const char *message2=tpsens.get_error();
  int L_val=tpsens.get_norm_L_val();
  int M_val = tpsens.get_norm_M_val();
  int R_val=tpsens.get_norm_R_val();

  display.println("Left normalized: ");
  display.print(L_val);
  display.println("Middle normalized: ");
  display.print(M_val);
  display.println("Right normalized: ");
  display.print(R_val);

  display.println("Position type: ");
  display.print(message2);

  display.println("Current position value: ");
  display.print(tpsens.position);

  display.println("Prev position value: ");
  display.print(tpsens.last_position);

    display.println("Current error value: ");
  display.print(tpsens.error);

  display.println("Prev error value: ");
  display.print(tpsens.last_error);

  int *new_motor_speeds=tpsens.follow_tape_speed_correction(speed_right, speed_left);

  speed_right=new_motor_speeds[0];
  speed_left=new_motor_speeds[1];
  
  display.display();
  delay(200);
  
}


