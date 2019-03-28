#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//REPLACE IN config.h #defines to YOUR PARAMETERS
#include "config.h"

char msg[50];
int value = 0;
long lastMsg = 0;


int pins[] = {D0, D1, D2, D3, D4, D5, D6, D7, D8};
int pins_len = sizeof(pins) / sizeof(pins[0]);
int pins_status[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int i=0;

String mqtt_topic_out_name;

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
    pins_status[i] = digitalRead(pins[i]); //init
  }
     
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);  
}


void pub_status(bool check_old) {  
  bool send;
  
  for (i=0; i< pins_len; i++) {
    send = true;
    
    mqtt_topic_out_name = mqtt_topic_out;
    mqtt_topic_out_name += String(i);
    
    value = digitalRead(pins[i]);
    if ( check_old ) {
      if (pins_status[i] != value) {
        send = true;
      } else {
        send = false;
      }
    } else {
      send = true;
    }

    if (send) {
      sprintf(msg,"%i", value);
      client.publish(mqtt_topic_out_name.c_str(),msg);      
      sprintf(msg,"PIN %i=%i", i, value);
      Serial.println(msg);
    }
  }
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    pub_status(false); //every 5 second send full
  } else {
    pub_status(true); //send only changes
  }
}
