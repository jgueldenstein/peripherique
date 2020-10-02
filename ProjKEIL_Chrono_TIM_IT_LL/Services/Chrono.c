// A COMPLETER

/*
Service permettant de chornométrer jusqu'à 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 à TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "Chrono.h"
#include "MyTimer.h"
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_utils.h"

// variable privée de type Time qui mémorise la durée mesurée
static Time Chrono_Time; // rem : static rend la visibilité de la variable Chrono_Time limitée à ce fichier 

// variable privée qui mémorise pour le module le timer utilisé par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par défaut au cas où l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.

// déclaration callback appelé toute les 10ms
void Chrono_Task_10ms(void);

int running = 0;
int was_zero = 0;

/**
	* @brief  Configure le chronomètre. 
  * @note   A lancer avant toute autre fonction.
	* @param  Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Chrono_Conf(TIM_TypeDef * Timer)
{
	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
	
	// Fixation du Timer
	Chrono_Timer=Timer;

	Chrono_Conf_io();
	// Réglage Timer pour un débordement à 10ms
	MyTimer_Conf(Chrono_Timer,999, 719);
	
	// Réglage interruption du Timer avec callback : Chrono_Task_10ms()
	MyTimer_IT_Conf(Chrono_Timer, Chrono_Task_10ms,3);
	
	// Validation IT
	MyTimer_IT_Enable(Chrono_Timer);
	
	
}


/**
	* @brief  Démarre le chronomètre. 
  * @note   si la durée dépasse 59mn 59sec 99 Hund, elle est remise à zéro et repart
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Start(void)
{
	MyTimer_Start(Chrono_Timer);
	running = 1;
}


/**
	* @brief  Arrête le chronomètre. 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Stop(void)
{
	MyTimer_Stop(Chrono_Timer);
	running = 0;
}


/**
	* @brief  Remet le chronomètre à 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void)
{
  // Arrêt Chrono
	MyTimer_Stop(Chrono_Timer);

	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
}


/**
	* @brief  Renvoie l'adresse de la variable Time privée gérée dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void)
{
	return &Chrono_Time;
}




/**
	* @brief  incrémente la variable privée Chron_Time modulo 60mn 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Task_10ms(void)
{ 
	Chrono_Time.Hund++;
	if (Chrono_Time.Hund==100)
	{
		Chrono_Time.Sec++;
		Chrono_Time.Hund=0;
	}
	if (Chrono_Time.Sec==60)
	{
		Chrono_Time.Min++;
		Chrono_Time.Sec=0;
	}
	if (Chrono_Time.Min==60)
	{
		Chrono_Time.Hund=0;
	}
	if ( Chrono_Time.Sec % 2) {
		LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_10);
	} else {
		LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_10);
	}
	
}

/**
  * @brief configurer les 3 IO pour recevoir les 2 BP et la LED
  */
void Chrono_Conf_io(void) {
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	
	LL_GPIO_InitTypeDef button_rouge_conf;
	button_rouge_conf.Mode = LL_GPIO_MODE_FLOATING;
	button_rouge_conf.Pin = LL_GPIO_PIN_8;
	LL_GPIO_Init(GPIOC, &button_rouge_conf);
	
	
	LL_GPIO_InitTypeDef button_bleu_conf;
	button_bleu_conf.Mode = LL_GPIO_MODE_FLOATING;
	button_bleu_conf.Pin = LL_GPIO_PIN_13;
	LL_GPIO_Init(GPIOC, &button_bleu_conf);
	
	LL_GPIO_InitTypeDef led;
	led.Mode = LL_GPIO_MODE_OUTPUT;
	led.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	led.Speed = LL_GPIO_SPEED_FREQ_LOW;
	led.Pin = LL_GPIO_PIN_10;
	LL_GPIO_Init(GPIOC, &led);
}

void Chrono_Background(void){
	if(LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_8)) {
			if (was_zero) {
				LL_mDelay(2);
				if(LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_8)) {
					if(running) {
						Chrono_Stop();
					} else {
						Chrono_Start();
					}
				}
			}
			was_zero = 0;
	}
	else
	{
		was_zero = 1;
	}
	if(!(LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_13))) { // negative logic for blue button
		Chrono_Reset();
	}
}
