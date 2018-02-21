#ifndef __PWM_H
#define	__PWM_H
#include "sys.h"
#include "stm32f10x.h"
void TIM3_PWM_Init(u16 * CCR3_Val,u16 * CCR4_Val);

void TIM3_Mode_Config(u16 CCR3_Val,u16 CCR4_Val);

void TIM3_GPIO_Config(void);

#endif 
