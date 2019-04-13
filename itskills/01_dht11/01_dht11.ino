#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "functions.h"

//<DHT11>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 3     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
//</DHT11>

#define LED 		   16
int  delayMS = 10000;
long lastMsgMQTT = 0;
long lastLED = 0;
char msg[200];
int value = 0;


void callback(char* topic, byte* payload, unsigned int length) {
//no reaction
}


void setup() {
  pinMode(LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  //DHT11
  sensor_t sensor;
  dht.begin();  
  dht.humidity().getSensor(&sensor);
  //delayMS = sensor.min_delay / 1000;  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if ( millis() - lastMsgMQTT > delayMS) {
    Serial.println(millis());
    char buf [10];    
    sensors_event_t event;
    
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    } else {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));

      String tpl = "{\"data\": {\"humidity\": " + String(event.relative_humidity) + "},\"status\":{\"devEUI\": \"" + clientId + "\"}}";
      //sprintf(msg, tpl.c_str(), event.relative_humidity);
      client.publish(mqtt_topic_out, tpl.c_str());
      Serial.println(tpl);      
    }    
    
    lastMsgMQTT = millis();
  }
}
