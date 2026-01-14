#include <Arduino.h>

#define UART_TX 17
#define UART_RX 16
HardwareSerial SerialPort(2);
void setup()
{
  Serial.begin(115200);
  SerialPort.begin(115200, SERIAL_8N1, UART_RX, UART_TX);
}

void loop()
{
  if (SerialPort.available())
  {
    uint8_t data = SerialPort.read();

    Serial.print("Nhan duoc: 0x");
    Serial.println(data, HEX);
    delay(1000);
  }
}
