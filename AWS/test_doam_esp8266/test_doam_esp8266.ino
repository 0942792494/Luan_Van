#include "FS.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h> //https://www.arduinolibraries.info/libraries/pub-sub-client
#include <NTPClient.h> //https://www.arduinolibraries.info/libraries/ntp-client
#include <WiFiUdp.h>

#define DHTTYPE DHT11
#include "DHT.h"
#define dht_dpin 0
DHT dht(dht_dpin, DHTTYPE);
int value1 = digitalRead(dht_dpin);
// Update these with values suitable for your network.
//const char* ssid = "Cafe NgocYen 3 BET 2.4GHz";
//const char* password = "ngocyen2014";
const char* ssid = "Thanh Binh 12";
const char* password = "@12345678@";
//const char* ssid = "PHUONG HAN_3";
//const char* password = "0983882322";
//const char* ssid = "anh ne";
//const char* password = "99999999";
//const char* ssid = "1234";
//const char* password = "vinhdeptrai";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const char* AWS_endpoint = "a2vy0a9v7n77r9-ats.iot.us-east-1.amazonaws.com"; //MQTT broker ip
void callback(char* topic, byte* payload, unsigned int length) {
Serial.print("Message arrived [");
Serial.print(topic);
Serial.print("] ");
for (int i = 0; i < length; i++) {
Serial.print((char)payload[i]);
}
Serial.println();

}
WiFiClientSecure espClient;
PubSubClient client(AWS_endpoint, 8883, callback, espClient); //set MQTT port number to 8883 as per //standard
//============================================================================
#define BUFFER_LEN 256
long lastMsg = 0;
char msg[BUFFER_LEN];
int value = 0;
byte mac[6];
char mac_Id[18];
//============================================================================

void setup_wifi() {
delay(10);
// We start by connecting to a WiFi network
espClient.setBufferSizes(512, 512);
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
timeClient.begin();
while(!timeClient.update()){
timeClient.forceUpdate();
}
espClient.setX509Time(timeClient.getEpochTime());
}
void reconnect() {
// Loop until we're reconnected
while (!client.connected()) {
Serial.print("Attempting MQTT connection...");
// Attempt to connect
if (client.connect("ESPthing")) {
Serial.println("connected");
// Once connected, publish an announcement...
client.publish("outTopic", "hello world");
// ... and resubscribe
client.subscribe("inTopic");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");

char buf[256];
espClient.getLastSSLError(buf,256);
Serial.print("WiFiClientSecure SSL error: ");
Serial.println(buf);

// Wait 5 seconds before retrying
delay(5000);
}
}
}

void setup() {

Serial.begin(115200);
Serial.setDebugOutput(true);
// initialize digital pin LED_BUILTIN as an output.
pinMode(LED_BUILTIN, OUTPUT);
setup_wifi();
delay(1000);
if (!SPIFFS.begin()) {
Serial.println("Failed to mount file system");
return;
}

Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());

// Load certificate file
          File cert = SPIFFS.open("/cert.der", "r"); //replace cert.crt eith your uploaded file name
          if (!cert) {
          Serial.println("Failed to open cert file");
          }
          else
          Serial.println("Success to open cert file");
          delay(1000);
          if (espClient.loadCertificate(cert))
          Serial.println("cert loaded");
          else
          Serial.println("cert not loaded");
// Load private key file
          File private_key = SPIFFS.open("/private.der", "r"); //replace private eith your uploaded file name
          if (!private_key) {
          Serial.println("Failed to open private cert file");
          }
          else
          Serial.println("Success to open private cert file");
          delay(1000);
          if (espClient.loadPrivateKey(private_key))
          Serial.println("private key loaded");
          else
          Serial.println("private key not loaded");
// Load CA file
          File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
          if (!ca) {
          Serial.println("Failed to open ca ");
          }
          else
          Serial.println("Success to open ca");
          
          delay(1000);
          
          if(espClient.loadCACert(ca))
          Serial.println("ca loaded");
          else
          Serial.println("ca failed");

Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());

//===========================================================================
WiFi.macAddress(mac);
snprintf(mac_Id, sizeof(mac_Id), "%02x:%02x:%02x:%02x:%02x:%02x",
mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
Serial.print(mac_Id);
Serial.println(" sensor DHT11");
delay(700);
//============================================================================

}

void loop() {
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.println(value1);
    Serial.print("Do AM = ");
    Serial.print(h);
    Serial.print(" % ");
    Serial.print(" Nhiet Do = ");
    Serial.print(t); 
    Serial.println(" C ");
if (!client.connected()) {
reconnect();
}
client.loop();
long now = millis();
if (now - lastMsg > 2000) {
lastMsg = now;
//============================================================================
String macIdStr = mac_Id;
String doam = String(h);
String nhietdo = String(t);
//String nhietdo1 = "" + nhietdo;
//uint8_t randomNumber = random(20, 50);
//String randomString = String(random(0xffff), HEX);
snprintf (msg, BUFFER_LEN, " mac_Id: \"%s\", Do_Am : \"%s\" % , Nhiet_do : \"%s\" C", macIdStr.c_str(),doam.c_str(),nhietdo.c_str());

Serial.print("Publish message: ");
Serial.println(msg);
//mqttClient.publish("outTopic", msg);
client.publish("outTopic", msg);
//=============================================================================
Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
}
//digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
//delay(800); // wait for a second
digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
//delay(500); // wait for a second
delay(500);
}