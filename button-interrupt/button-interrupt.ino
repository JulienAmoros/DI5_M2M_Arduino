const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  3;      // the number of the LED pin

// variables will change:
volatile int buttonState = 0;         // variable for reading the pushbutton status
int led_status = 0;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // Attach an interrupt to the ISR vector
  attachInterrupt(0, pin_ISR, FALLING);
}

void loop() {
  delay(2000);
}

void pin_ISR() {
  buttonState = digitalRead(buttonPin);
  if(led_status == 0){
    digitalWrite(ledPin, HIGH);
    led_status = 1;
  }
  else{
    digitalWrite(ledPin, LOW);
    led_status = 0;
  }
}
