void setup() {
  Serial.begin(9600);
}
void loop() {
  int Soundvalue = analogRead(A0);  //读取输入模拟值
  if(Soundvalue<40)
  {
     Serial.print("soundvalue:");
     Serial.println(Soundvalue);
  }
  //Serial.print("soundvalue:");
  //Serial.println(Soundvalue);
}
 
