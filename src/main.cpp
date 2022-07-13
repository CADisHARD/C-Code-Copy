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
TapeSensors tpsens(tape_threshold);

//DECLARE EDGE DETECTION
int edge_threshold = 10;
const char *unit = "cm";
long duration; // variable for the duration of sound wave travel
int distance;
#define trigPin PA9
#define echoPin PA10


//MOTORS

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

void setMotor(int dir, int pwm, int in1, int in2);
void readEncoder();



int pos = 0;

void right_motor_encoder_wrapper();

void right_motor_encoder_wrapper(){
  right_motor.read_encoder();
}

void left_motor_encoder_wrapper(){
  left_motor.read_encoder();
}

void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  attachInterrupt(digitalPinToInterrupt(ENCA_R),right_motor_encoder_wrapper,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA_L),left_motor_encoder_wrapper,RISING);

  delay(2000);
}


void loop() {

  display.clearDisplay();
  display.setCursor(0,0);
  
  
  right_motor.set_direction(FORWARD);
  right_motor.set_pwm(2100);
  left_motor.set_direction(BACKWARD);
  left_motor.set_pwm(3000);
  right_motor.go();
  display.println(right_motor.get_position());
  left_motor.go();
  display.println(left_motor.get_position());

  delay(1000);
  right_motor.set_direction(BACKWARD);
  right_motor.set_pwm(2100);
  left_motor.set_direction(FORWARD);
  left_motor.set_pwm(3000);
  right_motor.go();
  display.println(right_motor.get_position());
  left_motor.go();
  display.println(left_motor.get_position());

  

  display.display();

  delay(1000);

  

  
}

 
/*void readEncoder(){
  int b = digitalRead(ENCB);
  if(b > 0){
    pos++;
  }
  else{
    pos--;
  }
}
*/

void setMotor(int dir, int pwm, int in1, int in2){
  analogWrite(in1,pwm);
  analogWrite(in2, pwm);
  if(dir == 1){
    //digitalWrite(in1,HIGH);
    //digitalWrite(in2,LOW);
  }
  else if(dir == -1){
    //digitalWrite(in1,LOW);
    //digitalWrite(in2,HIGH);
  }
  else{
    //digitalWrite(in1,LOW);
    //digitalWrite(in2,LOW);
  }
}