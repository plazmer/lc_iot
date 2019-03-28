#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//REPLACE IN config.h #defines to YOUR PARAMETERS
#include "config.h"

#define LED D1

long lastMsg = 0;
char msg[50];
int btn_status = LOW;
int btn_status_changed = 0;
int blink = LOW;


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ( strcmp(topic, mqtt_topic_in) == 0 ) { 
    if (payload[0] == '1') {
      digitalWrite(D1, HIGH);
      Serial.println("LED1 HIGH");
    }

    if (payload[0] == '2') {
      digitalWrite(D2, HIGH);
      Serial.println("LED2 HIGH");
    }

    if (payload[0] == '3') {
      digitalWrite(D3, HIGH);
      Serial.println("LED3 HIGH");
    }

    if (payload[0] == '4') {
      digitalWrite(D4, HIGH);
      Serial.println("LED4 HIGH");
    }

    if (payload[0] == '5') {
      digitalWrite(D5, HIGH);
      Serial.println("LED5 HIGH");
    }

    if (payload[0] == '6') {
      digitalWrite(D6, HIGH);
      Serial.println("LED6 HIGH");
    }

    if (payload[0] == '7') {
      digitalWrite(D7, HIGH);
      Serial.println("LED7 HIGH");
    }

    if (payload[0] == '8') {
      digitalWrite(D8, HIGH);
      Serial.println("LED8 HIGH");
    }    
    
    if (payload[0] == '9') {
      digitalWrite(D0, HIGH);
      Serial.println("LED0 HIGH");
    }

    if (payload[0] == '0') {
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      digitalWrite(D3, LOW);
      digitalWrite(D4, LOW);
      digitalWrite(D5, LOW);
      digitalWrite(D6, LOW);
      digitalWrite(D7, LOW);
      digitalWrite(D0, LOW);
      
      Serial.println("LED LOW");
    }
  }
}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D0, OUTPUT);
    
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
}
