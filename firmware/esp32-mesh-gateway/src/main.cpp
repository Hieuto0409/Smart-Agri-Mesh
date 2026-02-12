#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "webpage.h"

#define UART_TX 17
#define UART_RX 16
#define baud 9600
#define DryValue 4000
#define WetValue 967

HardwareSerial SerialPort(2);

uint16_t HumiValue;
uint16_t LightValue;
uint16_t TempValue;

uint8_t PercentHumi;
uint8_t PercentLight;
uint8_t PercentTemp;

const char *WifiName = "ESP_wifi";
const char *passWifi = "123456789";

WebServer myServer(80);

void MapValue(uint16_t HMValue, uint16_t Lvalue, uint16_t Tvalue)
{
  PercentHumi = map(HMValue, DryValue, WetValue, 0, 100);
  PercentLight = map(Lvalue, 4064, 54, 0, 100);
  PercentTemp = map(Tvalue, 600, 100, 20, 70);
}
void webSetup()
{
  // Serial.println("HTTP Request");

  String html = html_page;

  myServer.send(200, "text/html", html);
}
void SendHumi()
{
  myServer.send(200, "text/plain", String(PercentHumi));
}
void SendLight()
{
  myServer.send(200, "text/plain", String(PercentLight));
}
void SendTemp()
{
  myServer.send(200, "text/plain", String(PercentTemp));
}
void GetValuePump()
{
  String status = myServer.arg("Status");
  Serial.print(status);
  SerialPort.print(status);
  myServer.send(200, "text/plain", "Da nhan");
}

void setup()
{
  Serial.begin(baud);
  SerialPort.begin(baud, SERIAL_8N1, UART_RX, UART_TX);
  WiFi.softAP(WifiName, passWifi);
  Serial.println("IP: ");
  Serial.println(WiFi.softAPIP());

  myServer.on("/", webSetup);
  myServer.on("/GetHumiValue", SendHumi);
  myServer.on("/GetLightValue", SendLight);
  myServer.on("/GetTempValue", SendTemp);
  myServer.on("/Pumpstatus", GetValuePump);
  myServer.begin();
}
void loop()
{
  myServer.handleClient();
  if (SerialPort.available() >= 8)
  {
    if (SerialPort.peek() == 0xAA)
    {
      uint8_t data[8];
      SerialPort.readBytes(data, 8);
      if (data[7] == 0x55)
      {

        uint8_t HightByteHumi = data[1];
        uint8_t LowByteHumi = data[2];

        uint8_t HightByteLight = data[3];
        uint8_t LowByteLight = data[4];

        uint8_t HightByteTemp = data[5];
        uint8_t LowByteTemp = data[6];
        HumiValue = (HightByteHumi << 8) | LowByteHumi;
        LightValue = (HightByteLight << 8) | LowByteLight;
        TempValue = (HightByteTemp << 8) | LowByteTemp;

        MapValue(HumiValue, LightValue, TempValue);
        // Serial.println(TempValue);
        // Serial.println(LightValue);
        Serial.println(TempValue);
      }
    }
  }
}
