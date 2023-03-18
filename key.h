#ifndef __KEY_H
#define __KEY_H	
#include "main.h"

#define KEY0 		HAL_GPIO_ReadPin(KEY0_GPIO_Port,GPIO_PIN_4)//PE4
#define KEY1 		HAL_GPIO_ReadPin(KEY1_GPIO_Port,GPIO_PIN_3)//PE3 
#define KEY2 		HAL_GPIO_ReadPin(KEY2_GPIO_Port,GPIO_PIN_2) //PE2
#define WK_UP 		HAL_GPIO_ReadPin(WK_UP_GPIO_Port,GPIO_PIN_0)	//PA0

#define KEY0_PRES 	4
#define KEY1_PRES	3
#define KEY2_PRES	2
#define WKUP_PRES   1

uint8_t KEY_Scan(uint8_t mode);
#endif
