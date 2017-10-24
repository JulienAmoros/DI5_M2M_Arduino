#define LED1_PIN 13

void setup(void){
  Serial.begin(9600);
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  Serial.println("Lancement de l'application");
  //lcd.begin(16, 2);
}

void loop(void){
  digitalWrite(LED1_PIN, HIGH);
  Serial.println("JOUR!");
  delay(1000);
  digitalWrite(LED1_PIN, LOW);
  Serial.println("Nuit.");
  delay(1000);
}
