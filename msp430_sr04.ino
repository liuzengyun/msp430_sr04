// 超声波模块//

#define Trig P1_2
#define Echo P1_4 
float s;  // 距离
float t;  // 高脉冲信号时间

const String wifiname="iPhone";
const String wifipsw="lmq073043";
const String ip="172.20.10.4";
const String port="5000";




void setup() 
{
  Serial.begin(115200); //初始化串口监视器
  pinMode(Trig,OUTPUT);
  pinMode(Echo,INPUT);
  connectt();
}

void loop() 
{
  //给Trig发送一个低高低的短时间脉冲,触发测距
  digitalWrite(Trig, LOW); //给Trig发送一个低电平
  delayMicroseconds(2);    //等待2微妙
  digitalWrite(Trig,HIGH); //给Trig发送一个高电平
  delayMicroseconds(15);   //15微妙高电平，触发测距
  digitalWrite(Trig, LOW); //给Trig发送一个低电平

  //pulseIn函数，返回接收端高电平维持时间，计算与障碍物距离
  t=float(pulseIn(Echo,HIGH)); 
  s=(t*17)/1000; 

  //串口监视器
   Serial.print("Echo =");
   Serial.print(t);//串口输出等待时间的原始数据
   Serial.print(" | | Distance = ");
   Serial.print(s);//串口输出距离换算成cm的结果
  Serial.print("cm\n");
  delay(500);

}



//**连接 & 设置模式**//
void connectt()
{
    sendCommand("+++\r",1000);
    sendCommand("AT+RESTORE\r\n",1000);
    sendCommand("AT+CWMODE=1\r\n",1000);                                  // 设置 STA 模式
    sendCommand("AT+CWJAP=\""+wifiname+"\",\""+wifipsw+"\"\r\n",5000);   // 连接 WIFI
    sendCommand("AT+CIFSR\r\n",1000);                                     // 查询模块当前的IP
    sendCommand("AT+CIPMUX=0\r\n",1000);                                  // 设置模块单连接
    sendCommand("AT+CIPMODE=1\r\n",1000);                                 // 设置透传模式
    sendCommand("AT+CIPSTART=\"TCP\",\""+ip+"\","+port+"\r\n",5000);  // 连接服务器端口
    sendCommand("AT+CIPSEND\r\n",1000);                                   // 发送数据指令
}


//**发送指令**//
String sendCommand(String command, const int timeout)
{
    String response = "";
    Serial.print(command);            // 将指令发送给 esp8266
    long int time = millis();          // 返回当前cpu的时间，单位为微秒
    while( (time+timeout) > millis())  // 延时，timeout为延时毫秒数
    {
      while(Serial.available())       // 当esp8266串口收到了数据
      {
        char c = Serial.read();       // 读取esp8266缓冲区内容
        response+=c;
      }  
    }
    return response;
}
