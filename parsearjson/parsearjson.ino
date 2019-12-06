#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
// WiFi Parameters
const char* ssid = "TD";
const char* password = "TESTING7779";
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
}

void loop() {
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://192.168.1.131:8000/acciones/");
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
      //const char* email = root["email"]; // "Sincere@april.biz"
  
      // Output to serial monitor
      Serial.print("Pararse:");
      Serial.println(pararse);
      Serial.print("Sentarse:");
      Serial.println(sentarse);
      
      if(pararse == 1){
        envio('p');
      }else if(sentarse == 1){
        envio('s');
      }
      
      http.end();   //Close connection11      
  }


   HTTPClient http2;  //Object of class HTTPClient
    http2.begin("http://192.168.1.131:8000/accion/?accion=reinicio");
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
