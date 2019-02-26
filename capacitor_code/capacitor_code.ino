
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);



#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const int OUT_PIN = A2;
const int IN_PIN = A0;
const float IN_STRAY_CAP_TO_GND = 24.48;
const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 34.8;  
const int MAX_ADC_VALUE = 1023;

void setup() {

    pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  // Set up the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize with the I2C addr 0x3D if not working use 0x3C (for the 128x64)
  display.setTextColor(WHITE);

}
void loop(){

   // draw scrolling text
  delay(1);
  display.clearDisplay();
display.setCursor(0,10);
  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
 


    pinMode(IN_PIN, INPUT);
    digitalWrite(OUT_PIN, HIGH);
    int val = analogRead(IN_PIN);
    digitalWrite(OUT_PIN, LOW);

    if (val < 1000)
    {
      pinMode(IN_PIN, OUTPUT);

      float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);

    display.print(F("Capacitance Value = "));
      display.print(capacitance, 3);
      display.print(F(" pF ("));
      display.print(val);
      display.println(F(") "));
    }
    else
    {
      pinMode(IN_PIN, OUTPUT);
      delay(1);
      pinMode(OUT_PIN, INPUT_PULLUP);
      unsigned long u1 = micros();
      unsigned long t;
      int digVal;

      do
      {
        digVal = digitalRead(OUT_PIN);
        unsigned long u2 = micros();
        t = u2 > u1 ? u2 - u1 : u1 - u2;
      } while ((digVal < 1) && (t < 400000L));

      pinMode(OUT_PIN, INPUT);  
      val = analogRead(OUT_PIN);
      digitalWrite(IN_PIN, HIGH);
      int dischargeTime = (int)(t / 1000L) * 5;
      delay(dischargeTime);   
      pinMode(OUT_PIN, OUTPUT);  
      digitalWrite(OUT_PIN, LOW);
      digitalWrite(IN_PIN, LOW);

      float capacitance = -(float)t / R_PULLUP
                              / log(1.0 - (float)val / (float)MAX_ADC_VALUE);

      display.print(F("Capacitance Value = "));
      if (capacitance > 1000.0)
      {
        display.print(capacitance / 1000.0, 2);
        display.print(F(" uF"));
      }
      else
      {
        display.print(capacitance, 2);
        display.print(F(" nF"));
      }

      display.print(F(" ("));
      display.print(digVal == 1 ? F("Normal") : F("HighVal"));
      display.print(F(", t= "));
      display.print(t);
      display.print(F(" us, ADC= "));
      display.print(val);
      display.println(F(")"));
    }
    while (millis() % 1000 != 0)
      ;    



  
  display.display();

}
