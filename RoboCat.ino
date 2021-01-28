
// Hardware setup:
//
// Arduino Uno
// PIR sensor (motion detection) on A0 (had trouble on digital pins)
// OLED display (128x64) on pins 4 (SDA) and 5 (SCL)
// ### Not yet used: ###
// Arduino motor shield
// Two geared wheel motors (20x22x70mm)

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PIR_PIN A0 // Pin of the PIR sensor

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define STATE_SLEEP 0
#define STATE_ALERT 1
#define STATE_SCAN  2
#define STATE_FLEE  3

int state = STATE_SLEEP;
int pir   = 0;

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,20);
  display.println("Cat booting up...");
  display.display();
  
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  
  delay(1000);
}

void loop() {
  pir = digitalRead(PIR_PIN);

  if(pir == 1 && state == STATE_SLEEP) {
    state = STATE_ALERT;
  } else if (pir == 0 && state == STATE_ALERT) {
    state = STATE_SLEEP;
  }
  
  paintDisplay();

  delay(500);
}

void paintDisplay() {
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(35,43);
  display.println(pir == 1 ? "O.O" : ". .");

  display.setTextSize(1);
  display.setCursor(0,10);
  
  switch(state) {
    case STATE_SLEEP:
      // show no text to save power
      break;
    case STATE_ALERT:
      display.println("alert");
      break;
    case STATE_SCAN:
      display.println("scanning");
      break;
    case STATE_FLEE:
      display.println("fleeing");
      break;
  }
  
  display.display();
}
