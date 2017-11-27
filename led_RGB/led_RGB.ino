#include "ChainableLED.h"

#define NUM_LEDS  5
//bme280 capteur température pression et humidite
//Led branché sur D7
ChainableLED leds(7, 8, NUM_LEDS);

void setup()
{
  Serial.begin(9600);
  //initialisation de la LED
  leds.init();
    Serial.println("Lancement de l'application");
}


int sensorPin = A0;    // Potentionmetre A0
double sensorValue = 0;  // variable to store the value coming from the sensor
double rgbValue=0.0;
float teinte = 0.0;
boolean augmente = true;


void loop()
{
  sensorValue =(double) analogRead(sensorPin)/1024;
  
  rgbValue=(double) ((double) analogRead(sensorPin)/1024)*255;
  Serial.println(sensorValue); 
  //POur chaque Led
  for (byte i=0; i<NUM_LEDS; i++)
  {
    //Numero de la led, teinte, saturation, luminosité
    //leds.setColorHSB(i, sensorValue,0.7, 0.49);
    leds.setColorRGB(i,rgbValue,rgbValue,rgbValue);
    //leds.setColorHSB(i, teinte,0.7, 0.49);
  }
  delay(500);
    
  /*if (augmente)
  {
    teinte+= 0.05;
  }
  else
  {
    teinte-= 0.05;
  }  
  if (teinte>=1 && augmente)
  {
    augmente = false;
  }
  else if (teinte<=0 && augmente==false)
  {
   augmente = true;
  }*/
}
