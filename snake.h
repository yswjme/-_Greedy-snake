#ifndef __snack_H
#define __snack_H

#include "main.h"
#include "key.h"
#include "rng.h"
#include "tim.h"
#include "gpio.h"
#include "stdlib.h"
#include "lcd.h"
#include "math.h"

typedef struct  
{	
	uint8_t life;
	uint16_t headX;			
	uint16_t headY;		
	uint16_t snakeX[100];
	uint16_t snakeY[100];
	uint16_t snakecolor;
	uint8_t length;
	int dir;
	int tpdir;
	uint32_t foodX;
	uint32_t foodY;
	uint16_t foodcolor;

}Snake;

void Welcomegame(void);
void Drowgound(void);
void snakeInit(void);
void generateFood(void);
void refresh(void);
void GameStart(void);

void dead(void);
#endif
