#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "DHT.h"

#define DHTPIN 2 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

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
  WiFi.begin(wifi_ssid, wifi_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
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

/*EXAMPLE*/
  if ( strcmp(topic, mqtt_topic_in) == 0 ) { 
   
    if ((char)payload[0] == '0') {
      digitalWrite(BUILTIN_LED, HIGH);
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
    if (client.connect("plazmer", "student", "rtf-123")) {  //TODO: use variables 
      Serial.println("connected");
      client.publish(mqtt_topic_heartbeat, "reconnected");
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
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  if (now - lastMsg > 1000) {    //TODO: overflow long in 49 days
  
    float h2 = dht.readHumidity();
    float t2 = dht.readTemperature();
    float t_out=t2-t1;
    if (t_out>2){
    client.publish(mqtt_topic_temp, t_out)
    }
  
    lastMsg = now;
    ++value;
    snprintf (msg, 50, "heartbeat #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_topic_heartbeat, msg);
  }
}
