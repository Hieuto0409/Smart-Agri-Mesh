#include <stdint.h>
#include <stm32f401re.h>
#include <stm32f401re_rcc.h>
#include <stm32f401re_usart.h>
#include <stm32f401re_gpio.h>
#include <stm32f401re_adc.h>
#include <timer.h>
#include <misc.h>

// Định nghĩa chân cho USART1
#define UART_TX_PIN         	GPIO_Pin_9
#define UART_RX_PIN         	GPIO_Pin_10
#define UART_GPIO_PORT      	GPIOA
#define UART_GPIO_CLOCK     	RCC_AHB1Periph_GPIOA
#define USARTx_INSTANCE     	USART1
#define USARTx_CLOCK        	RCC_APB2Periph_USART1 // USART1 nằm trên bus APB2
// định nghĩa chân sensor
#define SENSOR_GPIO				GPIOA
#define SENSOR_GPIO_PIN			GPIO_Pin_1
#define SENSOR_GPIO_CLOCK     	RCC_AHB1Periph_GPIOA
// định nghĩa chân relay
#define RELAY_GPIO				GPIOA
#define RELAY_GPIO_PIN			GPIO_Pin_4
#define RELAY_GPIO_CLOCK		RCC_AHB1Periph_GPIOA
#define USARTx_Baud         	9600

volatile uint16_t HumiValue;
volatile uint8_t ReceiveData ;
uint8_t HightByte;
uint8_t LowByte;

void Relay_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = RELAY_GPIO_PIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;

	RCC_AHB1PeriphClockCmd(RELAY_GPIO_CLOCK, ENABLE);
	GPIO_Init(RELAY_GPIO, &GPIO_InitStructure);
}
void SENSOR_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin = SENSOR_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;

	RCC_AHB1PeriphClockCmd(SENSOR_GPIO_CLOCK, ENABLE);
	GPIO_Init(SENSOR_GPIO, &GPIO_InitStructure);
}
static void ADC_HUMI_Init(void){
	ADC_InitTypeDef				ADC_InitStructure;
	ADC_CommonInitTypeDef		ADC_CommonInitStructure;
	//Cap clock cho ADC1 hoat dong
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// ADC Deinitialization
	ADC_DeInit(); // khoi tao cac gia trị ADC ve mac dinh

	// ADC Common Init
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; // Moi ADC hoat dong doc lap
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; // cai dat bo chia tan, quyet dinh xung nhip cho ADC (APB2 = 84Mhz)
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // khong su dung DMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;// chon thoi gian giua 2 lan chuyn doi la 5 Cycles
	ADC_CommonInit(&ADC_CommonInitStructure);

	// ADC1 Init
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; // lua chon do phan giai la 12 bit
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; // cho phep quet che do quet don kenh (1 kenh) va chuyen doi du lieu lien tuc
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // chon che do Single Continuous
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;// thuc hien chuyen doi mot lan trong luc quet
	ADC_Init(ADC1, &ADC_InitStructure);


	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_144Cycles);

	// Enable ADC1
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
}
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
uint16_t Cover_Humidity(void){
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}
void SendMess (uint16_t humivalue){

	HightByte= (uint8_t)(humivalue >> 8);
	LowByte = (uint8_t)(humivalue & 0xFF);

	while(USART_GetFlagStatus(USARTx_INSTANCE, USART_FLAG_TXE) == RESET);

	USART_SendData(USARTx_INSTANCE, 0xAA);

	// Kiểm tra cờ TXE trước khi gửi để tránh mất bit hoặc rác dữ liệu
	 while(USART_GetFlagStatus(USARTx_INSTANCE, USART_FLAG_TXE) == RESET);

	 USART_SendData(USARTx_INSTANCE, HightByte);

	 while(USART_GetFlagStatus(USARTx_INSTANCE, USART_FLAG_TXE) == RESET);

	 USART_SendData(USARTx_INSTANCE, LowByte);


//	 received_data = USART_ReceiveData(USART1);
}
static void MultiSensorScan(void)
{
    uint32_t dwTimeCurrent;
    static uint32_t dwTimeTotal, dwTimeInit;

    dwTimeCurrent = GetMilSecTick();

    if (dwTimeCurrent >= dwTimeInit)
    {
        dwTimeTotal += dwTimeCurrent - dwTimeInit;
    }
    else
    {
        dwTimeTotal += 0xFFFFFFFFU - dwTimeCurrent + dwTimeInit;
    }

    if (dwTimeTotal >= 2000)
    {
        // Time scan 2s
        dwTimeTotal = 0;
        HumiValue= Cover_Humidity();
        SendMess(HumiValue);
    }
    dwTimeInit = dwTimeCurrent;
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
    SENSOR_Init();
    ADC_HUMI_Init();
    TimerInit();
    Relay_Init();
    while(1) {
    	processTimerScheduler();
    	MultiSensorScan();
    	if(USART_GetFlagStatus(USARTx_INSTANCE, USART_FLAG_RXNE) != RESET){
    		ReceiveData = (uint8_t) USART_ReceiveData(USART1);
    		if(ReceiveData == '0'){
    			GPIO_ResetBits(RELAY_GPIO, RELAY_GPIO_PIN);
    		}
    		else if (ReceiveData == '1'){
    			GPIO_SetBits(RELAY_GPIO, RELAY_GPIO_PIN);
    		}
    	}
    }
}
