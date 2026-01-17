#include <stdint.h>
#include <stm32f401re.h>
#include <stm32f401re_rcc.h>
#include <stm32f401re_usart.h>
#include <stm32f401re_gpio.h>
#include <misc.h>

// Định nghĩa chân cho USART1
#define UART_TX_PIN         GPIO_Pin_9
#define UART_RX_PIN         GPIO_Pin_10
#define UART_GPIO_PORT      GPIOA
#define UART_GPIO_CLOCK     RCC_AHB1Periph_GPIOA
#define USARTx_INSTANCE     USART1
#define USARTx_CLOCK        RCC_APB2Periph_USART1 // USART1 nằm trên bus APB2

#define USARTx_Baud         9600

void USART1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // 1. Bật Clock cho GPIOA (AHB1) và USART1 (APB2)
    RCC_AHB1PeriphClockCmd(UART_GPIO_CLOCK, ENABLE);
    RCC_APB2PeriphClockCmd(USARTx_CLOCK, ENABLE); // Đã sửa: USART1 dùng APB2

    // 2. Cấu hình chân PA9 (TX) và PA10 (RX)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_InitStructure.GPIO_Pin = UART_TX_PIN | UART_RX_PIN;
    GPIO_Init(UART_GPIO_PORT, &GPIO_InitStructure);

    // 3. Map chân vào chức năng Alternate Function cho USART1
    // Đã sửa: Phải dùng PinSource9 và PinSource10
    GPIO_PinAFConfig(UART_GPIO_PORT, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(UART_GPIO_PORT, GPIO_PinSource10, GPIO_AF_USART1);

    // 4. Cấu hình bộ USART1
    USART_InitStructure.USART_BaudRate = USARTx_Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USARTx_INSTANCE, &USART_InitStructure);

    // 5. Kích hoạt USART1
    USART_Cmd(USARTx_INSTANCE, ENABLE);
}

void Delay_Ms(uint32_t ms){
    uint32_t i,j;
    for(i=0;i<=ms;i++){
        for(j=0;j<5000;j++);
    }
}

int main(void)
{
    // Cấu hình Clock hệ thống dùng HSI (16MHz)
    RCC_HSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

    SystemCoreClockUpdate();
    USART1_Init();

    while(1) {
        // Kiểm tra cờ TXE trước khi gửi để tránh mất bit hoặc rác dữ liệu
        while(USART_GetFlagStatus(USARTx_INSTANCE, USART_FLAG_TXE) == RESET);

        // Gửi byte 0x55 (Test)
        USART_SendData(USARTx_INSTANCE, 0x55);

        Delay_Ms(1000);
    }
}
