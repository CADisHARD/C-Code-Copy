#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET   -1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define REFLECTANCE1 PA0
#define REFLECTANCE2 PA1
#define REFLECTANCE3 PA2

void setup() {

  pinMode(REFLECTANCE1, INPUT);
  pinMode(REFLECTANCE2, INPUT);
  pinMode(REFLECTANCE3,INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.display();

  delay(2000);
  
}


void loop() {

  display.clearDisplay();
  display.setCursor(0,0);

  int reflec1 = analogRead(REFLECTANCE1);
  int reflec2 = analogRead(REFLECTANCE2);
  int reflec3 = analogRead(REFLECTANCE3);
  display.println(reflec1);
  display.println(reflec2);
  display.println(reflec3);
  display.display();

  

  delay(200); 
  
};