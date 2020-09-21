// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des p�riph�riques.
 Rem : OBLIGATION d'utiliser les d�finitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilit� du code.

 Pour les masques, utiliser �galement les d�finitions propos�e
 Rappel : pour mettre � 1  , reg = reg | Mask (ou Mask est le repr�sente le ou les bits � positionner � 1)
				  pour mettre � 0  , reg = reg&~ Mask (ou Mask est le repr�sente le ou les bits � positionner � 0)
 
*/ 
#include "stm32f103xb.h" 
#include "MyTimer.h"
#include <stdlib.h>
#include <stdio.h>

 void (*Ptr_Fct_TIM1) (void);
 void (*Ptr_Fct_TIM2) (void);
 void (*Ptr_Fct_TIM3) (void);
 void (*Ptr_Fct_TIM4) (void);

/**
	* @brief  Active l'horloge et r�gle l'ARR et le PSC du timer vis�
  * @note   Fonction � lancer avant toute autre. Le timer n'est pas encore lanc� (voir MyTimerStart)
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
	* 				int Arr   : valeur � placer dans ARR
	*				int Psc   : valeur � placer dans PSC
  * @retval None
  */
void MyTimer_Conf(TIM_TypeDef * Timer, uint16_t Arr, uint16_t Psc)
{
	if (Timer == TIM1){
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	}else if (Timer == TIM2){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}else if(Timer == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}else if(Timer == TIM4){
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	
	Timer->ARR = Arr;
	Timer->PSC = Psc;
}


/**
	* @brief  D�marre le timer consid�r�
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Start(TIM_TypeDef * Timer)
{
	Timer->CR1 |= TIM_CR1_CEN;
}


/**
	* @brief  Arr�t le timer consid�r�
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Stop(TIM_TypeDef * Timer)
{
	Timer->CR1 &= ~TIM_CR1_CEN;
}


/**
	* @brief  Configure le Timer consid�r� en interruption sur d�bordement.
  * @note   A ce stade, les interruptions ne sont pas valid�s (voir  MyTimer_IT_Enable )
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
	* 				void (*IT_function) (void) : nom (adresse) de la fonction � lancer sur interruption
	*         int Prio : priorit� associ�e � l'interruption
  * @retval None
  */
void MyTimer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio)
{
	//NVIC_SetPriority(TIM2_IRQn,Prio);
	if (Timer == TIM1){
		NVIC->IP[TIM1_UP_IRQn] |= (Prio << 4);
		Ptr_Fct_TIM1 = IT_function;
	}else if (Timer == TIM2){
		NVIC->IP[TIM2_IRQn] |= (Prio << 4);
		Ptr_Fct_TIM2 = IT_function;
	}else if(Timer == TIM3){
		NVIC->IP[TIM3_IRQn] |= (Prio << 4);
		Ptr_Fct_TIM3 = IT_function;
	}else if(Timer == TIM4){
		NVIC->IP[TIM4_IRQn] |= (Prio << 4);
		Ptr_Fct_TIM4 = IT_function;
	}
	
	
}


/**
	* @brief  Autorise les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_IT_Enable(TIM_TypeDef * Timer){
	
	Timer->DIER |= TIM_DIER_UIE;
	//NVIC_EnableIRQ(TIM2_IRQn);
	if (Timer == TIM1){
		NVIC->ISER[0] |= (1<<TIM1_UP_IRQn);
	}else if (Timer == TIM2){
		NVIC->ISER[0] |= (1<<TIM2_IRQn);
	}else if(Timer == TIM3){
		NVIC->ISER[0] |= (1<<TIM3_IRQn);
	}else if(Timer == TIM4){
		NVIC->ISER[0] |= (1<<TIM4_IRQn);
	}
	
}	


/**
	* @brief  Interdit les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_IT_Disable(TIM_TypeDef * Timer);



void TIM1_UP_IRQHandler(void)
{
	TIM1->SR &= ~TIM_SR_UIF;
	Ptr_Fct_TIM1();
}

void TIM2_IRQHandler(void)
{
	TIM2->SR &= ~TIM_SR_UIF;
	Ptr_Fct_TIM2();
}

void TIM3_IRQHandler(void)
{
	TIM3->SR &= ~TIM_SR_UIF;
	Ptr_Fct_TIM3();
}

void TIM4_IRQHandler(void)
{
	TIM4->SR &= ~TIM_SR_UIF;
	Ptr_Fct_TIM4();
}

