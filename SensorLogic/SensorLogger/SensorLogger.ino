#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include "settings.h"

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

// Get settings from settings file
const char* ssid1 = STASSID1;
const char* password1 = STAPSK1;
const char* ssid2 = STASSID2;
const char* password2 = STAPSK2;
const char* host = STAHOST;
const char* sensorid = STASENSORID;
const char* message = STAMESSAGE;
const char* token = STATOKEN;
const uint16_t port = STAPORT;
// Set other consts
const int TIME_TO_WAIT_AFTER_TRIGGER = 30000;
const int LOOP_DELAY = 500;
const int SENSE_PIN = D2;

// Set globals
ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
int prevState = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);
  
  pinMode(LED_BUILTIN, OUTPUT);
  // Set sensor pin to input and get initial value
  pinMode(SENSE_PIN, INPUT);
  prevState = digitalRead(SENSE_PIN);

   // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid1, password1);
  // WiFiMulti.addAP(ssid1, password1);
  // WiFiMulti.addAP(ssid2, password2);
  
  Serial.println(ssid1);
  Serial.println(password1);
  Serial.println(ssid2);
  Serial.println(password2);
  Serial.print("Wait for WiFi... ");
  
  // while (WiFiMulti.run() != WL_CONNECTED) {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("WiFi.status(): ");
    Serial.println(WiFi.status());
    delay(1500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
  // delay(LOOP_DELAY);
  // Check sensor
  // Serial.println("digitalRead(SENSE_PIN)");
  // Serial.println(digitalRead(SENSE_PIN));
  if (digitalRead(SENSE_PIN) == 1) {
    digitalWrite(LED_BUILTIN, HIGH);  
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (prevState != digitalRead(2)) {
    Serial.println("Change Detected, fire all the stuff here, pausing for a while, not sure the amount of time yet");
    if (!client.connect(host, 54322)) {
      Serial.println("Connection failed");
      return;
    }
    prevState = digitalRead(2);

    String url = "/savelog";
    Serial.println("sending request");
    client.print(String("POST ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\nsensor_id=" + sensorid + "&message=" + message + "&token=" + token + "\r\nUser-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");
    Serial.println("request sent");
    delay(TIME_TO_WAIT_AFTER_TRIGGER); 
  }
}

