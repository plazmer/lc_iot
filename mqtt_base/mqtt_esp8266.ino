#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* wifi_ssid = "REPLACE_YOURS";
const char* wifi_pass = "REPLACE_YOURS";
const char* mqtt_server = "REPLACE_YOURS";
const int   mqtt_port = 1883; //REPLACE_YOURS!!!
const char* mqtt_login = "REPLACE_YOURS";
const char* mqtt_pass = "REPLACE_YOURS";

const char* mqtt_topic_hearbeat = "REPLACE_YOURS";
const char* mqtt_topic_in = "REPLACE_YOURS";
const char* mqtt_topic_out = "REPLACE_YOURS";


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
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);
  } else {
    digitalWrite(BUILTIN_LED, HIGH);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX); //TODO: unique client id based on serial number - ESP.getChipId()
    if (client.connect(clientId.c_str()), mqtt_login,mqtt_pass) {
      Serial.println("connected");
      client.publish(mqtt_topic_hearbeat, "reconnected");
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
  myservo.attach(5);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
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
  if (now - lastMsg > 2000) {    //TODO: overflow long in 49 days
    lastMsg = now;
    ++value;
    snprintf (msg, 50, "heartbeat #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_topic_heartbeat, msg);
  }
}
