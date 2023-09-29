String lora_band = "915000000";
String lora_networkid = "18";
String lora_address = "1";
String lora_RX_address = "2";

String textoEntrada;
int count = 0;

void setup() {
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial.begin(115200);

  delay(1500);
  Serial2.println("AT+BAND=" + lora_band);
  delay(500);  
  Serial2.println("AT+NETWORKID=" + lora_networkid);
  delay(500);
  Serial2.println("AT+ADDRESS=" + lora_address);
  delay(1000);
  Serial.println("Inicializado transmisor");
}

void loop() {
  delay(50);
  count = count + 1;

  if(count >= 400){ 
    Serial2.println("AT+SEND="+lora_RX_address+",4,PING" );
    Serial.println("sent");
    count = 0;
  }

  if(Serial2.available()){
    textoEntrada = Serial2.readString();
    Serial.println("entrada: " + textoEntrada);
  }

}