/*
 * WebSocketClientSocketIO.ino
 *
 *  Created on: 06.06.2016
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

const char* password = "g2L4}344";
const char* ssid = "DESKTOP-VIUOB7E 5413";

const char* serverIP = "192.168.137.1";
const int port = 3001;

void setup() {
    Serial.begin(115200);

    for(uint8_t t = 4; t > 0; t--) {
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

    // server address, port and URL
    socketIO.begin(serverIP, port);
}

unsigned long messageTimestamp = 0;
void loop() {
    socketIO.loop();
    
    uint64_t now = millis();
    String bb = Serial.readStringUntil('\n');
    if(now - messageTimestamp > 2000) {
        messageTimestamp = now;

        // creat JSON message for Socket.IO (event)
        DynamicJsonDocument doc(1024);
        JsonArray array = doc.to<JsonArray>();
        
        // add evnet name
        // Hint: socket.on('event_name', ....
        array.add("sens");

        // add payload (parameters) for the event
        JsonObject param1 = array.createNestedObject();
        param1["now"] = now;
        param1["value"] = bb;

        // JSON to String (serializion)
        String output;
        serializeJson(doc, output);

        // Send event        
        socketIO.sendEVENT(output);
    }
}
