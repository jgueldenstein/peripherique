#ifndef MYSPI_H
#define MYSPI_H

#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_utils.h"

void MySPI_conf(SPI_TypeDef * spi_port);

void MySPI_send_bytes(char buf[], int len);
 #endif