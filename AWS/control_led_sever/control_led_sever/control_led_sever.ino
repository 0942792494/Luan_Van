#include "FS.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
#include <NTPClient.h>
#include <WiFiUdp.h>
// thu vien wiffi
//const char* ssid = "PHUONG HAN_3";
//const char* password = "0983882322";
const char* ssid = "Thanh Binh 12";
const char* password = "@12345678@";
// dat sever chay port 80
WiFiServer server(80);
String header; 
String output5State = "off";
String output4State = "off";
const int output5 = 5;
const int output4 = 4;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
const char* AWS_endpoint = "a2vy0a9v7n77r9-ats.iot.us-east-1.amazonaws.com"; //MQTT broker ip
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]); // Pring payload content
  }
    char led = (char)payload[62]; // Extracting the controlling command from the Payload to Controlling LED from AWS
    Serial.print("led command=");
    Serial.println(led);
    if(led==49) // 49 is the ASCI value of 1
    {
      digitalWrite(D5, HIGH);
      Serial.println("LED_State changed to HIGH");
    }
    else if(led==48) // 48 is the ASCI value of 0
    {
      digitalWrite(D5, LOW);
      Serial.println("LED_State changed to LOW");
     }          
  Serial.println();
}
WiFiClientSecure espClient;
PubSubClient client(AWS_endpoint, 8883, callback, espClient); //set  MQTT port number to 8883 as per //standard
long lastMsg = 0;
char msg[50];
int value = 0;

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
  pinMode (output5,OUTPUT);
  pinMode (output4,OUTPUT);
  digitalWrite(output5,LOW);
  digitalWrite(output4,LOW);
  Serial.setDebugOutput(true);
  pinMode(D5, OUTPUT);// initialize digital pin LED_BUILTIN as an output.
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
}
void loop() {
WiFiClient client = server.available();
if(client ){
Serial.println("Client moi.");
String currentLine = "";
while (client.connected()){
  if(client.available()){
    char c = client. read ();
    Serial.write(c);
    header += c ; 
    if (c == '\n'){
      if (currentLine.length()==0){
        client.println("HTTP/1.1 200 ok");
        client.println("content-type:text/html");
        client.println("connection: close");
        client.println();
        if (header.indexOf("GET /5/on") >= 0 ){
          Serial.println("GIPO 5 on");
          output5State = " on "; 
          digitalWrite(output5,HIGH);
         } else if (header.indexOf("GET /5/on") >= 0 ){
          Serial.println("GIPO 5 off");
          output5State = " of "; 
          digitalWrite(output5,LOW);
         } else if (header.indexOf("GET /4/on") >= 0 ){
          Serial.println("GIPO 4 on");
          output4State = " on "; 
          digitalWrite(output4,HIGH);
         } else if (header.indexOf("GET /4/on") >= 0 ){
          Serial.println("GIPO 4 off");
          output4State = " of "; 
          digitalWrite(output4,LOW);
         }
         //  Giao dien WEB 
         client.println("<!DOCTYPE html><html>");
         client.println("<head><meta name = \"viewport\" content= \" width= device-with, initial-scale = 1 \">");
         client.println("<link rel= \" icon\" href = \" Data:,\">");

         client.println("<style> html {font-family: Helvetial; display: inline-block; margin: opx auto; text-aligin: center;}");
         client.println(".button {backroud-color: #195B6A; border: none; color: white; padding: 16px 40px;}");
         client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;");
         client.println(".button2 {backround-color: #77878A;}</style></head>");

         client.println("<body><h1> Welcome to IOT AWS </h1>");
         
         client.println("<p> GIPO 5 - State" + output5State + "</p>");
         if (output5State = "off"){
            client.println("<p><a href= \"/5/on\"><button class= \"button\"> ON </button></a></p>");
          } else {  
            client.println("<p><a href= \"/5/off\"><button class= \"button button2\"> OFF </button></a></p>");
          }

         client.println("<p> GIPO 4 - State" + output4State + "</p>");
         if (output4State = "off"){
            client.println("<p><a href= \"/4/on\"><button class= \"button\"> ON </button></a></p>");
          } else {  
            client.println("<p><a href= \"/4/off\"><button class= \"button button2\"> OFF </button></a></p>");
          }
          client.println("</body></html>");
          client.println();
          break;
         } else {
          currentLine = "" ; 
         }
      }else if (c !='\r'){
         currentLine += c;   
      }
    }
  } 
  header = "";
  client.stop();
  Serial.println("Ngat ket noi Client ");
  Serial.println();
}
  if (!client.connected()) {
    reconnect();
  }
}
