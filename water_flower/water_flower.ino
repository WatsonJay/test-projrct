int ASignal = A0;
int ReceiveByte = 0;
int loopCnt;
int sensorValue;
int dirwetmax=0;
unsigned long time;
int humi;int temp;
int chr[24] = {0};
int tol=0;
int ditwet=0;
#define pin 10
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void dir_wet() {
  sensorValue = analogRead(ASignal);
  if(sensorValue>dirwetmax)
      dirwetmax=sensorValue;
}

void temp_wet(){
   bgn:
  delay(500);
//设置2号接口模式为：输出
//输出低电平20ms（>18ms）
//输出高电平40μs
  pinMode(pin,OUTPUT);
  digitalWrite(pin,LOW);
  delay(20);
  digitalWrite(pin,HIGH);
  delayMicroseconds(40);
  digitalWrite(pin,LOW);
//设置2号接口模式：输入
  pinMode(pin,INPUT);
  //高电平响应信号
  loopCnt=10000;
  while(digitalRead(pin) != HIGH)
  {
    if(loopCnt-- == 0)
    {
//如果长时间不返回高电平，输出个提示，重头开始。
      Serial.println("HIGH");
      goto bgn;
    }
  }
  //低电平响应信号
  loopCnt=30000;
  while(digitalRead(pin) != LOW)
  {
    if(loopCnt-- == 0)
    {
//如果长时间不返回低电平，输出个提示，重头开始。
    //  Serial.println("LOW");
      goto bgn;
    }
  }
//开始读取bit1-24的数值  
    for(int i=0;i<24;i++)
  {
    while(digitalRead(pin) == LOW)
    {}
//当出现高电平时，记下时间“time”
    time = micros();
    while(digitalRead(pin) == HIGH)
    {}
//当出现低电平，记下时间，再减去刚才储存的time
//得出的值若大于50μs，则为‘1’，否则为‘0’
//并储存到数组里去
    if (micros() - time >50)
    {
      chr[i]=1;
    }else{
      chr[i]=0;
    }
  }
   
//湿度，8位的bit，转换为数值
humi=chr[0]*128+chr[1]*64+chr[2]*32+chr[3]*16+chr[4]*8+chr[5]*4+chr[6]*2+chr[7];
   
//温度，8位的bit，转换为数值
temp=chr[16]*128+chr[17]*64+chr[18]*32+chr[19]*16+chr[20]*8+chr[21]*4+chr[22]*2+chr[23];

//输出：温度、湿度、校对码
   // Serial.print("temp:");
   // Serial.println(temp);
   // Serial.print("humi:");
   // Serial.println(humi);
}

void loop(){
  dir_wet();
  temp_wet();
  int i=0;
  ditwet=(dirwetmax-sensorValue)*100/(dirwetmax-250);
  if(ditwet<0)
  {
    ditwet=-ditwet;
  }
  if (Serial.available()> 0) 
   {   //串口是否有输入
            ReceiveByte = Serial.read();
            i++;
            switch(ReceiveByte){
                case 0x30:    //十进制48转换十六进制为30
                          Serial.print(99,DEC);   //发送温度识别码
                          Serial.print(temp, DEC);//发送值
                          Serial.print(98,DEC);   //发送湿度识别码
                          Serial.print(humi, DEC); //发送值
                          Serial.print(97,DEC);   //发送d土壤识别码
                          if(ditwet==100||ditwet<=0)
                          {
                             Serial.print(0,DEC);
                             Serial.print(0,DEC);
                          }
                          else
                          {
                            if(ditwet<10&&ditwet>0)
                            {
                                Serial.print(0, DEC);//发送值
                            }
                            Serial.print(ditwet, DEC);//发送值
                            break;
                          }
                other:break;
            }
          ReceiveByte=0x00;  
   }
}

