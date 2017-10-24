int sensorPin = A0;    // Potentionmetre A0
int ledPin = 3;      // LED D3
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  Serial.begin(9600);
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);  
  digitalWrite(ledPin, HIGH);
  Serial.println("Lancement de l'application");
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);    
  // Write analogic value
  analogWrite(ledPin, sensorValue/4);  
  //Serial.println(sensorValue);         
}
