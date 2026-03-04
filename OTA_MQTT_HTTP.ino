#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#define SOF 0xA5
#define ACK 0x79
#define NACK 0x1F

const char* ssid="Pussy Hunter";
const char* password="Jk@18.12";

const char* mqtt_server="broker.hivemq.com";
const char* deviceID="esp01s";

String currentVersion="1.1.0";

HardwareSerial &stmSerial= Serial;  //All serials from here will be commented as they were used for debugging. Here on only stmSerial statements

WiFiClient espClient;
PubSubClient client(espClient);

uint16_t crc16(uint8_t* a, uint8_t len){
  uint16_t crcVal=0;

  for(uint8_t i=0;i<len;i++){
    crcVal^=a[i];
    for(uint8_t j=0;j<8;j++){
      if(crcVal & 0x0001)
        crcVal=(crcVal>>1)^0xA001;
      else
        crcVal=(crcVal>>1);
    }
  }

  return crcVal;
}

bool sendChunk(uint8_t* a, uint8_t len){
  uint16_t crc=crc16(a, len);

  stmSerial.write(SOF);
  stmSerial.write(len);
  stmSerial.write(a, len);
  stmSerial.write((uint8_t *)&crc, 2);

  uint32_t curTime=millis();

  while(!stmSerial.available()){
    if(millis()-curTime>=1000)
      return false;
  }

  uint8_t response=stmSerial.read();

  if(response==ACK)
    return true;
  else 
    return false;
}

void startUpdate(){
  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://192.168.137.1:8000/firmware");

  int httpCode = http.GET();
  uint32_t totalSize=http.getSize();

  stmSerial.write((uint8_t *)&totalSize, 4);

  int retry = 0;
  while(stmSerial.read()!=ACK){
    retry++;
    if (retry > 3) {
      // Serial.println("Failed Size information transfer to STM");
      return;
    }
  }

  if (httpCode == HTTP_CODE_OK) {
    WiFiClient* stream = http.getStreamPtr();


    while (http.connected()) {
      while (stream->available()) {
        uint8_t buffer[128];
        uint8_t len = stream->readBytes(buffer, sizeof(buffer));

        int retry = 0;
        while (!sendChunk(buffer, len)) {
          retry++;
          if (retry > 3) {
            // Serial.println("Failed transfer to STM");
            return;
          }
        }
      }
    }
  } 
  else {
    // Serial.printf("HTTP GET failed, error: %d\n", httpCode);
  }

  http.end();
}

void callback(char* topic, byte* message, unsigned int len){
  StaticJsonDocument<200> jdoc;

  deserializeJson(jdoc, message, len);

  bool updateAvailable=jdoc["update"];
  String version=jdoc["version"];

  // Serial.println("Update information received");

  if(updateAvailable && currentVersion !=version){
    // Serial.println("Update has to be done");
    // Serial.printf("Version: %s\r\n", version);

    startUpdate();
  }
  else{
    // Serial.println("Updated to point");
  }
}

void reconnect(){
  while(!client.connected()){
    // Serial.println("Trying to connect to MQTT broker.....");
    if(client.connect(deviceID)){
      // Serial.println("Connected to MQTT broker");

      String topic="device/"+ String(deviceID) +"/update/response";
      client.subscribe(topic.c_str());

      String checkTopic = "device/" + String(deviceID) + "/update/check";
      client.publish(checkTopic.c_str(), "check");
    }
    else{
      delay(2000);
    }
  }
}

void setup() {
  // Serial.begin(115200);
  stmSerial.begin(115200);
  WiFi.begin(ssid, password);

  while(WiFi.status()!=WL_CONNECTED){
    // Serial.println("Trying to connect to Hunter...");
    delay(500);
  }

  // Serial.println("Connected to Hunter");  

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}
