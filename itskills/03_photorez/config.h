#define wifi_ssid     "Class1"
#define wifi_pass     "Klass300d76"

#define mqtt_server     "2.remzalp.ru"
#define mqtt_port     9976
#define mqtt_login    "student"
#define mqtt_pass     "rtf-123"

#define mqtt_topic_status  "itskills/status"
#define mqtt_topic_out     "itskills/sensor"
#define mqtt_topic_in      "itskills/command"

WiFiClient espClient;
PubSubClient client(espClient);
String clientId = String(ESP.getChipId());
