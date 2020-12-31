
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "J2aWVd7e6NIcwC99__l6nib6D9ZpHsvq";

char ssid[] = "Vinh Van";
char pass[] = "tamchuvvietho@";

WidgetLED LedConnect(V0);
WidgetLED LedAction(V5);
WidgetLED StatusD1(V1);
WidgetLED StatusD2(V2);
WidgetLED StatusD3(V3);
WidgetLED StatusD4(V4);
WidgetLED StatusD5(V5);
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
BlynkTimer timer;


void blinkLedWidget()
{
  if (LedConnect.getValue()) {
    LedConnect.off();
  } else {
    LedConnect.on();
  }
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); 
  digitalWrite(D1,pinValue);
  Blynk.virtualWrite(V1, pinValue);
  if (!digitalRead(D1)) {
    StatusD1.off();
  } else {
    StatusD1.on();
  }
  for(int i=0; i<5; i++){
    LedAction.on();
    delay(50);
    LedAction.off();
    delay(50);
  }
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  digitalWrite(D2,pinValue);
  Blynk.virtualWrite(V2, pinValue);
  if (!digitalRead(D2)) {
    StatusD2.off();
  } else {
    StatusD2.on();
  }
  for(int i=0; i<5; i++){
    LedAction.on();
    delay(50);
    LedAction.off();
    delay(50);
  }
}

BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); 
  digitalWrite(D3,pinValue);
  Blynk.virtualWrite(V3, pinValue);
  if (!digitalRead(D3)) {
    StatusD3.off();
  } else {
    StatusD3.on();
  }
  for(int i=0; i<5; i++){
    LedAction.on();
    delay(50);
    LedAction.off();
    delay(50);
  }
}

BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); 
  digitalWrite(D4,pinValue);
  Blynk.virtualWrite(V4, pinValue);
  if (!digitalRead(D4)) {
    StatusD4.off();
  } else {
    StatusD4.on();
  }
  for(int i=0; i<5; i++){
    LedAction.on();
    delay(50);
    LedAction.off();
    delay(50);
  }
}

BLYNK_WRITE(V5)
{
  int pinValue = param.asInt(); 
  digitalWrite(D5,pinValue);
  Blynk.virtualWrite(V5, pinValue);
  if (!digitalRead(D5)) {
    StatusD5.off();
  } else {
    StatusD5.on();
  }
  for(int i=0; i<5; i++){
    LedAction.on();
    delay(50);
    LedAction.off();
    delay(50);
  }
}
void setup()
{
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, blinkLedWidget);

  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D5,OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,HIGH);
  digitalWrite(D5,HIGH);
}
void loop()
{
  Blynk.run();
  timer.run();
}
