#include <Arduino.h>

#define UART_TX     17
#define UART_RX     16
#define baud        9600
#define DryValue    4095
#define WetValue    1400
HardwareSerial SerialPort(2);

uint16_t HumiValue;
uint8_t PercentHumi;
void MapValue(uint16_t HMValue){
  PercentHumi = map(HMValue,DryValue,WetValue,0,100);
}
void setup()
{
  Serial.begin(baud);
  SerialPort.begin(baud, SERIAL_8N1, UART_RX, UART_TX);
}
void loop()
{
  // if(SerialPort.available()){
  //   uint8_t test = SerialPort.read();
  //   Serial.println(test, HEX);
  // }
  if (SerialPort.available()>=3)
  {
    uint8_t Header = SerialPort.read();
    if (Header == 0xAA){
      uint8_t HightByte = SerialPort.read();
      uint8_t LowByte = SerialPort.read();

      HumiValue =(HightByte << 8) | LowByte ;
      MapValue(HumiValue);
      Serial.println(PercentHumi);
    }
  }
}
