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
 modified 02 Sept 2015
 by Arturo Guadalupi

 */

#include <SPI.h>
#include <Ethernet.h>

char * getHttpWord(char * head);
char * getRessource(char * head);
char * getProtocolVersion(char * head);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10,0,0,2);

EthernetClient client;

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  client = server.available();
  String head;
  String body;
  
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        
        // HTTP request end of header
        if (c == '\n' && currentLineIsBlank) {
          head = head.substring(0, head.length()-1);
          
          // Put content string in body table
          while (client.available()) {
            body += (char) client.read();
            //Serial.write(client.read());
          }
          
          // Process and send proper response
          respond(head, body);

          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          head += c;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
          head += c;
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

void respond(String head, String body){
  String http_word = getHttpWord(head);
  String ressource = getRessource(head);
  String protocol_version = getProtocolVersion(head);
  
  if(http_word == "POST"){
    doPost(head, body);
  } else if(http_word == "GET"){
    doGet(head, body);
  } else if(http_word == "PUT"){
    doPut(head, body);
  }else if(http_word == "DELETE"){
    doDelete(head, body);
  }else {
    Serial.print("Error: HTTP word ");
    Serial.print(http_word);
    Serial.println(" is not recognised.");
    
    client.println("HTTP/1.1 400 Bad Request");
    client.println("Connection: close");
  }
}

// Get the fisrt word of param (HTTP word if param is request's head)
String getHttpWord(String head){
  return head.substring(0, head.indexOf(' '));
}

// Get the 2nd word of param (ressource if param is request's head)
String getRessource(String head){
  int index_2nd_space = head.indexOf(' ', head.indexOf(' ')+1);
  
  return head.substring(head.indexOf(' ')+1, index_2nd_space);
}

// Get the 3rd word of param (Protocol/version if param is request's head)
String getProtocolVersion(String head){
  int index_2nd_space = head.indexOf(' ', head.indexOf(' ')+1);
  int index_endline = head.indexOf('\n');
  
  return head.substring(index_2nd_space+1, index_endline);
}

// Get the value of a particular key in the head (non case-sensitive)
String getHeadValueOf(String head, String key){
  head.toLowerCase();
  key.toLowerCase();
  
  int key_start = head.indexOf(key);
  int key_end = head.indexOf(' ', key_start);
  
  if(key_start < 0){
    return "";
  }
  
  int value_start = key_end;
  int value_end = head.indexOf('\n', value_start);
  
  String value = head.substring(value_start, value_end);
  value.trim();
  
  return value;
}

// Get value of parameters in body
String getBodyValue(String body, String key){
  body.toLowerCase();
  key.toLowerCase();
  
  int start_key = body.indexOf(key);
  int start_value = body.indexOf('=', start_key);
  int end_value = body.indexOf('&', start_key);
  
  return body.substring(start_value+1, end_value);
}

// Describe here what to do when POST received
void doPost(String head, String body){
  int red = getBodyValue(body, "red").toInt();
  int green = getBodyValue(body, "green").toInt();
  int blue = getBodyValue(body, "blue").toInt();
  
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);
  
  // Light RGB Led
  
  client.println("HTTP/1.1 200 OK");
  client.println("Connection: close");
}

// Describe here what to do when GET received
void doGet(String head, String body){
  String accepting = getHeadValueOf(head, "accept");
  Serial.println(accepting);
  if(accepting.indexOf("json") >= 0){
    // Send JSON response
    
  }else{
    // Send HTML response
    
  }
  
  // TODO: put next on HTML part up here
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  // output the value of each analog input pin
  for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
    int sensorReading = analogRead(analogChannel);
    client.print("analog input ");
    client.print(analogChannel);
    client.print(" is ");
    client.print(sensorReading);
    client.println("<br />");
  }
  client.println("</html>");
}

// Describe here what to do when POST received
void doPut(String head, String body){
  Serial.println("Nothing implemented on PUT request");
}

// Describe here what to do when DELETE received
void doDelete(String head, String body){
  Serial.println("Nothing implemented on DELETE request");
}

