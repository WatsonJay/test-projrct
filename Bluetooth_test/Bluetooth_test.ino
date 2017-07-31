int ReceiveByte = 0;
void setup() {
   Serial.begin(9600); //打开蓝牙串口
}

void loop()
{
  if (Serial.available()> 0) 
   {   //串口是否有输入
            ReceiveByte = Serial.read();
            Serial.print(ReceiveByte); //打开 Arduino 自带的监视器  
            Serial.println(); //输出换行  
   }
}

