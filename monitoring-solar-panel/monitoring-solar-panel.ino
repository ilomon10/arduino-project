#include <DHT.h>
#include "ACS712.h"

#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM900
SoftwareSerial mySerial(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8
int pinTegangan = A5;
int pinArus = A4;
int pinTemperatur = A0;
float tegangan,arus, temperatur;
DHT asd(pinTemperatur, DHT11);

ACS712 arusACS(ACS712_05B, pinArus);

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);

  Serial.println("Initializing...");
  asd.begin();
  delay(1000);

  initSIM();
  Serial.println("Callibrating...");
  int zero = arusACS.calibrate();
  Serial.println("Done!");
}

void loop()
{
  delay(5000);
  tegangan = bacaTegangan(pinTegangan);
  float aa = arusACS.getCurrentDC();
  arus = aa;
  temperatur = asd.readTemperature();
  Serial.print("Tegangan :");
  Serial.print(tegangan);
  Serial.print("| Arus :");
  Serial.print(arus);
  Serial.print("| Temperature :");
  Serial.println(temperatur);
  sendData(tegangan, arus, temperatur);
}

void initSIM() {
  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial(500);
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial(500);
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial(500);
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial(500);
  mySerial.println("AT+CGATT=1");
  updateSerial(500);
}

void updateSerial(int d)
{
  delay(d);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void sendData(float voltage, float current, float temp) {
  Serial.println("Sending data.");
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(500);
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"\"");
  delay(500);
  mySerial.println("AT+SAPBR=1,1");
  delay(500);
  mySerial.println("AT+SAPBR=2,1");
  delay(3000);
  mySerial.println("AT+HTTPINIT");
  delay(500);
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  delay(500);
  
  mySerial.print("AT+HTTPPARA=\"URL\",\"http://us-central1-monitoring-panel-surya.cloudfunctions.net/addData?");
  mySerial.print("current=");
  mySerial.print(current);
  mySerial.print("&voltage=");
  mySerial.print(voltage);
  mySerial.print("&temperature=");
  mySerial.print(temp);
  mySerial.println("\"");
  delay(3000);
  
  mySerial.println("AT+HTTPACTION=0");
  delay(15000);
  mySerial.println("AT+HTTPTERM");
  delay(500);
  mySerial.println("AT+SAPBR=0,1");
  delay(500);
  Serial.println("Finish.");
}

float bacaTegangan(int pin) {
float R1 = 30000.0;
float R2 = 7500.0;
  float value = analogRead(pin);
  float vOUT = (value * 5.0) / 1024.0;
  float vIN = vOUT / (R2/(R1+R2));
  delay(500);
  return vIN;
}

int bacaArus(int pin) {
  int nilaiadc = 0;
  const int teganganoffset = 25;
  double tegangan = 0.0;
  double nilaiarus = 0.0;
  const int sensitivitas = 66;
  nilaiadc = analogRead(pin);
  tegangan = (nilaiadc / 1024.0) * 5000;
  Serial.println(tegangan);
  nilaiarus = ((tegangan - teganganoffset) / sensitivitas);
  return nilaiarus;
}
