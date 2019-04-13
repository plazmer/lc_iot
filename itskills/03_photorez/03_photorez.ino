#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "functions.h"

int  delayMS = 10000;
long lastMsgMQTT = 0;
long lastLED = 0;
char msg[200];
int value = 0;


void callback(char* topic, byte* payload, unsigned int length) {
//no reaction
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
    int luminocity = 1023 - analogRead(A0);
    Serial.print(F("luminocity: "));
    Serial.println(luminocity);
    
    String tpl = "{\"data\": {\"luminocity\": " + String(luminocity) + "},\"status\":{\"devEUI\": \"" + clientId + "\"}}";
    //sprintf(msg, tpl.c_str(), event.relative_humidity);
    client.publish(mqtt_topic_out, tpl.c_str());
    Serial.println(tpl);      
    
    lastMsgMQTT = millis();
  }    
}
