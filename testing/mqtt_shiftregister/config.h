#define wifi_ssid 		"mqtt"
#define wifi_pass 		"rtf-1234"

#define mqtt_server 		"2.remzalp.ru"
#define mqtt_port  		9976
#define mqtt_login 		"student"
#define mqtt_pass 		"rtf-123"

/* DEFINE YOUR PARAMS */
#define mqtt_topic_status "praw/status"
#define mqtt_topic_in     "praw/led"
#define mqtt_topic_out    "praw/shift"


WiFiClient espClient;
PubSubClient client(espClient);

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

void reconnect() {
  String clientId = String(ESP.getChipId());

  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    Serial.print("Client ID:");
    Serial.println(clientId);

    Serial.print("MQTT: ");    
    Serial.print(mqtt_login);
    Serial.print("/");    
    Serial.println(mqtt_pass);
    
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
