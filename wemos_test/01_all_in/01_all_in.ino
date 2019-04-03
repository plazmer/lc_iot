int pins[] = {D0,D1,D2,D3,D4,D5,D6,D7,D8};
int pins_length = sizeof(pins) / sizeof(pins[0]);
int i=0;
String stats;

void setup() {
  Serial.begin(115200);
  for(i=0;i<pins_length;i++) pinMode(pins[i],INPUT);
}

void loop() {
  stats = "";
  for(i=0;i<pins_length;i++) {
    stats += digitalRead(pins[i]);
  }
  Serial.println(stats);
  delay(100);
}
