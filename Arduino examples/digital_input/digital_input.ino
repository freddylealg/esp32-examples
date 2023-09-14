
const int INPUT_BUTTON = 14;
const int OUTPUT_LED = 4;

void setup() {
  pinMode(INPUT_BUTTON, INPUT); // other opcion INPUT_PULLUP or INPUT_PULLDOWN
  pinMode(OUTPUT_LED, OUTPUT);
}

void loop() {
  int state = digitalRead(INPUT_BUTTON);
  digitalWrite(OUTPUT_LED, state);
}
