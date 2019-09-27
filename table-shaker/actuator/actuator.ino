#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <SocketIoClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;

int pwm = 0;

void event(const char * payload, size_t length) {
  pwm = atoi(payload);
  USE_SERIAL.printf("got message: %s\n", payload);
}

const char* password = "g2L4}344";
const char* ssid = "DESKTOP-VIUOB7E_5413";

const char* serverIP = "192.168.137.1";
const int port = 3001;

void setup() {
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  analogWrite(D4, 0);
  analogWrite(D3, 0);
    USE_SERIAL.begin(115200);

    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    WiFiMulti.addAP(ssid, password);

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    webSocket.on("change pwm", event);
    webSocket.begin(serverIP, port);
}

unsigned long messageTimestamp = 0;

void loop() {
    webSocket.loop();
    analogWrite(D3, pwm);
    analogWrite(D4, 0);
}
