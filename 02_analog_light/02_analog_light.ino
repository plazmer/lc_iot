#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

/* DEFINE YOUR PARAMS */
#define mqtt_topic_status  "rtfbest/status"
#define mqtt_topic_out    "rtfbest/alight"
long lastMsgMQTT = 0;
int value = 0;

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
long delayMS = 1000;
char msg[50];



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


  if ( millis() > lastMsgMQTT + delayMS) {
    value = analogRead(A0);
    sprintf (msg, "%i", value);
    client.publish(mqtt_topic_out, msg);
    Serial.println(msg);

    lastMsgMQTT = millis();
    }

  long now = millis();
  if (now - lastMsg > 30000) {    //TODO: overflow long in 49 days
    lastMsg = now;
    ++value;
    sprintf(msg, "status #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_topic_status, msg);
  }
}
