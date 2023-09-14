
const int INPUT_PIN = 34;
int analog_value = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  analog_value = analogRead(INPUT_PIN);
  Serial.println(analog_value);
  delay(1000);
}
