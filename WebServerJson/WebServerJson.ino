/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"
#include <stdio.h>
#include <string.h>

#define PinDHT A1     //connecte sur A1
#define PIN_LED 4

#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(PinDHT, DHTTYPE);

void printLine(int analogChannel, double sensorReading);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xA9, 0xB3 };
//IPAddress ip(192,168,0,1);

IPAddress ip(169,254,1,1);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// Peripherals name
char* names[] = {"potentiometre", "humidity", "temperature", "", "", ""};

EthernetClient client;

void setup() {
 // Initialise Temp sensor
 dht.begin();
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // Light on LED
  pinMode(PIN_LED, OUTPUT);
  digitalWrite[PIN_LED, HIGH);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  client = server.available();

  char str[1000];
  int i = 0;

  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          respond(str);
          i=0;
          break;
        }
        if (c == '\n') {
          //strcat(str, c);
          str[i++] = c;
          str[i++] = 0;
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          //strcat(str, c);
          str[i++] = c;
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void printLine(int analogChannel, double sensorReading){
  client.print("\t\"");
  client.print(names[analogChannel]);
  client.print("\":");
  client.print(sensorReading);
  client.println(",");
}

void respond(char * str){
  Serial.write(str);
  if(str[0]=='G'){
    Serial.write("Answering to GET - Standard answer");
  // send a standard http response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println();
    client.println("{");
    
    printLine(0, analogRead(0));
    printLine(1, dht.readHumidity());
    printLine(2, dht.readTemperature());

    client.println("\t\"date\":0");
    client.println("}");
  }
  else if(str[0] == 'P'){
    
  }
  else{
    Serial.println("Invalid request");
  }
}

