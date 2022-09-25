/****************************************************************************
* Copyright (C), 2011 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V2,V2.1��V3��MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: tim.c
* ���ݼ���:	
*    TIM3�ĳ�ʼ��	      
	
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.2    2011-7-29 sun68  �������ļ�
*
*/				 
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"

void time_ini(void);
extern unsigned int CCR1_Val;
TIM_TimeBaseInitTypeDef  TIM4_TimeBaseStructure;//��ʱ����ʼ��
TIM_OCInitTypeDef  TIM4_OCInitStructure;//��ʱ������Ƚ�
TIM_BDTRInitTypeDef TIM4_BDTRInitStructure;//��ʱ����·������


/****************************************************************************
* ��    �ƣ�void time_ini(void)
* ��    �ܣ�TIM3��ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void time_ini(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ͨ��ABP1�Զ�ʱ��4ʱ��ʹ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					//PD12����ΪTIM4��ͨ��1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//����ʽ���������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�����ٶ�50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //TIM4�ֲ����ù��ܿ���
  GPIO_PinRemapConfig(GPIO_Remap_TIM4 , ENABLE);			 
  				
  TIM4_TimeBaseStructure.TIM_Prescaler = 2;						    //Ԥ��Ƶ��TIM4_PSC=2	 
  TIM4_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���������ϼ���ģʽ
  TIM4_TimeBaseStructure.TIM_Period =47999;				       //�Զ���װ�ؼĴ��� ȷ��Ƶ��Ϊ500Hz 		     
  TIM4_TimeBaseStructure.TIM_ClockDivision = 0x0;					//ʱ�ӷ�Ƶ����
  TIM4_TimeBaseStructure.TIM_RepetitionCounter = 0x0;

  TIM_TimeBaseInit(TIM4,&TIM4_TimeBaseStructure);					//дTIM4���Ĵ�������
  
  TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 			    //PWMģʽ2 ���ϼ���ʱ����CCRΪ��Ч��ƽ
  TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //����/����2�������  OC1�ź��������Ӧ���������
  TIM4_OCInitStructure.TIM_Pulse = CCR1_Val; 					    				//ȷ��ռ�ձȣ����ֵ��������Ч��ƽ��ʱ�䡣
  TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	    //�͵�ƽ��Ч 
         
  TIM_OC1Init(TIM4, &TIM4_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);		//����TIM4_CCR1�Ĵ�����Ԥװ�ع���
  TIM_Cmd(TIM4,ENABLE);											//������ʱ��4
}
/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/
