/****************************************************************************
* Copyright (C), 2013 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V2,V2.1��V3��V5��MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: main.c
* ���ݼ���:	
*       
*	��ʾ����3����ɫLED��LED1-LED3) ������˸
    
	���壺	
	LED1-LED3 ---V6����V8 
	V6----- PB5-LED1
	V7----- PD6-LED2����V2��V2.1 V3  V5�壩
	V8----- PD3-LED3����V2��V2.1 V3  V5�壩
    
	����MDK�汾��        3.8
	���ڹٷ������汾�� 3.5
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.2    2011-7-28 sun68  �������ļ�
*
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

GPIO_InitTypeDef GPIO_InitStructure;//��ʼ��IO��
#define LED1_ON GPIO_SetBits(GPIOB, GPIO_Pin_5);//PB5 = 1
#define LED1_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_5);//PB5 = 0

#define LED2_ON GPIO_SetBits(GPIOD, GPIO_Pin_6);//PD6 = 1
#define LED2_OFF GPIO_ResetBits(GPIOD, GPIO_Pin_6);//PD6 = 0 

#define LED3_ON GPIO_SetBits(GPIOD, GPIO_Pin_3);//PD3 = 1  
#define LED3_OFF GPIO_ResetBits(GPIOD, GPIO_Pin_3);//PD3 = 0   

void RCC_Configuration(void);
void LED_Config(void);
void Delay(__IO uint32_t nCount);

/****************************************************************************
* ��    �ƣ�void LED_Config(void)
* ��    �ܣ�LED ���Ƴ�ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LED_Config(void){	
	//��������GPIOB��GPIODʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD , ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //LED1-V6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //������ģʽ����Ϊͨ��������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_3;	//LED2-V7, LED3-V8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//������ģʽ����Ϊͨ��������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 		//���߷�ת�ٶ�Ϊ50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			      //LCD�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);
//  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			              //LCD����ر�	
}

/****************************************************************************
* ��    �ƣ�int main(void)
* ��    �ܣ�������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
int flag;

int main(void)
{
  RCC_Configuration();   				//ϵͳʱ������
  LED_Config();							    //LED��������
	/*
  while (1) //LED 0.25s-1Hz
  {
  	LED1_ON; LED2_ON; LED3_ON;    //LED��
		Delay(250);	                  //��ʱ0.25s
		LED1_OFF; LED2_OFF; LED3_OFF;	//LED��
		Delay(250);	                  //��ʱ0.25s
  }
	*/
	///*
  flag = 0;
	while(1){
		 flag++;//һ��flag����0.01s
		 Delay(1);
		 //LED3 3HZ 1/(3*2) = 0.167
		 // ÿ����167��LEDת��״̬
		 if(flag%334 == 0){
			 LED3_ON; 
		 }
		 if(flag%334 == 167){
			 LED3_OFF;
		 }
		 //LED2 2HZ 1/(2*2) = 0.25 
		 //ÿ����250��LEDת��״̬
		 if(flag%500 == 0){
			 LED2_ON; 
		 }
		 if(flag%500 == 250){
			 LED2_OFF; 
		 }
		 //LED1 1HZ 1/(2*1) = 0.5 
		 //ÿ����500��LEDת��״̬
		 if(flag%1000 == 0){
			 LED1_ON; 
		 }
		 if(flag%1000 == 500){
			 LED1_OFF; 
		 }
		 if(flag == 1000000){
			 flag = 0;
		 }
	}
	
	//*/
	
}

/****************************************************************************
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void RCC_Configuration(void)
{   
  SystemInit();//ϵͳʱ�ӳ�ʼ��
}

/****************************************************************************
* ��    �ƣ�void Delay(__IO uint32_t nCount)
* ��    �ܣ���ʱ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void Delay(__IO uint32_t nCount)
{
   //for(; nCount != 0; nCount--);  
	 //while(nCount != 0)  nCount--; //ͨ���ݼ�ʵ����ʱ
	
	__IO uint32_t temp;
	__IO uint32_t nms = nCount;//�趨��ʱֵ����λ����
	SysTick->LOAD = 9000*nms;//SysTickƵ��Ϊ72/8=9MHz
	SysTick->VAL=0X00;//��ռĴ���
	SysTick->CTRL=0X01;//ʹ��
	do
	{
		temp=SysTick->CTRL;//��ȡ��ǰ����ֵ
	}while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��
  SysTick->CTRL=0x00; //�رռ�����
  SysTick->VAL =0X00; //��ռ�����
}

/******************* (C) COPYRIGHT 2013 �ܶ�STM32 *****END OF FILE****/
