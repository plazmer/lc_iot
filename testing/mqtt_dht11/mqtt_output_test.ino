#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//REPLACE IN config.h #defines to YOUR PARAMETERS
#include "config.h"

const int dataPin = 12;   //Outputs the byte to transfer
const int loadPin = 13;   //Controls the internal transference of data in SN74HC595 internal registers
const int clockPin = 15;  //Generates the clock signal to control the transference of data

long lastMsg = 0;
char msg[50];
int blink = LOW;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  int data = 0;
  if ( strcmp(topic, mqtt_topic_in) == 0 ) { 
    data = atoi( payload[0] ); 
    digitalWrite(loadPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 1 << data); 
    digitalWrite(loadPin, HIGH);  

    Serial.println(String(data));
  }
}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(loadPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
    
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
