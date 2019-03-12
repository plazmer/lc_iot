#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

//<DHT11>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN D2     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;
long lastMsgMQTT = 0;
//</DHT11>

/* DEFINE YOUR PARAMS */
#define mqtt_topic_status  "rtfbest/status"
#define mqtt_topic_temp    "rtfbest/temperature"
#define mqtt_topic_humi    "rtfbest/humidity"

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  Serial.println(wifi_pass);
  WiFi.begin(wifi_ssid, wifi_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
    WiFi.printDiag(Serial);
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
//no reaction
}

void reconnect() {
  String clientId = "ESP8266Client-";
  clientId += String(ESP.getChipId());

  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    Serial.print("Client ID:");
    Serial.print(clientId);
    Serial.print(mqtt_login);
    Serial.print(mqtt_pass);
    if (client.connect(clientId.c_str(), mqtt_login, mqtt_pass)) {  
      Serial.println("connected");
      client.publish(mqtt_topic_status, "reconnected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  //WARNING: BUILTIN_LED has pullup resistor, HIGH = OFF, LOW = ON
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  //<DHT11>
  sensor_t sensor;
  dht.begin();  
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;  
  //</DHT11>
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  if ( millis() > lastMsgMQTT + delayMS) {
    char buf [10];    
    sensors_event_t event;
    
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    } else {
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("°C"));

      sprintf (buf, "%f", event.temperature);     
      client.publish(mqtt_topic_temp, buf);
      Serial.println(buf);
    }
    
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    } else {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
      
      sprintf (buf, "%f", event.relative_humidity);     
      client.publish(mqtt_topic_humi, buf);
      Serial.println(buf);
    }    

    lastMsgMQTT = millis();
  }


  long now = millis();
  if (now - lastMsg > 30000) {    //TODO: overflow long in 49 days
    lastMsg = now;
    ++value;
    snprintf (msg, sizeof(msg), "status #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_topic_status, msg);
  }
}
