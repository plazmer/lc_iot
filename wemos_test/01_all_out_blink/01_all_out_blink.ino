int pins[] = {D0,D1,D2,D3,D4,D5,D6,D7,D8};
int pins_length = sizeof(pins) / sizeof(pins[0]);
int i=0;

void setup() {
  Serial.begin(115200);
  for(i=0;i<pins_length;i++) pinMode(pins[i],OUTPUT);
}

void loop() {    
  for(i=0;i<pins_length;i++) {
    digitalWrite(pins[i], HIGH);
  }
  delay(300);

  for(i=0;i<pins_length;i++) {
    digitalWrite(pins[i], LOW);
  }
  delay(300);
}
