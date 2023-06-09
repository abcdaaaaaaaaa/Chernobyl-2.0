#include <WiFi.h>
#include <HTTPClient.h>
#include "ESPAsyncWebServer.h"
#include "Wire.h"
#include "I2Cdev.h" 
#include "MPU6050.h"
#include <SPI.h>
#include <Pixy2SPI_SS.h>


IPAddress local_IP(192, 168, 1, 2);
IPAddress gateway(255, 255, 0, 0);
IPAddress subnet(255, 255, 0, 0);

const char* ssid = "Chernobyl";
const char* password = NULL;

String xox, xox2;
const int IN2=12; 
const int IN1=13;
const int ENA=14;
const int IN4=25;
const int IN3=26;
const int ENB=27;


Pixy2SPI_SS pixy;

float deadZone = 0.15;
int baseSpeed = 130;
int now = 0;
int cont = 0;
int signature, x, y, width, height;
float cx, cy, area;

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

int motor_value;
int gyro_value;

const char* serverNamexxx = "http://192.168.1.1/xxx";
const char* serverNamepot = "http://192.168.1.1/pot";
const char* serverNamelih = "http://192.168.1.1/servo";

int minimum;
int maximum;

AsyncWebServer server(80);

void setup(){
  Serial.begin(115200);
  WiFi.config(local_IP, gateway, subnet);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 Serial.println(WiFi.localIP());

  Wire.begin();
  mpu.initialize();
  pinMode(IN2,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(ENB,OUTPUT);
  pixy.init();

  server.on("/pixy", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(pixy.ccc.numBlocks).c_str());
  });
  
  server.begin();
}

void loop(){
  vites();
switch(now) { 
  case 0:
 {
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
gyro_value = map(ax, -17000, 17000, minimum,maximum);
motor_value = abs(gyro_value);
     // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
if (httpGETRequest(serverNamexxx) == "1"){
    Serial.println("Dur");
    digitalWrite(IN2,LOW);
    digitalWrite(IN1,LOW);
    analogWrite(ENA,0);
    digitalWrite(IN4,LOW);
    digitalWrite(IN3,LOW);
    analogWrite(ENB,0);
    }
if (httpGETRequest(serverNamexxx) == "2"){
    Serial.println("İleri");  
    digitalWrite(IN2,LOW);
    digitalWrite(IN1,HIGH);
    analogWrite(ENA,motor_value);
    digitalWrite(IN4,LOW);
    digitalWrite(IN3,HIGH);
    analogWrite(ENB,motor_value);
    }
if (httpGETRequest(serverNamexxx) == "3"){
    Serial.println("Geri");  
    digitalWrite(IN2,HIGH);
    digitalWrite(IN1,LOW);
    analogWrite(ENA,motor_value);
    digitalWrite(IN4,HIGH);
    digitalWrite(IN3,LOW);
    analogWrite(ENB,motor_value);
    }
if (httpGETRequest(serverNamexxx) == "4"){
    Serial.println("Sol");  
    digitalWrite(IN2,LOW);
    digitalWrite(IN1,HIGH);
    analogWrite(ENA,motor_value);
    digitalWrite(IN4,HIGH);
    digitalWrite(IN3,LOW);
    analogWrite(ENB,motor_value);
    }
if (httpGETRequest(serverNamexxx) == "5"){
    Serial.println("Sağ");  
    digitalWrite(IN2,HIGH);
    digitalWrite(IN1,LOW);
    analogWrite(ENA,motor_value);
    digitalWrite(IN4,LOW);
    digitalWrite(IN3,HIGH);
    analogWrite(ENB,motor_value);
    }
}       
else {
    Serial.println("Dur");
    digitalWrite(IN2,LOW);
    digitalWrite(IN1,LOW);
    analogWrite(ENA,0);
    digitalWrite(IN4,LOW);
    digitalWrite(IN3,LOW);
    analogWrite(ENB,0);
    }
 delay(1); 
 }
  break;
 case 1:
{
  Serial.println("pixy nesne takip modu");  
  float turn = pixyCheck();
if(turn> -deadZone && turn < deadZone){
 turn = 0;
}
if (turn < 0) {
   digitalWrite(IN2,LOW); 
   digitalWrite(IN1,HIGH);
   analogWrite(ENA,80); 
   digitalWrite(IN4,HIGH);
   digitalWrite(IN3,LOW);
   analogWrite(ENB,170); 
}
else if (turn > 0) {
   digitalWrite(IN2,HIGH); 
   digitalWrite(IN1,LOW);
   analogWrite(ENA,170); 
   digitalWrite(IN4,LOW);
   digitalWrite(IN3,HIGH);
   analogWrite(ENB,80); 
}
else {
   digitalWrite(IN2,HIGH); 
   digitalWrite(IN1,LOW);
   analogWrite(ENA,70); 
   digitalWrite(IN4,HIGH);
   digitalWrite(IN3,LOW);
   analogWrite(ENB,70); 
}
 delay(1); 
} 
 break;
case 2:
{
    if(WiFi.status()== WL_CONNECTED ){ 
   Serial.println("lidar algoritması");  
if (httpGETRequest(serverNamexxx) == "L1"){
  //Dur
    digitalWrite(IN2,LOW);
    digitalWrite(IN1,LOW);
    analogWrite(ENA,0);
    digitalWrite(IN4,LOW);
    digitalWrite(IN3,LOW);
    analogWrite(ENB,0);
    }
if (httpGETRequest(serverNamexxx) == "L2"){
  //İleri
    digitalWrite(IN2,LOW);
    digitalWrite(IN1,HIGH);
    analogWrite(ENA,200);
    digitalWrite(IN4,LOW);
    digitalWrite(IN3,HIGH);
    analogWrite(ENB,200);
    }
if (httpGETRequest(serverNamexxx) == "L3"){
  //Geri
    digitalWrite(IN2,HIGH);
    digitalWrite(IN1,LOW);
    analogWrite(ENA,200);
    digitalWrite(IN4,HIGH);
    digitalWrite(IN3,LOW);
    analogWrite(ENB,200);
    }
if (httpGETRequest(serverNamexxx) == "L4"){
  //Sol
    digitalWrite(IN2,LOW);
    digitalWrite(IN1,HIGH);
    analogWrite(ENA,200);
    digitalWrite(IN4,HIGH);
    digitalWrite(IN3,LOW);
    analogWrite(ENB,200);
    }
if (httpGETRequest(serverNamexxx) == "L45"){
  // HAFİF Sol
    digitalWrite(IN2,LOW);
    digitalWrite(IN1,HIGH);
    analogWrite(ENA,200);
    digitalWrite(IN4,HIGH);
    digitalWrite(IN3,LOW);
    analogWrite(ENB,100);
    }
if (httpGETRequest(serverNamexxx) == "L5"){
  //Sağ
    digitalWrite(IN2,HIGH);
    digitalWrite(IN1,LOW);
    analogWrite(ENA,200);
    digitalWrite(IN4,LOW);
    digitalWrite(IN3,HIGH);
    analogWrite(ENB,200);
    }
if (httpGETRequest(serverNamexxx) == "L55"){
  //HAFİF Sağ
    digitalWrite(IN2,HIGH);
    digitalWrite(IN1,LOW);
    analogWrite(ENA,100);
    digitalWrite(IN4,LOW);
    digitalWrite(IN3,HIGH);
    analogWrite(ENB,200);
    }
}       
else {
    digitalWrite(IN2,LOW);
    digitalWrite(IN1,LOW);
    analogWrite(ENA,0);
    digitalWrite(IN4,LOW);
    digitalWrite(IN3,LOW);
    analogWrite(ENB,0);
    }
 delay(1);   
}
 break;
default:
{
    digitalWrite(IN2,LOW);
    digitalWrite(IN1,LOW);
    analogWrite(ENA,0);
    digitalWrite(IN4,LOW);
    digitalWrite(IN3,LOW);
    analogWrite(ENB,0);
}
  break;
    }
    delay(1);
  }
