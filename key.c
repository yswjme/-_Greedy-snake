#include "main.h"
#include "key.h"
#include "tim.h"

uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key_up=1;//按键松开标志
	if(mode)key_up=1; //支持连按
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);//去抖动
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1;
	return 0;//无按键按下
}
