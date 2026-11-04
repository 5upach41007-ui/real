#include <ESP8266WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define WiFiU  "PICREAL"
#define WiFipas  "12345678"
#define Wifihost  "192.168.10.100"
#define WifiPORT 1883
#define Mqttuse  "REAL"
#define Mqttpass  "123456"
#define TOPIC  "it/vec"
#define DHTPIN D2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
WiFiClient psb;
PubSubClient mqtt(psb);

 void setup() {
    Serial.begin(115200);
    WiFi.begin(WiFiU, WiFipas);

    WiFi.setAutoReconnect(true);
    dht.begin();
    mqtt.setServer(Wifihost, WifiPORT);
 }
 void loop()
 {
    static unsigned long sui=0;
    if(WiFi.status() == WL_CONNECTED && !mqtt.connected() &&millis() - sui > 5000)
 
  {
    sui = millis();
    bool ok = mqtt.connect("ESP8266-Cleancode",Mqttuse,Mqttpass);
    Serial.println(ok ? "Mqtt connect":"Mqtt Fail");
  }  
   mqtt.loop(); 

    float tem =dht.readTemperature();
    float hu =dht.readHumidity();
     
    if(isnan(tem) || isnan(hu))
    {
      Serial.println("Fail");
      return;
    }  
    JsonDocument doc;
    doc["temperature"] = tem;
    doc["humidity"] = hu;

    char push[128];
    serializeJson(push, doc);
    mqtt.publish(TOPIC, push);
    delay(2000);
    }
