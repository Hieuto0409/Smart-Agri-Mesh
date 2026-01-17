#include <Arduino.h>

#define UART_TX 17
#define UART_RX 16
#define baud 9600
HardwareSerial SerialPort(2);
void setup()
{
  Serial.begin(baud);
  SerialPort.begin(baud, SERIAL_8N1, UART_RX, UART_TX);
}

void loop()
{
  // SerialPort.write(0x10);
  // delay(1000);
  if (SerialPort.available())
  {
    uint8_t data = SerialPort.read();

    // Serial.print("Nhan duoc: 0x");
    Serial.println(data, HEX);
    // delay(1000);
  }
}
