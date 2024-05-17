#include <ESP8266WiFi.h>

#include "settings.h"

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

// Get settings from settings file
const char* ssid1 = STASSID1;
const char* password1 = STAPSK1;
const char* host = STAHOST;
const String sensorid = STASENSORID;
const String message = STAMESSAGE;
const String token = STATOKEN;
const uint16_t port = STAPORT;
const String HTTP_METHOD = "POST";
const String PATH_NAME = "/savelog";
// Set other consts
const int TIME_TO_WAIT_AFTER_TRIGGER = 30000;
const int LOOP_DELAY = 500;
const int SENSE_PIN = D2;

// Set globals
WiFiClient client;
int prevState = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SENSE_PIN, INPUT);
  prevState = digitalRead(SENSE_PIN);
  blinkTimes(3, 250);
  delay(2000);
  blinkTimes(5, 500);
  delay(2000);
  blinkTimes(10, 750);
  delay(2000);
  
  WiFi.mode(WIFI_STA); // Sets wifi to station mode
  WiFi.enableInsecureWEP();
  WiFi.begin(ssid1, password1);
    
  WiFiConnect();

  Serial.println("Other settings:");
  Serial.println(String(host));
  Serial.println(String(port));
  Serial.println(String(sensorid));
  Serial.println(String(message));
  Serial.println(String(token));

  delay(500);
}

void loop()
{
  int currentState = digitalRead(SENSE_PIN);
  digitalWrite(LED_BUILTIN, currentState == 1 ? HIGH : LOW);  
  
  if (prevState != currentState) {
      
      Serial.println("Change Detected, fire all the stuff here, pausing for a while, not sure the amount of time yet");
      WiFiConnect();
      if (client.connect(host, 54322)) {

      prevState = currentState;

      Serial.println("sending request");
      client.println(HTTP_METHOD + " " + PATH_NAME + " HTTP/1.1");
      client.println("Host: " + String(host));
      client.println("Connection: close");
      client.println();  // end HTTP header

      // send HTTP body
      client.println("sensor_id=" + sensorid + 
                     "&message=" + message + 
                     "&token=" + token);

      client.println("Content-Type: application/x-www-form-urlencoded");


      while(client.connected()){
        if(client.available()){
          char c = client.read();
          Serial.print(c);
        }
      }
      Serial.println("request sent");
      client.stop();
      Serial.println("disconnected");
      blinkTimes(5, 500);
      delay(TIME_TO_WAIT_AFTER_TRIGGER); 
    } else{
      Serial.println("Connection failed");
      blinkTimes(10, 750);
      return;
    }
  }
}

void blinkTimes(int blinkCount, int timeDelay){
  for(int i = 0; i < blinkCount; i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(timeDelay);
    digitalWrite(LED_BUILTIN, LOW);
    delay(timeDelay);
  }
}

void WiFiConnect(){
  Serial.print("Wait for WiFi... ");
  Serial.print("WiFi.status(): ");
  Serial.print(WiFi.status());

  while (WiFi.status() != WL_CONNECTED) {
      WiFi.printDiag(Serial);
      delay(3500);
    }
    blinkTimes(3, 250);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}