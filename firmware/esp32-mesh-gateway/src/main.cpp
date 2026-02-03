#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "webpage.h"

#define UART_TX     17
#define UART_RX     16
#define baud        9600
#define DryValue    4095
#define WetValue    1400

HardwareSerial SerialPort(2);

uint16_t HumiValue;
uint8_t PercentHumi;
const char *WifiName = "ESP_wifi";
const char *passWifi = "123456789";

WebServer myServer(80);

void MapValue(uint16_t HMValue){
  PercentHumi = map(HMValue,DryValue,WetValue,0,100);
}
void webSetup(){
  // Serial.println("HTTP Request");

  String html = html_page;

  myServer.send(200,"text/html",html);
}
void SendHumi (){
  myServer.send(200,"text/plain",String(PercentHumi));
}
void GetValuePump(){
  String status = myServer.arg("Status");
  Serial.println(status);
  myServer.send(200, "text/plain", "Da nhan");
}
void setup()
{
  Serial.begin(baud);
  SerialPort.begin(baud, SERIAL_8N1, UART_RX, UART_TX);
  WiFi.softAP(WifiName,passWifi);
  Serial.println("IP: ");
  Serial.println(WiFi.softAPIP());

  myServer.on("/",webSetup);
  myServer.on("/GetHumiValue",SendHumi);
  myServer.on("/Pumpstatus",GetValuePump);
  myServer.begin();
}
void loop()
{
  myServer.handleClient();
  if (SerialPort.available()>=3)
  {
    uint8_t Header = SerialPort.read();
    if (Header == 0xAA){
      uint8_t HightByte = SerialPort.read();
      uint8_t LowByte = SerialPort.read();

      HumiValue =(HightByte << 8) | LowByte ;
      MapValue(HumiValue);
      // Serial.println(PercentHumi);
    }
  }
}
