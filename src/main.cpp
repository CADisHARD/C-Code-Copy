#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "tape_sensor.h"

//DECLARE OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//DECLARE TAPE SENSORS
int threshold=38;
TapeSensors tpsens(threshold);


void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  
  delay(2000);
}


void loop() {

  display.clearDisplay();
  display.setCursor(0,0);

  int L_val=tpsens.get_L_val();
  display.println(L_val);

  const char *tpsens_feedback=tpsens.routine();
  display.println(tpsens_feedback);
  display.display();
  delay(200); 

  
};