#include "snake.h"
#include "bmp.h"

Snake snake;
uint8_t count=0;
int8_t score=-1;
uint16_t lcd_discolor[12]={  BLACK, BLUE,  BRED,      
						GRED,  GBLUE,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };
extern TIM_HandleTypeDef htim3;
void Welcomegame()
{
	uint8_t skey=0;
	uint8_t dir=0;
	uint8_t dir2=1;
	LCD_Clear(CYAN);
	LCD_Draw_Picture(0,80,480,269,(uint16_t *)gImage_start);//开始图片
	Chinese_Show_one(180, 410,0, 16, 0);//开
	Chinese_Show_one(200, 410,1, 16, 0);//始
	Chinese_Show_one(220, 410,2, 16, 0);//游
	Chinese_Show_one(240, 410,3, 16, 0);//戏
	LCD_ShowString(260,410,150,16,16,"[WK_UP]");
	
	Chinese_Show_one(180, 440,4, 16, 0);//蛇
	Chinese_Show_one(200, 440,5, 16, 0);//体
	Chinese_Show_one(220, 440,6, 16, 0);//颜
	Chinese_Show_one(240, 440,7, 16, 0);//色
	LCD_ShowString(260,440,150,16,16,"[KEY1]");
	
	Chinese_Show_one(180, 470,4, 16, 0);//蛇
	Chinese_Show_one(200, 470,5, 16, 0);//体
	Chinese_Show_one(220, 470,8, 16, 0);//速
	Chinese_Show_one(240, 470,9, 16, 0);//度
	LCD_ShowString(260,470,150,16,16,"[KEY2]");
	
	LCD_ShowString(180,500,150,16,16,"color:");
	LCD_ShowString(180,530,150,16,16,"speed:");
	LCD_ShowString(180,560,150,16,16,"KYY0 make");
//	snake.snakecolor=RED;
	while(1){
		skey=KEY_Scan(0);
		//KEY1判断
		if(skey==3)
			dir=!dir;
		if(dir==0){
			snake.snakecolor=RED;
			LCD_ShowString(260,500,150,16,16,"Red ");
		}
		if(dir==1){
			snake.snakecolor=BLUE;
			LCD_ShowString(260,500,150,16,16,"Blue");
		}
		//KEY2判断
		if(skey==2)
			dir2+=1;
		if(dir2==1){
			htim3.Init.Period = 5000-1;
			LCD_ShowString(260,530,150,16,16," slow ");//慢
		}
		else if(dir2==2){
			htim3.Init.Period = 1000-1;
			LCD_ShowString(260,530,150,16,16,"normal");
		}
		else if(dir2==3){
			htim3.Init.Period = 250-1;
			LCD_ShowString(260,530,150,16,16," high ");//快
		}
		if(dir2==3)
			dir2=0;
		if(skey==4){
			LCD_ShowString(330,500,150,16,16,"OK");//OK
			LCD_ShowString(330,530,150,16,16,"OK");//OK
			break;
		}
	}
	while(!WK_UP);
	
}
void Drowgound(void)//背景
{
	LCD_Clear(WHITE);
	LCD_Draw_Picture(30,10,64,64,(uint16_t *)gImage);//蛇像
	LCD_Draw_Picture(120,10,202,64,(uint16_t *)gImage_logo);//LOGO
	
	POINT_COLOR=DARKBLUE;
	LCD_ShowString(324,50,150,24,24,"Score:");//分数位置
	LCD_ShowNum(324+24*3,50,score,2,24);//实时分数
	LCD_DrawRectangle(320, 45, 324+24*5, 75);//分数框
	POINT_COLOR=RED;
	LCD_DrawRectangle(30, 80, 450, 760);//游戏边框
	LCD_DrawRectangle(31, 81, 449, 759);//游戏边框加粗
}
/*
初始化蛇
*/
void snakeInit(){
	snake.length=3;           //初始长度为3
	snake.snakecolor=snake.snakecolor;
	
	snake.snakeX[0]=240;
	snake.snakeX[1]=240;
	snake.snakeX[2]=240;
	
	snake.snakeY[0]=420;
	snake.snakeY[1]=430;
	snake.snakeY[2]=440;
	
	snake.headX=snake.snakeX[0]; //记录下头部的位置
	snake.headY=snake.snakeY[0]; //记录下头部的位置
	snake.dir=1;  //设置运动方向
	snake.tpdir=1;//设置初始按键方向
	
	snake.life=1; //1：蛇还活着；0：蛇死亡
	generateFood();
	refresh();    //调用函数显示出蛇和食物的位置
}
/*
刷新屏幕上蛇和食物的位置
*/
void refresh(){
	if(snake.life==1){
		uint8_t i;
		POINT_COLOR=DARKBLUE;
		LCD_ShowNum(324+24*3,50,score+1,2,24);//实时分数
		LCD_Fill(33,82,448,758,WHITE);//清屏游戏区域
		for(i=0;i<snake.length;i++){
			circle_full_color(snake.snakeX[i],snake.snakeY[i],5,snake.snakecolor);
		}
		circle_full_color(snake.foodX,snake.foodY,5,snake.foodcolor);
	}
}
/*
判断随机产生的食物是否处于蛇体内
*/
uint16_t isCover(uint16_t snakeX[],uint16_t snakeY[],uint16_t foodX,uint16_t foodY){
	uint8_t i;
	for(i=0;i<snake.length;i++){
		if(snakeX[i]==foodX&&snakeY[i]==foodY)
		{
		return 1;
		}
		
	}
	return 0;
}
void generateFood()//创建食物坐标
{
	uint16_t col=RNG_Get_RandomRange(0,11);
	snake.foodX=RNG_Get_RandomRange(40,440);
	snake.foodY=RNG_Get_RandomRange(90,750);
	while(isCover(snake.snakeX,snake.snakeY,snake.foodX,snake.foodY))
	{
		snake.foodX=RNG_Get_RandomRange(40,440);
		snake.foodY=RNG_Get_RandomRange(90,750);
	}
	snake.foodcolor=lcd_discolor[col];		
}
/*
在头部撞到墙之后执行死亡程序
*/
void dead(){
	HAL_TIM_Base_Stop_IT(&htim3);//关
	LCD_Clear(WHITE);
	LCD_Draw_Picture(0,200,478,256,(uint16_t *)gImage_over);//结束游戏
	snake.length=0;		
	snake.dir=0;  
	snake.life=0;	

	HAL_GPIO_WritePin(LED0_GPIO_Port, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BEEP_GPIO_Port,GPIO_PIN_8,GPIO_PIN_SET);
	delay_ms(500);
	HAL_GPIO_WritePin(LED0_GPIO_Port, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BEEP_GPIO_Port,GPIO_PIN_8,GPIO_PIN_RESET);
	
	POINT_COLOR=DARKBLUE;
	LCD_ShowString(180,500,200,24,24,"You dead!!!");
	LCD_ShowString(165,550,200,24,24,"WK_UP restart"); //重新开始游戏
	
}
/*
判断是否头部是否触碰到自己
*/
uint16_t isNearSelf(){
	uint16_t i;
	for(i=1;i<snake.length;i++){
		if(snake.snakeX[i]==snake.headX&&snake.snakeY[i]==snake.headY)
			return 0;	
	}
	return 1;
}
/*
每隔200ms将会执行一次，以此实现蛇的运动
*/
void snakeGo(int direct){
	uint8_t i;
	float dis;
	if(snake.life==1){
		if(direct==1 && snake.dir==3) direct=snake.dir;//蛇运动的方向仍是以前运动的方向
		else if(direct==3 && snake.dir==1) direct=snake.dir;
		else if(direct==2 && snake.dir==4) direct=snake.dir;
		else if(direct==4 && snake.dir==2) direct=snake.dir;
		else
			snake.dir=direct;//将蛇运动的方向改变为按下的方向
		
		switch(direct){
			case 1:
				snake.headY-=10;  //上：蛇的身体是有半径为5的圆组成，每运动一格的单位为10
			break;
			case 3:
				snake.headY+=10;  //下：蛇的身体是有半径为5的圆组成，每运动一格的单位为10
			break;
			case 2:
				snake.headX-=10;  //左：蛇的身体是有半径为5的圆组成，每运动一格的单位为10
			break;
			case 4:
				snake.headX+=10;  //右：蛇的身体是有半径为5的圆组成，每运动一格的单位为10
			break;
		}	
	dis=sqrt((snake.headX - snake.foodX)*(snake.headX - snake.foodX)+(snake.headY - snake.foodY)*(snake.headY - snake.foodY));
	if(dis<8)//如果吃到了食物
	{ 
		snake.length++;  //长度加1
		score++;		//分数加1	
		for(i=1;i<snake.length;i++){ //除头部以外的坐标前移
			snake.snakeX[snake.length-i]=snake.snakeX[snake.length-i-1];
			snake.snakeY[snake.length-i]=snake.snakeY[snake.length-i-1];
		}
		snake.snakeX[0]=snake.foodX; //头部坐标等于食物的坐标
		snake.snakeY[0]=snake.foodY;
		generateFood();  //再生成一个食物坐标
	}
	else
	{
		for(i=1;i<snake.length;i++){
			snake.snakeX[snake.length-i]=snake.snakeX[snake.length-i-1];
			snake.snakeY[snake.length-i]=snake.snakeY[snake.length-i-1];
		}
		snake.snakeX[0]=snake.headX;
		snake.snakeY[0]=snake.headY;			
	}	
	}
	if(snake.headY<760&&snake.headY>80&&snake.headX<450&&snake.headX>30&&isNearSelf()){
		snake.life=1;
	}
	else
		dead();	//触碰到墙壁或自己游戏结束
}

/*
开始游戏
*/
void GameStart()
{
	uint8_t key=0;
	while(1){
		key=KEY_Scan(0);
		if(key!=0)			//被按下
		{	
			switch(key)
			{
				case WKUP_PRES: snake.tpdir=1;break;
				case KEY1_PRES: snake.tpdir=3;break;
				case KEY2_PRES: snake.tpdir=2;break;
				case KEY0_PRES: snake.tpdir=4;break;
			}
			if(snake.life==0){
					while(WK_UP);
					HAL_TIM_Base_Start_IT(&htim3);
					Drowgound();
					score = -1;
					
					snakeInit();					
				}
  			   
		}		
		if(count==2){  //定时器设置的100ms的中断，200ms运行一次蛇的运动函数
			count=0;			//重新计数
			snakeGo(snake.tpdir);
			refresh();	
		}
	}
}
