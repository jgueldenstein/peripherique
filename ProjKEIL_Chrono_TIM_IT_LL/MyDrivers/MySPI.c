
#include "MySPI.h"

void MySPI_conf(SPI_TypeDef * spi_port){

	LL_SPI_InitTypeDef My_LL_SPI_Init_Struct;
	
	if(spi_port == SPI1){
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
			LL_GPIO_InitTypeDef mosi;
			mosi.Mode = LL_GPIO_MODE_ALTERNATE;
			mosi.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			mosi.Speed = LL_GPIO_SPEED_FREQ_LOW;
			mosi.Pin = LL_GPIO_PIN_7;
			LL_GPIO_Init(GPIOA, &mosi);
		
			LL_GPIO_InitTypeDef sck;
			sck.Mode = LL_GPIO_MODE_ALTERNATE;
			sck.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			sck.Speed = LL_GPIO_SPEED_FREQ_LOW;
			sck.Pin = LL_GPIO_PIN_5;
			LL_GPIO_Init(GPIOA, &sck);
		
			LL_GPIO_InitTypeDef nss;
			nss.Mode = LL_GPIO_MODE_ALTERNATE;
			nss.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			nss.Speed = LL_GPIO_SPEED_FREQ_LOW;
			nss.Pin = LL_GPIO_PIN_4;
			LL_GPIO_Init(GPIOA, &nss);
	}
	if(spi_port == SPI2){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
		
	}
	
	My_LL_SPI_Init_Struct.TransferDirection = LL_SPI_HALF_DUPLEX_TX;
	My_LL_SPI_Init_Struct.Mode = LL_SPI_MODE_MASTER;
	My_LL_SPI_Init_Struct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
	My_LL_SPI_Init_Struct.ClockPolarity = LL_SPI_POLARITY_HIGH;
	My_LL_SPI_Init_Struct.ClockPhase = LL_SPI_PHASE_2EDGE;
	My_LL_SPI_Init_Struct.NSS = 0; //??;
	My_LL_SPI_Init_Struct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;
	My_LL_SPI_Init_Struct.BitOrder = LL_SPI_MSB_FIRST;
	My_LL_SPI_Init_Struct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	
	LL_SPI_Init(spi_port, &My_LL_SPI_Init_Struct);
	LL_SPI_Enable(spi_port);

}

void MySPI_send_bytes(char buf[], int len) {
	
}