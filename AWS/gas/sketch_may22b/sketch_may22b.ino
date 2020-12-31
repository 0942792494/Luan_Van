void setup() {
  Serial.begin(9600);  //Mở cổng Serial để giap tiếp | tham khảo Serial
}
int pin = A3;
void loop() {
  int valuegas = analogRead(pin);  
  if(valuegas > 250 ){
  Serial.print("Co khi doc",valuegas);                                   
  }
  else {
  Serial.println(" khong khi binh thuong ");
  }
  //-----------------------------------------------------
  delay(100);  
}
