/****************************************************************************
* Copyright (C), 2013 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V2,V2.1��V3,V5��MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: main.c
* ���ݼ���:	
*       
*	��������ʾ��ͨ��ͨ�ö�ʱ��3��2ͨ����ʵ��12.5Hz���εĲ��������ڿ���LED1����˸��
    
	����MDK�汾��        3.8
	���ڹٷ������汾�� 3.5
  
    Ӳ�����ӹ�ϵ 
    PB5---TIM3-CH2
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.2    2011-7-29 sun68  �������ļ�
*
*/

#include  "stm32f10x.h"
#include  "stm32f10x_exti.h" //�ⲿ�ж�
#include  "stm32f10x_tim.h" //��ʱ��
#include  "stm32f10x_rcc.h"	//ϵͳʱ��
#include  "misc.h"

unsigned int _it0;
unsigned int CCR2_Val; 
extern void time_ini(void);

/****************************************************************************
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void RCC_Configuration(void){

  SystemInit(); 		//ϵͳʱ�ӳ�ʼ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 	//����ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);	//IO��ʱ��
}

/****************************************************************************
* ��    �ƣ�void NVIC_Configuration(void)
* ��    �ܣ��ж�Դ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				 	 //4λ��Ӧ���ȼ�

  NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn ;			   	 //���ö�ʱ���ж� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //��Ӧ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					 //�ж�ʹ��
  NVIC_Init(&NVIC_InitStructure);  	
}

/****************************************************************************
* ��    �ƣ�int main(void)
* ��    �ܣ�������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
int main(void)
{    
  RCC_Configuration();   	   //ϵͳʱ�����ü�����ʱ��ʹ��					     
  NVIC_Configuration();			 //�ж�Դ����
	
	CCR2_Val=5000; 
	//CR2_Val=500; 
	time_ini();								 //��ʱ����ʼ��
  while(1);
}
/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/

