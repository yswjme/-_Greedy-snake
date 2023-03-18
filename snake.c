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
	LCD_Draw_Picture(0,80,480,269,(uint16_t *)gImage_start);//��ʼͼƬ
	Chinese_Show_one(180, 410,0, 16, 0);//��
	Chinese_Show_one(200, 410,1, 16, 0);//ʼ
	Chinese_Show_one(220, 410,2, 16, 0);//��
	Chinese_Show_one(240, 410,3, 16, 0);//Ϸ
	LCD_ShowString(260,410,150,16,16,"[WK_UP]");
	
	Chinese_Show_one(180, 440,4, 16, 0);//��
	Chinese_Show_one(200, 440,5, 16, 0);//��
	Chinese_Show_one(220, 440,6, 16, 0);//��
	Chinese_Show_one(240, 440,7, 16, 0);//ɫ
	LCD_ShowString(260,440,150,16,16,"[KEY1]");
	
	Chinese_Show_one(180, 470,4, 16, 0);//��
	Chinese_Show_one(200, 470,5, 16, 0);//��
	Chinese_Show_one(220, 470,8, 16, 0);//��
	Chinese_Show_one(240, 470,9, 16, 0);//��
	LCD_ShowString(260,470,150,16,16,"[KEY2]");
	
	LCD_ShowString(180,500,150,16,16,"color:");
	LCD_ShowString(180,530,150,16,16,"speed:");
	LCD_ShowString(180,560,150,16,16,"KYY0 make");
//	snake.snakecolor=RED;
	while(1){
		skey=KEY_Scan(0);
		//KEY1�ж�
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
		//KEY2�ж�
		if(skey==2)
			dir2+=1;
		if(dir2==1){
			htim3.Init.Period = 5000-1;
			LCD_ShowString(260,530,150,16,16," slow ");//��
		}
		else if(dir2==2){
			htim3.Init.Period = 1000-1;
			LCD_ShowString(260,530,150,16,16,"normal");
		}
		else if(dir2==3){
			htim3.Init.Period = 250-1;
			LCD_ShowString(260,530,150,16,16," high ");//��
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
void Drowgound(void)//����
{
	LCD_Clear(WHITE);
	LCD_Draw_Picture(30,10,64,64,(uint16_t *)gImage);//����
	LCD_Draw_Picture(120,10,202,64,(uint16_t *)gImage_logo);//LOGO
	
	POINT_COLOR=DARKBLUE;
	LCD_ShowString(324,50,150,24,24,"Score:");//����λ��
	LCD_ShowNum(324+24*3,50,score,2,24);//ʵʱ����
	LCD_DrawRectangle(320, 45, 324+24*5, 75);//������
	POINT_COLOR=RED;
	LCD_DrawRectangle(30, 80, 450, 760);//��Ϸ�߿�
	LCD_DrawRectangle(31, 81, 449, 759);//��Ϸ�߿�Ӵ�
}
/*
��ʼ����
*/
void snakeInit(){
	snake.length=3;           //��ʼ����Ϊ3
	snake.snakecolor=snake.snakecolor;
	
	snake.snakeX[0]=240;
	snake.snakeX[1]=240;
	snake.snakeX[2]=240;
	
	snake.snakeY[0]=420;
	snake.snakeY[1]=430;
	snake.snakeY[2]=440;
	
	snake.headX=snake.snakeX[0]; //��¼��ͷ����λ��
	snake.headY=snake.snakeY[0]; //��¼��ͷ����λ��
	snake.dir=1;  //�����˶�����
	snake.tpdir=1;//���ó�ʼ��������
	
	snake.life=1; //1���߻����ţ�0��������
	generateFood();
	refresh();    //���ú�����ʾ���ߺ�ʳ���λ��
}
/*
ˢ����Ļ���ߺ�ʳ���λ��
*/
void refresh(){
	if(snake.life==1){
		uint8_t i;
		POINT_COLOR=DARKBLUE;
		LCD_ShowNum(324+24*3,50,score+1,2,24);//ʵʱ����
		LCD_Fill(33,82,448,758,WHITE);//������Ϸ����
		for(i=0;i<snake.length;i++){
			circle_full_color(snake.snakeX[i],snake.snakeY[i],5,snake.snakecolor);
		}
		circle_full_color(snake.foodX,snake.foodY,5,snake.foodcolor);
	}
}
/*
�ж����������ʳ���Ƿ���������
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
void generateFood()//����ʳ������
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
��ͷ��ײ��ǽ֮��ִ����������
*/
void dead(){
	HAL_TIM_Base_Stop_IT(&htim3);//��
	LCD_Clear(WHITE);
	LCD_Draw_Picture(0,200,478,256,(uint16_t *)gImage_over);//������Ϸ
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
	LCD_ShowString(165,550,200,24,24,"WK_UP restart"); //���¿�ʼ��Ϸ
	
}
/*
�ж��Ƿ�ͷ���Ƿ������Լ�
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
ÿ��200ms����ִ��һ�Σ��Դ�ʵ���ߵ��˶�
*/
void snakeGo(int direct){
	uint8_t i;
	float dis;
	if(snake.life==1){
		if(direct==1 && snake.dir==3) direct=snake.dir;//���˶��ķ���������ǰ�˶��ķ���
		else if(direct==3 && snake.dir==1) direct=snake.dir;
		else if(direct==2 && snake.dir==4) direct=snake.dir;
		else if(direct==4 && snake.dir==2) direct=snake.dir;
		else
			snake.dir=direct;//�����˶��ķ���ı�Ϊ���µķ���
		
		switch(direct){
			case 1:
				snake.headY-=10;  //�ϣ��ߵ��������а뾶Ϊ5��Բ��ɣ�ÿ�˶�һ��ĵ�λΪ10
			break;
			case 3:
				snake.headY+=10;  //�£��ߵ��������а뾶Ϊ5��Բ��ɣ�ÿ�˶�һ��ĵ�λΪ10
			break;
			case 2:
				snake.headX-=10;  //���ߵ��������а뾶Ϊ5��Բ��ɣ�ÿ�˶�һ��ĵ�λΪ10
			break;
			case 4:
				snake.headX+=10;  //�ң��ߵ��������а뾶Ϊ5��Բ��ɣ�ÿ�˶�һ��ĵ�λΪ10
			break;
		}	
	dis=sqrt((snake.headX - snake.foodX)*(snake.headX - snake.foodX)+(snake.headY - snake.foodY)*(snake.headY - snake.foodY));
	if(dis<8)//����Ե���ʳ��
	{ 
		snake.length++;  //���ȼ�1
		score++;		//������1	
		for(i=1;i<snake.length;i++){ //��ͷ�����������ǰ��
			snake.snakeX[snake.length-i]=snake.snakeX[snake.length-i-1];
			snake.snakeY[snake.length-i]=snake.snakeY[snake.length-i-1];
		}
		snake.snakeX[0]=snake.foodX; //ͷ���������ʳ�������
		snake.snakeY[0]=snake.foodY;
		generateFood();  //������һ��ʳ������
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
		dead();	//������ǽ�ڻ��Լ���Ϸ����
}

/*
��ʼ��Ϸ
*/
void GameStart()
{
	uint8_t key=0;
	while(1){
		key=KEY_Scan(0);
		if(key!=0)			//������
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
		if(count==2){  //��ʱ�����õ�100ms���жϣ�200ms����һ���ߵ��˶�����
			count=0;			//���¼���
			snakeGo(snake.tpdir);
			refresh();	
		}
	}
}
