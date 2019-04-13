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

  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    Serial.print("Client ID:");
    Serial.print(clientId);
    Serial.print(mqtt_login);
    Serial.print(mqtt_pass);
    if (client.connect(clientId.c_str(), mqtt_login, mqtt_pass)) {  
      Serial.println("connected");
      client.publish(mqtt_topic_status, clientId.c_str());
      client.subscribe(mqtt_topic_in);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
