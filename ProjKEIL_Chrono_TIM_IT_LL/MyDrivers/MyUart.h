#ifndef MYUART_H
#define MYUART_H

#include "stm32f1xx_ll_usart.h"

void MyUart_Conf(USART_TypeDef * uart_port, int baudrate);

void MyUart_send_bytes(USART_TypeDef * uart_port,char* buf, int len);

#endif