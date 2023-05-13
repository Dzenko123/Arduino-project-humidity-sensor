#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

#define FIREBASE_HOST "."
#define FIREBASE_AUTH "."
#define WIFI_SSID "."
#define WIFI_PASSWORD "."

const int sensor_pin = A0;
const int led_pin = D4;
const int led_pin2 = D3;

FirebaseJson json;
FirebaseData firebasedata;


unsigned long ledPrevMillis = 0;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
int ledStatus;
int brojac=0;

void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(sensor_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  digitalWrite(led_pin2, LOW);

  Serial.begin(115200);
  
//nasumično paljenje i ostaju ugašene
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  int soilMoistureValue = 0;
  int percentage = 0;
  soilMoistureValue = analogRead(sensor_pin);
  percentage = map(soilMoistureValue, 490, 1023, 100, 0);
  Serial.println(percentage);
  json.set("/vlaznost", percentage);
  Firebase.updateNode(firebasedata, "/Vlaznost", json);
  delay(1000);

//paljenje i gašenje stalno

  if (Firebase.RTDB.getInt(&firebasedata, "/LED/status")) {
    if (firebasedata.dataType() == "int") {
      ledStatus = firebasedata.intData();
      //paljenje i gašenje 5 puta i ostanu upaljene
      //postepeno paljenje gašenje
      
      if (ledStatus == 1) {
        digitalWrite(led_pin, LOW);
        digitalWrite(led_pin2, HIGH);
      }
      else if (ledStatus == 0) {
        digitalWrite(led_pin, HIGH);
        digitalWrite(led_pin2, LOW);
      }
     }
  }
  else {
    Serial.println(firebasedata.errorReason());
  }
  
//vlažnost ispod 25
//vlaznost izmedu 30 i 50
}
