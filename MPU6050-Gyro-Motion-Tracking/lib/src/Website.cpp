#include <Wire.h>
#include <Wifi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <Functions.h>
#include <Core.cpp>
#include <Drone.h>

#ifndef WEBSITE_H
#define WEBSITE_H

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
const char* SSID = "07410202";
const char* PASSWORD = "ekvnrqqE8hDy";
// const char* SELF_SSID = "ESP32 AP";
// const char* SELF_PASS = "299792458";
StaticJsonDocument<512> outgoing; 
StaticJsonDocument<200> incoming; 
static int numClientsConnected = 0;

class Website
{
public:
  static String Webpage;
  
  static void socketReceive(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
  {
    switch (type)
    {
    case WStype_CONNECTED:
      numClientsConnected++;
      Serial.print("Client ");
      Serial.print(num);
      Serial.print(" connected. Total:");
      Serial.println(numClientsConnected);
    break;
    case WStype_DISCONNECTED:
        numClientsConnected--;
      Serial.print("Client ");
      Serial.print(num);
      Serial.print(" disconnected. Total:");
      Serial.println(numClientsConnected);
    break;
    case WStype_TEXT:
      DeserializationError error = deserializeJson(incoming, payload);
      if (error)
      {
        Serial.println("Error: Couldn't deserialize.");
      }
      else {
        //------Server Commands----------
        String msg = incoming["msg"];
        Serial.println("Message: "+msg);
        msg.toLowerCase();
        if (msg == "restart" || msg == "server restart" || msg == "restart server") {
          ESP.restart();
        }
        else if (msg == "calibrate" || msg == "reset" || msg == "calibrate gyro" || msg == "reset gyro") {
          imu.Calibrate();
        }
        clear();
        debug(WiFi.localIP().toString());
      }
    }
  }

  static void socketSend(KeyValue<String, String> *keyValuePair, int size)
  {
    String jsonString = "";
    JsonObject root = outgoing.to<JsonObject>();

    for (int i=0; i < size; i++)
    {
      if (keyValuePair[i].Key.length() > 0)
      {
        root[keyValuePair[i].Key] = keyValuePair[i].Value;
      }
    }
    
    serializeJson(outgoing, jsonString);
    webSocket.broadcastTXT(jsonString);
  }

  void Setup()
  {
    clear();
    WiFi.mode(WIFI_AP_STA);
    //WiFi.softAP(SELF_SSID, SELF_PASS);
    WiFi.begin(SSID, PASSWORD);
    while(WiFi.status() != WL_CONNECTED)
    { 
      delay(1000);
      debug("Attempting to connect to wifi...");
    }
    debug("Connected.");
    delay(1000);
    clear();

    debug(WiFi.localIP().toString());
    //debug(WiFi.softAPIP().toString());

    server.on("/", []() {
      server.send(200, "text/html", Webpage);
    });

    server.begin();
    webSocket.begin();
    webSocket.onEvent(socketReceive);
  }

  void Loop() 
  {
    server.handleClient();
    webSocket.loop();
  }
}; 

Website website;

String Website::Webpage =  R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.3/jquery.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/p5.js/1.1.9/p5.js"></script>
        <title>IMU</title>
    <style>
        body {
            background-color: #1b1a1a;
            color: rgb(213, 212, 212);
            text-align: center;
            margin:auto;
            padding: auto;
        }
        h1 {
            text-align: center;
            padding: 20px;
        }
        canvas {
            min-width: 340px;
            min-height: 340px;
            max-width: 640px;
            max-height: 640px;
        }
    </style>
</head>
<body>
    <h1>Inertial Measurement Unit</h1>
    <script src="sketch.js"></script>
</body>
<script>
    //-----------GRAPHICS---------------
    let imu = {
        rotX: 0,
        rotY: 0,
        rotZ: 0,
    
        i: {x: 0, y: 0, z: -1},
        j: {x: 1, y: 0, z: 0},
        k: {x: 0, y: 1, z: 0},

        posX: 0,
        posY: 0,
        posZ: 0
    };

    let min = 340;
    let max = 640;
    let screenWidth = screen.width - 30;
    if (screen.width < min) {
        screenWidth = min;
    } else if (screenWidth > max) {
        screenWidth = max;
    }

