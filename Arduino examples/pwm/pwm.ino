
const int INPUT_PIN = 34;
int analog_value = 0;

const int OUTPUT_PIN = 18;
const int frecuencia = 5000;
const int canal = 0;
const int resolucion = 8;


void setup() {
  ledcSetup(canal, frecuencia, resolucion);
  ledcAttachPin(OUTPUT_PIN, canal);
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  analog_value = analogRead(INPUT_PIN);
  Serial.println(analog_value);
  Serial.println(analog_value/16);
  ledcWrite(canal, analog_value/16);
  delay(1000);
}
