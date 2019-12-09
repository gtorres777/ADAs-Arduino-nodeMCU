#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "DHT.h"
 
// CONSTRUCTOR DEL OBJETO DHT RECIBE EL PIN EN EL QUE SE CONECTA EL SENSOR
// Y TAMBIEN RECIBE EL TIPO DE SENSOR QUE VAMOS A CONECTAR
DHT dht(2, DHT11);
// WiFi Parameters
const char* ssid = "Kozak";
const char* password = "pzsh5324";
boolean estaParado = false;
boolean estaSentado = false;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Wire.begin(D1 , D2);
  delay(10);

 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  dht.begin();
}

void loop() {
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://tux777.pythonanywhere.com/acciones/");
    int httpCode = http.GET();
    //Check the returning code                                                                  
    if (httpCode > 0) {
      // Parsing    
      StaticJsonDocument<670> root;
      DeserializationError err = deserializeJson(root,http.getString());
      
      if (err){
        Serial.print("ERROR: ");
        Serial.println(err.c_str());
        return;
        }
      // Parameters
      int pararse = root[0]["pararse"]; // 1
      int sentarse = root[0]["sentarse"]; // "Leanne Graham"
      int avanzar = root[0]["avanzar"];
      int retroceder = root[0]["retroceder"];
      int girarIzquierda = root[0]["girarIzquierda"];
      int girarDerecha = root[0]["girarDerecha"];
      int saludar = root[0]["saludar"];
      int bailar = root[0]["bailar"];
  
      // Output to serial monitor
      Serial.print("Pararse:");
      Serial.println(pararse);
      Serial.print("Sentarse:");
      Serial.println(sentarse);
      Serial.print("Avanzar:");
      Serial.println(avanzar);
      Serial.print("Retroceder:");
      Serial.println(retroceder);
      Serial.print("Girar Izquierda:");
      Serial.println(girarIzquierda);
      Serial.print("Girar Derecha:");
      Serial.println(girarDerecha);
      Serial.print("Saludar:");
      Serial.println(saludar);
      Serial.print("Bailar:");
      Serial.println(bailar);
      Serial.println("---------------");
      
      if(pararse == 1){
        envio('p');
      }else if(sentarse == 1){
        envio('s');
      }else if(avanzar == 1){
        envio('a');
      }else if(retroceder == 1){
        envio('r');
      }else if(girarIzquierda == 1){
        envio('i');
      }else if(girarDerecha == 1){
        envio('d');
      }else if(saludar == 1){
        envio('v');
      }else if(bailar == 1){
        envio('b');
      }
      
      http.end();   //Close connection11      
  }

  HTTPClient http3;  //Object of class HTTPClient

  // LEER LA HUMEDAD USANDO EL METRODO READHUMIDITY
  float h = dht.readHumidity();
  // LEER LA TEMPERATURA USANDO EL METRODO READTEMPERATURE
  float t = dht.readTemperature();

  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" % ");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C");

  String urldatos = "http://tux777.pythonanywhere.com/dato/?temperatura=" + String(t) + "&humedad=" + String(h);
  Serial.println(urldatos);
  
  http3.begin(urldatos);
  int http3Code = http3.GET();

  if (http3Code > 0) {
      http3.end();   //Close connection1
  }

  HTTPClient http2;  //Object of class HTTPClient
    http2.begin("http://tux777.pythonanywhere.com/accion/?accion=reinicio");
    int http2Code = http2.GET();

  if (http2Code > 0) {
      http2.end();   //Close connection1
  }
  // Delay
  delay(1000);
}
}

void envio(char var){
  Wire.beginTransmission(8); 
  Wire.write(var); 
  Wire.endTransmission();
}
