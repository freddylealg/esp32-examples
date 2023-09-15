int i_limit = 50;
int i_lectura;
int led = 2;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  i_lectura = touchRead(4);
  if (i_lectura < i_limit) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
  delay(100);

}
