#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "tape_sensor.h"
#include "ultrasonic_sensor.h"

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


void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  
  delay(2000);
}


void loop() {

  display.clearDisplay();
  display.setCursor(0,0);
  
  
  const char *message = edge_detection(50, "in");
  display.println(message);
  display.display();
  delay(200); 

  
};