// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des périphériques.
 Rem : OBLIGATION d'utiliser les définitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilité du code.

 Pour les masques, utiliser également les définitions proposée
 Rappel : pour mettre à 1  , reg = reg | Mask (ou Mask est le représente le ou les bits à positionner à 1)
				  pour mettre à 0  , reg = reg&~ Mask (ou Mask est le représente le ou les bits à positionner à 0)
 
*/ 
#include "stm32f103xb.h" 
#include "MyTimer.h"

 void (*Ptr_Fct_TIM2) (void);

/**
	* @brief  Active l'horloge et règle l'ARR et le PSC du timer visé
  * @note   Fonction à lancer avant toute autre. Le timer n'est pas encore lancé (voir MyTimerStart)
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
	* 				int Arr   : valeur à placer dans ARR
	*				int Psc   : valeur à placer dans PSC
  * @retval None
  */
void MyTimer_Conf(TIM_TypeDef * Timer, uint16_t Arr, uint16_t Psc)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	Timer->ARR = Arr;
	Timer->PSC = Psc;
}


/**
	* @brief  Démarre le timer considéré
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Start(TIM_TypeDef * Timer)
{
	Timer->CR1 |= TIM_CR1_CEN;
}


/**
	* @brief  Arrêt le timer considéré
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Stop(TIM_TypeDef * Timer)
{
	Timer->CR1 &= ~TIM_CR1_CEN;
}


/**
	* @brief  Configure le Timer considéré en interruption sur débordement.
  * @note   A ce stade, les interruptions ne sont pas validés (voir  MyTimer_IT_Enable )
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
	* 				void (*IT_function) (void) : nom (adresse) de la fonction à lancer sur interruption
	*         int Prio : priorité associée à l'interruption
  * @retval None
  */
void MyTimer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio)
{
	NVIC_SetPriority(TIM2_IRQn,Prio);
	
	Ptr_Fct_TIM2 = IT_function;
}


/**
	* @brief  Autorise les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_IT_Enable(TIM_TypeDef * Timer){
	Timer->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
	//NVIC->ISER[0] |= (1<<28);
}	


/**
	* @brief  Interdit les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_IT_Disable(TIM_TypeDef * Timer);

void TIM2_IRQHandler(void)
{
	TIM2->SR &= ~TIM_SR_UIF;
	Ptr_Fct_TIM2();
}

