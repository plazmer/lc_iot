#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//REPLACE IN config.h #defines to YOUR PARAMETERS
#include "config.h"

char msg[50];
int value = 0;

int pins[] = {D0, D1, D2, D3, D4, D5, D6, D7, D8};
int pins_len = sizeof(pins) / sizeof(pins[0]);

int i=0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();  
}


void setup() {  
  for (i=0; i< pins_len; i++) {
    pinMode(pins[i], INPUT);  
  }
     
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

  for (i=0; i< pins_len; i++) {
    value = digitalRead(pins[i]);    
    sprintf(msg,"%i", value);
    client.publish(mqtt_topic_out,msg);
    
    sprintf(msg,"PIN %i=%i", i, value);
    Serial.println(msg);
  }
}
