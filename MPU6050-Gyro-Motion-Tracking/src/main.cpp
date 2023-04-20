#include <Arduino.h>
//Alex Lib
#include <Core.cpp>
#include <Website.cpp>
#include<Drone.h>
//IMU imu;

void setup()
{
  Serial.begin(115200);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.display();//displays initial adafruit image
  oled.clearDisplay();//clears initial adafruit image
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.println("");
  oled.display();
  oled.clearDisplay();
  #ifdef DEBUGGING
    oled.setRotation(PI*2);
  #endif
  imu.Init();
  imu.Calibrate();
  
  website.Setup();
}

void loop()
{
  imu.Update();
  
  website.Loop();
  static KeyValue<String, String> msg("msg", " ");

  static KeyValue<String, String> posX("posX", "0");
  static KeyValue<String, String> posY("posY", "0");
  static KeyValue<String, String> posZ("posZ", "0");

  static KeyValue<String, String> rotX("rotX", "0");
  static KeyValue<String, String> rotY("rotY", "0");
  static KeyValue<String, String> rotZ("rotZ", "0");

  static KeyValue<String, String> i_x("i_x", "1");
  static KeyValue<String, String> i_y("i_y", "0");
  static KeyValue<String, String> i_z("i_z", "0");

  static KeyValue<String, String> j_x("j_x", "0");
  static KeyValue<String, String> j_y("j_y", "1");
  static KeyValue<String, String> j_z("j_z", "0");

  static KeyValue<String, String> k_x("k_x", "0");
  static KeyValue<String, String> k_y("k_y", "0");
  static KeyValue<String, String> k_z("k_z", "1");

  static long time = 0;
  static long prevTime = millis();
  static long delayMillisec = 10;
  time += (millis() - prevTime);
  prevTime = millis();
  if (time >= delayMillisec)
  {
    time = 0;

    posX.Value = imu.position.x;
    posY.Value = imu.position.y;
    posZ.Value = imu.position.z;

    rotX.Value = imu.rotation.x;
    rotY.Value = imu.rotation.y;
    rotZ.Value = imu.rotation.z;
    
    i_x.Value = imu.rotationMatrix.matrix[0][0];
    i_y.Value = imu.rotationMatrix.matrix[1][0];
    i_z.Value = imu.rotationMatrix.matrix[2][0];

    j_x.Value = imu.rotationMatrix.matrix[0][1];
    j_y.Value = imu.rotationMatrix.matrix[1][1];
    j_z.Value = imu.rotationMatrix.matrix[2][1];

    k_x.Value = imu.rotationMatrix.matrix[0][2];
    k_y.Value = imu.rotationMatrix.matrix[1][2];
    k_z.Value = imu.rotationMatrix.matrix[2][2];

    msg.Value = "";
    while(Serial.available())
    {
      msg.Value = Serial.readString();
    }

    KeyValue<String, String> socketData[16] = {
      msg,
      posX,
      posY,
      posZ,
      rotX,
      rotY,
      rotZ,
      i_x,
      i_y,
      i_z,
      j_x,
      j_y,
      j_z,
      k_x,
      k_y,
      k_z
    };

    website.socketSend(socketData, 16);
  }
}