#include "I2Cdev.h"
#include "MPU6050.h"

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

#include <Hash.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro;

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

int16_t sx[2], sy[2], sz[2],
      ox, oy, oz;

const char* password = "g2L4}344";
const char* ssid = "DESKTOP-VIUOB7E 5413";

const char* serverIP = "192.168.137.1";
const int port = 3001;

#define USE_SERIAL Serial

void setup() {
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    // disable AP
    if(WiFi.getMode() & WIFI_AP) {
        WiFi.softAPdisconnect(true);
    }

    WiFiMulti.addAP(ssid, password);

    //WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    String ip = WiFi.localIP().toString();
    USE_SERIAL.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

    // server address, port and URL
    socketIO.begin(serverIP, port);

    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    
    sx[0] = accelgyro.getAccelerationX();
    sy[0] = accelgyro.getAccelerationY();
    sz[0] = accelgyro.getAccelerationZ();
    sx[1] = accelgyro.getAccelerationX();
    sy[1] = accelgyro.getAccelerationY();
    sz[1] = accelgyro.getAccelerationZ();
    
    for(int i = 0; i< 1000; i++) {
      int16_t x = accelgyro.getAccelerationX(),
              y = accelgyro.getAccelerationY(),
              z = accelgyro.getAccelerationZ();
      sx[0] = max(x, sx[0]);
      sy[0] = max(y, sy[0]);
      sz[0] = max(z, sz[0]);
      sx[1] = min(x, sx[1]);
      sy[1] = min(y, sy[1]);
      sz[1] = min(z, sz[1]);
      delay(10);
    }
    ox = (sx[0] + sx[1]) / 2;
    oy = (sy[0] + sy[1]) / 2;
    oz = (sz[0] + sz[1]) / 2;
}

unsigned long messageTimestamp = 0;
void loop() {
    socketIO.loop();
    uint64_t now = millis();
    
    float comp = 2048,
          ax = (accelgyro.getAccelerationX() - ox) / comp,
          ay = (accelgyro.getAccelerationY() - oy) / comp,
          az = (accelgyro.getAccelerationZ() - oz) / comp,
          accl = akselerasi(ax, ay, az),
          M = magnitude(accl);

//    if(now - messageTimestamp > 128) {
//        messageTimestamp = now;
//
//        // creat JSON message for Socket.IO (event)
//        DynamicJsonDocument doc(1024);
//        JsonArray array = doc.to<JsonArray>();
//        
//        // add evnet name
//        // Hint: socket.on('event_name', ....
//        array.add("sens");
//
//        // add payload (parameters) for the event
//        JsonObject param1 = array.createNestedObject();
//        param1["now"] = now;
//        param1["value"] = M;
//
//        // JSON to String (serializion)
//        String output;
//        serializeJson(doc, output);
//
//        // Send event        
//        socketIO.sendEVENT(output);
//    }
}


float akselerasi(float x, float y, float z) {
  float x_2 = pow(x,2),
          y_2 = pow(y,2),
          z_2 = pow(z,2);
  return sqrt(x_2 + y_2);
}
float magnitude(float a) {
  return (log(a) + 1.6 * log(30));
}

