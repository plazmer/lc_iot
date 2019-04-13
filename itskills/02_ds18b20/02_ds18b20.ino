#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "functions.h"

//<DS18B20>
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(4);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

int temperature = 0;
float tempC;
//</DS18B20>

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

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if ( millis() - lastMsgMQTT > delayMS) {
    Serial.println(millis());
    
    Serial.print(" Requesting temperatures..."); 
    sensors.requestTemperatures(); // Send the command to get temperature readings 
    Serial.println("DONE"); 
    /********************************************************************/
    Serial.print("Temperature is: "); 
    temperature = sensors.getTempCByIndex(0);
    Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
    
    Serial.print(F("Temperature: "));
    Serial.println(temperature);
    
    String tpl = "{\"data\": {\"temperature\": " + String(temperature) + "},\"status\":{\"devEUI\": \"" + clientId + "\"}}";
    //sprintf(msg, tpl.c_str(), event.relative_humidity);
    client.publish(mqtt_topic_out, tpl.c_str());
    Serial.println(tpl);      
    
    lastMsgMQTT = millis();
  }    
}
