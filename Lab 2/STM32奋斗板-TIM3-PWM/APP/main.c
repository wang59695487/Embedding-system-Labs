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
*	��������ʾ�����ڿ���ͨ�ö�ʱ��3��2ͨ����ʵ��1KHz�Ĳ�ͬռ�ձȲ��Σ����ڿ���LED1���ȵ��������䣺
    
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
#include  "stm32f10x_tim.h"
unsigned int CCR1_Val = 20000; 	
extern void time_ini(void);
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
static __IO uint32_t TimingDelay;


/****************************************************************************
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void RCC_Configuration(void){

  SystemInit(); //ϵͳʱ�ӳ�ʼ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);//IO��ʱ��
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
	unsigned char a=0;
	TIM_OCInitTypeDef  TIM4_OCInitStructure;
  	RCC_Configuration();  
  	time_ini();		      
    SysTick_Config(72000);  //����SYSTICKʱ�ӽ���Ϊ1msһ��
  	while(1){
		/*����CCR1_Val��ֵ���ı�ռ�ձ�*/
		if(a==0) CCR1_Val=CCR1_Val+10;										 
		else CCR1_Val=CCR1_Val-10;
		if(CCR1_Val>40000){ CCR1_Val=40000; a=1;} 							  
		else if(CCR1_Val<4000){ CCR1_Val=4000; a=0;} 
			
		Delay(1);		  //��ʱ1ms	   	
		TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					  	//PWMģʽ2 
		TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //�����ֹ        
		TIM4_OCInitStructure.TIM_Pulse = CCR1_Val; 							  			//ȷ��ռ�ձ�
		TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������  �͵�ƽ��Ч TIM3_CCER[5]=1;
		TIM_OC1Init(TIM4, &TIM4_OCInitStructure); 
  	}
}

/****************************************************************************
* ��    �ƣ�void Delay(__IO uint32_t nTime)
* ��    �ܣ���ʱ��ʱ���� 1msΪ��λ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/  
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;
  while(TimingDelay != 0);
}
/****************************************************************************
* ��    �ƣ�void TimingDelay_Decrement(void)
* ��    �ܣ���ȡ���ĳ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/  
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00) 	    TimingDelay--;
}
/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/

