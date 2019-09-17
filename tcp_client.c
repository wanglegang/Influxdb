#include <WiFi.h>
//arduino ide 1.8.8 ,ESP32 Wrover Module
const char *ssid = "x**";
const char *password = "***";

const IPAddress serverIP(148,70,7*,*); //欲访问的地址
uint16_t serverPort = 8086;         //服务器端口号

WiFiClient client; //声明一个客户端对象，用于与服务器进行连接

void setup()
{
    Serial.begin(115200);
    Serial.println();

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); //关闭STA模式下wifi休眠，提高响应速度
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected");
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());
}

void loop()
{
    Serial.println("尝试访问服务器");
    if (client.connect(serverIP, serverPort)) //尝试访问目标地址
    {
        Serial.println("访问成功");


        while (client.connected() || client.available()) //如果已连接或有收到的未读取的数据
        {  
           client.write("POST /write?db=** HTTP/1.1");   //数据库的名称
           client.write(0x0D0A);
           delay(50);
           client.write("Host:");
           client.write(0x0D0A);
           client.write("Content-Length: 43");
           client.write(0x0D0A);
           client.write(0x0D0A);
           client.write("");
           delay(50);
           client.write("***,host=server01,region=us-west value=3.66"); //表的名称
           client.write(0x0D0A);
           delay(50);
           Serial.println("写入成功");
                  
          
          
            if (client.available()) //如果有数据可读取
            {
                //String line = client.readStringUntil('\n'); //读取数据到换行符
                String line = client.readStringUntil('\r'); //读取数据到换行符
                Serial.print("读取到数据：");
                Serial.println(line);
               
             //   client.write(line.c_str()); //将收到的数据回发
            }
             delay(5000);
        }
        Serial.println("关闭当前连接");
        client.stop(); //关闭客户端
    }
    else
    {
        Serial.println("访问失败");
        client.stop(); //关闭客户端
    }
    delay(5000);
}
