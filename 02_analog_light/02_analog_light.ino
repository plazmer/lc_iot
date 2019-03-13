#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

/* DEFINE YOUR PARAMS */
#define mqtt_topic_status     "rtfbest/status"
#define mqtt_topic_out        "rtfbest/alight"
#define mqtt_topic_out_pwm    "rtfbest/alight_pwm"
#define mqtt_topic_in         "rtfbest/alight_pwm_override"
#define LED                   D2

long lastMsgMQTT = 0;
long lastMsgPWM = 0;
int value = 0;
int value_PWM = 0;
bool is_override = false;

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
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");  
  
  if (length > sizeof(msg) - 1) {
    length = sizeof(msg) - 1;
  }
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg[i] = (char) payload[i];
  }  
  Serial.println();
  Serial.println(msg);

  value_PWM = atoi(msg);
  if ( value_PWM > 1023 ) {
    value_PWM = 0;    
  } else {
    lastMsgPWM = millis();
    is_override = true;
  }  

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

  if ( is_override ) {
    if ( millis() > lastMsgPWM + 10*1000 ) { //timeout 10 second
      is_override = false; 
    } else {
      analogWrite(LED, value_PWM);
      sprintf (msg, "%i", value_PWM);
      client.publish(mqtt_topic_out, msg);
      Serial.print("OVERIDE ");
      Serial.println(msg);
  
    }
  } else {
    if ( millis() > lastMsgMQTT + delayMS) {
      value = analogRead(A0);
      sprintf (msg, "%i", value);
      client.publish(mqtt_topic_out, msg);
      
      Serial.print("LIGHT ");
      Serial.println(msg);
  
      lastMsgMQTT = millis();
  
      value = 1024-value;
      sprintf (msg, "%i", value);
      Serial.print("PWM ");
      Serial.print(msg);
      Serial.print("/");
      Serial.println(value_PWM);
  
      client.publish(mqtt_topic_out_pwm, msg);      
      analogWrite(LED, value);    
    }
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
