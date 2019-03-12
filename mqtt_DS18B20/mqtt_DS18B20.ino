#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

OneWire ds(4);
long lastUpdateTime = 0;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

char msgBuffer[20];
float temperature;

int detectTemperature(){
  byte data[2]; // Место для значения температуры
  ds.reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
  ds.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство 
  ds.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память
  delay(500); // Микросхема измеряет температуру, а мы ждем.  
  ds.reset(); // Теперь готовимся получить значение измеренной температуры
  ds.write(0xCC); 
  ds.write(0xBE); // Просим передать нам значение регистров со значением температуры
  // Получаем и считываем ответ
  data[0] = ds.read(); // Читаем младший байт значения температуры
  data[1] = ds.read(); // А теперь старший
  // Формируем итоговое значение: 
  //    - сперва "склеиваем" значение, 
  //    - затем умножаем его на коэффициент, соответсвующий разрешающей способности (для 12 бит по умолчанию - это 0,0625)
  float temperature =  ((data[1] << 8) | data[0]) * 0.0625;
  // Выводим полученное значение температуры в монитор порта
  Serial.println(temperature);
  return temperature;
  }

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
    snprintf (msg, 50, "heartbeat #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_topic_heartbeat, msg);
  }

   temperature = detectTemperature();
   client.publish(mqtt_topic_out, dtostrf(temperature, 6, 2, msgBuffer));
}
