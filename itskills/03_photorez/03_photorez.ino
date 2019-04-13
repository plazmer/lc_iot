#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "functions.h"


#define BEEP 4
int  delayMS = 10000;
long lastMsgMQTT = 0;
long lastLED = 0;
char msg[200];
int value = 0;


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ( strcmp(topic, mqtt_topic_in) == 0 ) { 
   
    if ((char)payload[0] == '1') {
      tone(BEEP,440,300);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if ( millis() - lastMsgMQTT > delayMS) {
    int luminocity = 1024 - analogRead(A0);
    Serial.print(F("luminocity: "));
    Serial.println(luminocity);
    
    String tpl = "{\"data\": {\"luminocity\": " + String(luminocity) + "},\"status\":{\"devEUI\": \"" + clientId + "\"}}";
    //sprintf(msg, tpl.c_str(), event.relative_humidity);
    client.publish(mqtt_topic_out, tpl.c_str());
    Serial.println(tpl);      
    
    lastMsgMQTT = millis();
  }    
}
