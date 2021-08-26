#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <Wire.h>

Adafruit_SSD1306 screen(128, 64, &Wire, 4);
char data[1];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(255000);
  screen.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  screen.clearDisplay();
  screen.display();
}

void loop() {
  // put your main code here, to run repeatedly:
  screen.clearDisplay();
  while(true)
  {
    Serial.readBytes(data, 1);
    if (data[0] == 'b')
    {
      break;
    }
    else
    {
    }
  }
  bool stop = false;
  for (int y = 0; y < 64; y++)
  {
    if (stop)
    {
      break;
    }
    for (int x = 0; x < 128; x++)
    {
      Serial.readBytes(data, 1);
      if (data[0] == '0')
      {
        screen.drawPixel(x, y, WHITE);
      }
      else if (data[0] == 's')
      {
        stop = true;
        break;
      }
    }
  }
  screen.display();
}