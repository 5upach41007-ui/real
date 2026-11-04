#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#define DHTPIN D2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer sever (80);
const char* Wifiid = "";
const char* Wifipas = "";




void handledata() {
   float hu = dht.readHumidity();
   float tem = dht.readTemperature();
  StaticJsonDocument<128> doc;
  if(isnan(hu) || isnan(tem)){
    doc["error"] = "อ่านค่าไม่ได้";
  }  else {
    doc["hu"]  = round(hu * 10)/(10.0);
    doc["tem"] = round(tem * 10)/(10.0);  
  } 
    String json;
    serializeJson(doc, json);
    sever.send(200, "application/json", json);
}
void setup() {
  Serial.begin(115200);
  dht.begin();

  Serial.print("กำลังเชื่อมต่อ");
  WiFi.begin(Wifiid, Wifipas);
  while(WiFi.status() != WL_CONNECTED){
     delay(500);
     Serial.print(".");
  }
   
    Serial.println("");
    Serial.print("เชื่อมต่อสำเร็จ");
    Serial.print(WiFi.localIP());
    sever.on("/data", handledata);
    sever.begin();
    Serial.println("เซิฟเวอร์เริ่มทำงานแล้ว");
}
  void loop() {
    sever.handleClient();
  }
}
