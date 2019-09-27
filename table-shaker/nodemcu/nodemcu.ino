#include <Wire.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SocketIoClient.h>

// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;

// Select SDA and SCL pins for I2C communication 
const uint8_t scl = D5;
const uint8_t sda = D6;

// sensitivity scale factor respective to full scale setting provided in datasheet 
const uint16_t AccelScaleFactor = 16384;
//const uint16_t AccelScaleFactor = 5461.333333333333;
const uint16_t GyroScaleFactor = 131;

int16_t AccelX, AccelY, AccelZ;
const uint8_t buzzer = D8;

SocketIoClient socketIO;

const char* password = "g2L4}344";
const char* ssid = "DESKTOP-VIUOB7E_5413";

const char* serverIP = "192.168.137.1";
const int port = 3001;

const int sampleLength = 100;
float mZeroPoint = 0;

boolean isCalibrate = false;

void onCalibrate(const char *payload, size_t length)
{
  Serial.printf("got message: %s\n", payload);
  isCalibrate = false;
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("ESP_Sensor");
  WiFi.begin(ssid, password);
  tryingConnection();
  // server address, port and URL
  socketIO.begin(serverIP, port);
  socketIO.on("calibrate", onCalibrate);
  Wire.begin(sda, scl);
  MPU6050_Init();
}

unsigned long messageTimestamp = 0;

void loop() {
  tryingConnection();
  socketIO.loop();
  uint64_t now = millis();
  if(isCalibrate == false){
    delay(200);
    calibrateMagnitude();
    isCalibrate = true;
  }
  double Ax, Ay, Az;
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  
  //divide each with their sensitivity scale factor
  Ax = (double)AccelX/AccelScaleFactor;
  Ay = (double)AccelY/AccelScaleFactor;
  Az = (double)AccelZ/AccelScaleFactor;
  float Accl = akselerasi(Ax, Ay, Az),
        M = magnitude(Accl);
//  Serial.print(M); Serial.print("\t");
//  Serial.print(M - mZeroPoint); Serial.print("\t");
  
//  Serial.print(AccelX); Serial.print("\t");
//  Serial.print(AccelY); Serial.print("\t");
//  Serial.print(AccelZ); Serial.print("\t");
//  Serial.println();
  if(now - messageTimestamp > 128) {
    messageTimestamp = now;
    String a = "\"";
    String b = a+(M-mZeroPoint)+a;
    char c[64];
    b.toCharArray(c, 32);
    socketIO.emit("sens",c);
  }
  if((M-mZeroPoint) >= 0.5) tone(buzzer, 440);
  else noTone(buzzer);
}
