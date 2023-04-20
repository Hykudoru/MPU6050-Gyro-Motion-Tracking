//namespace Hyku {
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MotorShield.h>
#include <utility/Adafruit_MS_PWMServoDriver.h>
#include <Functions.h>
#include <WirelessData.h>
#include <RotationMatrixVectorLib/Vector.h>
#include <RotationMatrixVectorLib/Matrix.h>

#ifndef CORE_H
#define CORE_H   

#define DEBUGGING

//pfunc can be reasigned at runtime to change the desired procedure invoked inside the default loop function.
typedef void (*pointerFunction)(void);
pointerFunction ptrMode;
Adafruit_SSD1306 oled = Adafruit_SSD1306(128, 32, &Wire);
IMU imu;

template <typename TKey, typename TValue>
class KeyValue
{
  public:
  TKey Key;
  TValue Value;

  KeyValue()
  {
    Key = "";
    Value = "";
  }

  KeyValue(TKey key, TValue value) 
  {
    Key = key;
    Value = value;
  }
};

template<typename T>
void debug(T str)
{
  Serial.println(str); 
  oled.setCursor(0, oled.getCursorY());
  oled.println(str);
  oled.display();
  oled.setCursor(0, oled.getCursorY()+1);
}

void clear()
{
  oled.clearDisplay();
  oled.setCursor(0, 0);
}

#endif