#include "MyUart.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"

void MyUart_Conf(USART_TypeDef * uart_port, int baudrate){

	LL_USART_InitTypeDef My_LL_Usart_Init_Struct;
	
	if (uart_port==USART1) {
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
		LL_GPIO_InitTypeDef tx;
	  tx.Mode = LL_GPIO_MODE_ALTERNATE;
	  tx.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  tx.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  tx.Pin = LL_GPIO_PIN_9;
	  LL_GPIO_Init(GPIOA, &tx);
	}
	if (uart_port==USART2){ 
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	  LL_GPIO_InitTypeDef tx;
	  tx.Mode = LL_GPIO_MODE_ALTERNATE;
	  tx.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		tx.Pull = LL_GPIO_PULL_UP;
	  tx.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  tx.Pin = LL_GPIO_PIN_2;
	  LL_GPIO_Init(GPIOA, &tx);
	}
	if (uart_port==USART3){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
		LL_GPIO_InitTypeDef tx;
	  tx.Mode = LL_GPIO_MODE_ALTERNATE;
	  tx.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		tx.Pull = LL_GPIO_PULL_UP;
	  tx.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  tx.Pin = LL_GPIO_PIN_10;
	  LL_GPIO_Init(GPIOC, &tx);
	}

	My_LL_Usart_Init_Struct.BaudRate = baudrate;
	My_LL_Usart_Init_Struct.DataWidth = LL_USART_DATAWIDTH_8B ;
	My_LL_Usart_Init_Struct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	My_LL_Usart_Init_Struct.OverSampling = LL_USART_OVERSAMPLING_16;
	My_LL_Usart_Init_Struct.Parity = LL_USART_PARITY_NONE;
	My_LL_Usart_Init_Struct.StopBits = LL_USART_STOPBITS_1;
	My_LL_Usart_Init_Struct.TransferDirection = LL_USART_DIRECTION_TX_RX ;
	
	LL_USART_Init(uart_port,&My_LL_Usart_Init_Struct);
	LL_USART_Enable(uart_port);
	
	/*int periph_speed;
	if (uart_port==USART1) periph_speed = 36000000;
	if (uart_port==USART2) periph_speed = 72000000;
	if (uart_port==USART3) periph_speed = 72000000;

	LL_USART_SetBaudRate(uart_port, periph_speed, baudrate);
*/}

void MyUart_send_bytes(USART_TypeDef * uart_port,char* buf, int len){
	for(int i = 0; i < len; i++){
		LL_USART_TransmitData8(uart_port, buf[i]);
		while(!LL_USART_IsActiveFlag_TXE(uart_port));
	}	
}
