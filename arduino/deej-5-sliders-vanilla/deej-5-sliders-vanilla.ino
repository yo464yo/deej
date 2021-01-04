#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A6};

int analogSliderValues[NUM_SLIDERS];
int values[NUM_SLIDERS];
String appnames[NUM_SLIDERS] = {"Master", "Discord", "Game", "Zoom", "Spotify"};

int volumePercentage;
void setup() { 
  Serial.begin(9600);
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Made by Yoav");
  display.display(); 
  display.clearDisplay();
  updateSliderValues();
  for (int i = 0; i < NUM_SLIDERS; i++) {
     values[i] = analogSliderValues[i];
  }
}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
     if(abs(values[i] - analogSliderValues[i]) > 9){
        values[i] = analogSliderValues[i];
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.println(appnames[i]);
        volumePercentage = map(analogSliderValues[i], 0, 1023, 0, 100);
        display.setTextSize(3);
        display.println(volumePercentage);
        display.display();  
     }
  }
  display.clearDisplay();
  delay(10);
}



void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
