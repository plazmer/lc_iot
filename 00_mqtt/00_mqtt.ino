#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//USE sample_config.h, insert real params and rename to config.h
#include "config.h"

/* DEFINE YOUR PARAMS */
#define mqtt_topic_status   "rtfbest/status"
#define mqtt_topic_in     "rtfbest/led1"
#define mqtt_topic_out    "rtfbest/status"

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
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ( strcmp(topic, mqtt_topic_in) == 0 ) { 
   
    if ((char)payload[0] == '0') {
      digitalWrite(BUILTIN_LED, HIGH); //BUILTIN_LED has pullup resistor, HIGH = OFF, LOW = ON
    }
    
    if ((char)payload[0] == '1') {
      digitalWrite(BUILTIN_LED, LOW);
    }
  }
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
      client.subscribe(mqtt_topic_in);
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
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

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