void vites(){
  xox = httpGETRequest(serverNamepot);
  xox2 = httpGETRequest(serverNamelih);
  Serial.println("ilk vites:");
  Serial.println(xox); 
  Serial.println("ikinci vites:");
  Serial.println(xox2);
      if (xox2 == "MV"){
        now = 2;
      }
      else if (xox == "X"){
        now = 1;
      }
      else if (xox == "A"){
      minimum = 75;
      maximum = 100;
      now = 0;
     }
      else if (xox == "B"){
      minimum = 75;
      maximum = 150;
      now = 0;
      }
      else if (xox == "C"){
      minimum = 75;
      maximum = 200;
      now = 0;
      }
      else if (xox == "D"){
      minimum = 75;
      maximum = 255;
      now = 0;
      }
      else if (xox == "E"){
      minimum = 75;
      maximum = 255;
      now = 0;
      }
      else if (xox == "F"){
      minimum = 95;
      maximum = 255;
      now = 0;
      }
      else if (xox == "G"){
      minimum = 145;
      maximum = 255;
      now = 0;
      }
      else{
      minimum = 75;
      maximum = 255;
      now = 0;
      }
    }


float pixyCheck() {
 static int i = 0;
 int j;
 uint16_t blocks;
 char buf[32];
 blocks = pixy.ccc.numBlocks;

 if(blocks)
 {
   signature = pixy.ccc.blocks[0].m_signature;
   height = pixy.ccc.blocks[0].m_height;
   width = pixy.ccc.blocks[0].m_width;
   x = pixy.ccc.blocks[0].m_x;
   y = pixy.ccc.blocks[0].m_y;
   cx = (x + (width / 2));
   cy = (y + (height / 2));
   cx = mapfloat(cx, 0, 320, -1, 1);
   cy = mapfloat(cy, 0, 200, 1, -1);
   area = width * height;

}
else {
  cont += 1;
if (cont == 100) {
  cont = 0;
  cx = 0;
  }
 }
 return cx;
}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

 String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {

    payload = http.getString();
  }
  // Free resources
  http.end();

  return payload;
}