    function setup() {
        createCanvas(screenWidth, screenWidth, WEBGL);
    }
    
    function draw() {
        background(0);
        rectMode(CENTER);
        angleMode(DEGREES);
        
        strokeWeight(2)
        radius = screenWidth/3;//(screen.width <= 700) ? screen.width/4 : 800/4;
        stroke(255);
        line(0, 0, 0, imu.i.x * radius, imu.i.y * radius, imu.i.z * radius);
        line(0, 0, 0, imu.j.x * radius, imu.j.y * radius, imu.j.z * radius);
        line(0, 0, 0, imu.k.x * radius, imu.k.y * radius, imu.k.z * radius);
        
        strokeWeight(4);
        stroke(255);
        point(imu.i.x * radius, imu.i.y * radius, imu.i.z * radius);
        point(imu.j.x * radius, imu.j.y * radius, imu.j.z * radius);
        point(imu.k.x * radius, imu.k.y * radius, imu.k.z * radius);
        
        // push();
        // rotateZ(angle++ * 0.1);
        // rotateY(angle++ *0.1);
        // rotateX(angle++ * 0.1);
    
        // stroke(255);
        // fill(0, 180, 220);//normalMaterial();
        
        // beginShape();//Body
        //     vertex(0, 0, -radius);
        //     vertex(-radius/2, 0, radius/2);
        //     vertex(-radius/2, 0, radius/2);
        //     vertex(radius/2, 0, radius/2);
        // endShape();
        
        // //fill(0, 180, 220);
        // beginShape();//Tail fin
        //     vertex(-1, -1, radius/4);
        //     vertex(-1, -1, radius/2);
        //     vertex(-1, -radius/3, radius/2);
        //     vertex(1, -1, radius/4);
        //     vertex(1, -1, radius/2);
        //     vertex(1, -radius/3, radius/2);
        // endShape();
        
        // pop();
    }
    
    //-----------WEB SOCKETS---------------
    
    var socket;
    function initWebSockets() {
        socket = new WebSocket('ws://' + window.location.hostname + ':81/');
        socket.onmessage = function(event) {
            incomingServerCommand(event);
        }
    }
    
    function incomingServerCommand(event) {
        console.log(event.data);
        var incoming  = JSON.parse(event.data);
        console.log(incoming);
    
        // Convert R-hand to L-hand 
        imu.rotX = -(incoming.rotY);
        imu.rotY = -(incoming.rotZ); 
        imu.rotZ = -(-incoming.rotX);
    
        // Map IMU rotation matrix to WebGL coordinate system
        let r1 = [0, 1, 0];
        let r2 = [0, 0, 1]; 
        let r3 = [-1, 0, 0];
        
        imu.i.x = (r1[0]*incoming.i_x + r1[1]*incoming.i_y + r1[2]*incoming.i_z);
        imu.i.y = (r2[0]*incoming.i_x + r2[1]*incoming.i_y + r2[2]*incoming.i_z);
        imu.i.z = (r3[0]*incoming.i_x + r3[1]*incoming.i_y + r3[2]*incoming.i_z);

        imu.j.x = (r1[0]*incoming.j_x + r1[1]*incoming.j_y + r1[2]*incoming.j_z);
        imu.j.y = (r2[0]*incoming.j_x + r2[1]*incoming.j_y + r2[2]*incoming.j_z);
        imu.j.z = (r3[0]*incoming.j_x + r3[1]*incoming.j_y + r3[2]*incoming.j_z);

        imu.k.x = (r1[0]*incoming.k_x + r1[1]*incoming.k_y + r1[2]*incoming.k_z);
        imu.k.y = (r2[0]*incoming.k_x + r2[1]*incoming.k_y + r2[2]*incoming.k_z);
        imu.k.z = (r3[0]*incoming.k_x + r3[1]*incoming.k_y + r3[2]*incoming.k_z);

        if (incoming.msg)
        {
            console.log(msg);
            //$("").html(incoming.msg);
        }
    }
    
    function sendServer(message = "") {
        //.socket.send('This data will be sent');
        var outgoing = {
            msg: message
        };
        socket.send(JSON.stringify(outgoing));
    }
    
    window.onload = function(event) {
        initWebSockets();
    }
    </script>
    </html>
)";

#endif