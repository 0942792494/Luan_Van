#include <ESP8266WiFi.h>
#include <PubSubClient.h>
WiFiClient espClient;
//WiFiClientSecure espClient;
const char* ssid = "Thanh Binh 12";
const char* password = "@12345678@";
#define BUFFER_LEN 256
long lastMsg = 0;
char msg[BUFFER_LEN];
int value = 0;
byte mac[6];
char mac_Id[18];

PubSubClient client(espClient);
void callback(char* topic, byte* payload, unsigned int length) {
  if ((char)payload[0] == '1') {
    digitalWrite(D0, HIGH);
  } else {
    digitalWrite(D0, LOW);
  }
}


void setup_wifi() {
delay(10);
// We start by connecting to a WiFi network
//espClient.setBufferSizes(512, 512);
Serial.println();
Serial.print("Dang ket noi wiffi ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
}
Serial.println("============================");
Serial.println(" Ket noi wiffi thanh cong ! ");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  digitalWrite(D0,LOW);
  pinMode(D0,OUTPUT);
  setup_wifi(); 
  client.setServer("io.adafruit.com", 1883);
  client.setCallback(callback);
  client.connect("Arduino8266", "vinh0942792494", "aio_XJyG23feVci9AM0j6oZ90CpQ37Al");
  client.subscribe("vinh0942792494/feeds/controll");
}
 
void loop(){
  client.loop();
}
