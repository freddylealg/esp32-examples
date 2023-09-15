
// Interruptions modes:
// LOW, CHANGE, RISING, FALLING, HIGH

const int green = 14;
const int yellow = 27;
const int red = 26;
const int inputButton = 25;


void IRAM_ATTR onAll(){
  digitalWrite(green, HIGH);
  digitalWrite(yellow, HIGH);
  digitalWrite(red, HIGH);
}


void setup() {
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(inputButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(inputButton), onAll, RISING);

}

void loop() {
  digitalWrite(green, HIGH);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
  delay(2000);
  digitalWrite(green, LOW);
  digitalWrite(yellow, HIGH);
  digitalWrite(red, LOW);
  delay(2000);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  delay(2000);

}
