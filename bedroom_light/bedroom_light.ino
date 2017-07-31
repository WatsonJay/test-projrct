const int TrigPin = 4; //管脚定义
const int EchoPin = 7; 
const int LightOn = 2;

float distance=0;//变量定义
int ReceiveByte = 0; 
float distance_now=0;
bool light_single=false;

void setup() //初始化设置
{
  Serial.begin(9600);
  pinMode(TrigPin, OUTPUT); 
  pinMode(EchoPin, INPUT);
}

bool sound_commend() //声控确认
{
  int Soundvalue = analogRead(A0);  //读取输入模拟值
  if(Soundvalue<40)
  {
     //Serial.print("soundvalue:");
     //Serial.println(Soundvalue);
     return true;
  }
  else
  {
    return false; 
  }
}

float distance_commend() //距离检测
{
   // 产生一个10us的高脉冲去触发TrigPin 
        digitalWrite(TrigPin, LOW); 
        delayMicroseconds(2); 
        digitalWrite(TrigPin, HIGH); 
        delayMicroseconds(10);
        digitalWrite(TrigPin, LOW); 
    // 检测脉冲宽度，并计算出距离
        distance = pulseIn(EchoPin, HIGH) / 58.00;
        return distance;
        delay(100);
}

void buletooth_commend() //蓝牙控制
{
  if (Serial.available()> 0) 
   {   //串口是否有输入
            ReceiveByte = Serial.read();
            //Serial.print(ReceiveByte); //打开 Arduino 自带的监视器  
            //Serial.println(); //输出换行  
            switch(ReceiveByte){
              case 0x30:Light_Commend();/**Serial.print("on")**/;break;
              case 0x31:Light_Commend();/**Serial.print("off")**/;break;
            }
   }
}

void light_unlock()  //灯光解锁
{
  float a,b,c,d;
  a=distance_commend();
  delay(200);
  b=distance_commend();
  delay(200);
  c=distance_commend();
  delay(200);
  d=distance_commend();
  if(a<10&&b<10&&c<10&&d<10)
  {
     light_single=false;
     Serial.print("false");
  }
}
void Light_Commend()  //亮灯控制
{
  digitalWrite(LightOn, HIGH); 
  light_single=true;
  delay(100);
  digitalWrite(LightOn, LOW);
}

void loop() {  
  if(!light_single)
  {
     if(sound_commend())
    {
      distance_now=distance_commend();
      if(distance_now<160)
      {    
        /**
        Serial.print(distance_now); 
        Serial.print("cm"); 
        Serial.println();
        **/
        Light_Commend();
      }
    }
  }
  else
  {
    light_unlock();
  }
  buletooth_commend();
}
