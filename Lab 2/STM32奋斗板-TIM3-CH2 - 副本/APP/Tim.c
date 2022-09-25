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
#include  <stm32f10x.h>

void time_ini(void);
extern unsigned int CCR2_Val;
TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;//��ʱ����ʼ��
TIM_OCInitTypeDef  			 TIM3_OCInitStructure;//��ʱ������Ƚ�
TIM_BDTRInitTypeDef 		 TIM3_BDTRInitStructure;//��ʱ����·������

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
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ͨ��ABP1�Զ�ʱ��3ʱ��ʹ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;						  //PB5����ΪTIM3��ͨ��2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//����ʽ���������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�����ٶ�50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* TIM3�ֲ����ù��ܿ���	 ��TIM3�ľֲ����ÿ���ʱ��PB5�ᱻ����ΪTIM3_CH2*/
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);	
	
  TIM3_TimeBaseStructure.TIM_Prescaler = 7199;									//Ԥ��Ƶϵ��TIM3_PSC=7199	 
  TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���������ϼ���ģʽ TIM3_CR1[4]=0
  TIM3_TimeBaseStructure.TIM_Period = 0xffff;				        		//�Զ���װ�ؼĴ���TIM3_APR      		     
  TIM3_TimeBaseStructure.TIM_ClockDivision = 0x0;								//ʱ�ӷ�Ƶ���� TIM3_CR1[9:8]=00
  TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);							 	//дTIM3���Ĵ�������
  
  TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle ;					//TIM3_CCMR1[14:12]=011  ��ת ��TIM3_CCR2=TIM3_CNTʱ����תOC2REF�ĵ�ƽ 
  TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//����/����2�������  OC2�ź��������Ӧ���������PB5
  TIM3_OCInitStructure.TIM_Pulse =CCR2_Val;						    //��CC1ͨ������Ϊ�����CCR2��װ�뵱ǰ����/�Ƚ�2 TIM3_CCR2�Ĵ�����ֵ��Ԥװ��ֵ����
																											//��ǰ����/�ȽϼĴ����������������TIM3_CNT�Ƚϵ�ֵ��������OC�˿�������ź�
  TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			//�������  �͵�ƽ��Ч TIM3_CCER[5]=1;

  TIM_OC2Init(TIM3, &TIM3_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);		//TIM3_CCMR1[1]=0  ��ֹTIM3_CCR2�Ĵ�����Ԥװ�ع��ܣ�����ʱд��TIM3_CCR2
                                                        //����ֵ����������
 
  TIM_Cmd(TIM3,ENABLE);															//������ʱ��3 TIM3_CR1[0]=1;  
  TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE); 							//TIM3_DIER[2]=1  ������/�Ƚ�2�ж�
}

/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/
