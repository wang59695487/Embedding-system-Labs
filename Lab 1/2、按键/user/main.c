/****************************************************************************
* Copyright (C), 2013 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V3.1,V5�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: main.c
* ���ݼ���:	
*       
*	��ʾ���ǰ���K1--K3 ��Ӧ��LED1--LED3����
    
	����MDK�汾��        3.8
	���ڹٷ������汾�� 3.5
  
   LED1-LED3 ---V6����V8 
	V6----- PB5-LED1 
	V7----- PD6-LED2
	V8----- PD3-LED3
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.3    2012-2-16 sun68  �������ļ�
*
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_exti.h" //�ⲿ�ж�
#include "stm32f10x_rcc.h"	//ϵͳʱ��
#include "misc.h"

#define LED1_ON GPIO_SetBits(GPIOB, GPIO_Pin_5)//PB5 = 1
#define LED1_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_5)//PB5 = 0
#define LED2_ON GPIO_SetBits(GPIOD, GPIO_Pin_6)//PD6 = 1
#define LED2_OFF GPIO_ResetBits(GPIOD, GPIO_Pin_6)//PD6 = 0 
#define LED3_ON GPIO_SetBits(GPIOD, GPIO_Pin_3)//PD3 = 1  
#define LED3_OFF GPIO_ResetBits(GPIOD, GPIO_Pin_3)//PD3 = 0   

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);		 //ϵͳʱ��
void GPIO_Configuration(void);	 //IO������
void NVIC_Configuration(void);	 //�ж�����
void Delay(__IO uint32_t nCount);//��ʱ����
unsigned char _it0=0,num=0;			 //����״̬��־λ
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
  RCC_Configuration();	   //ϵͳʱ�����ü�����ʱ��ʹ��	        
  NVIC_Configuration();		 //ϵͳ�ж�����
  /*�Կ���3��LEDָʾ�Ƶ�IO�ڽ����˳�ʼ������3���˿�����Ϊ�������������
  �����ٶ�Ϊ50Mhz�����ж���PC5,PC2,PC3����Ϊ����ģʽ��
  ������ĳ������ʱ������Ӧ�������ڵĶ˿ڵ�ʱ�ӽ���ʹ�ܡ������޷����óɹ��������õ��˶˿�B�Ͷ˿�D��C��E�� 
  ���Ҫ����4���˿ڵ�ʱ�ӽ���ʹ�ܣ�ͬʱ�����õ�����IO�ڹ������������ⲿ�жϡ���˻�Ҫʹ��AFIO�����ù���IO��ʱ�ӡ�*/
  GPIO_Configuration();		 //IO�˿�����
  while (1)
  {	  
		while(_it0)
		{
			num = _it0;
			_it0 = 0;
		}
	
	if(num==1)				 //K1����ʱ����
	{	
  	LED1_ON; LED2_OFF; LED3_OFF;		//LED1��  LED2,LED3��LED2��LED3 ��V5  V3��V2��V2.1���У�
    Delay(0x4FFFFF);
		LED1_OFF; LED2_ON; LED3_OFF;		//LED2��  LED1,LED3��LED2��LED3 ��V5  V3��V2��V2.1���У�
		Delay(0x4FFFFF);
		LED1_OFF; LED2_OFF; LED3_ON;		//LED3��  LED1,LED2��LED2��LED3 ��V5  V3��V2��V2.1���У�
		Delay(0x4FFFFF);
	}		     
	if(num==2)   			//K2����ʱ����
	{
  	LED1_ON; LED2_OFF; LED3_OFF;		//LED1��  LED2,LED3��LED2��LED3 ��V5  V3��V2��V2.1���У�
    Delay(0x4FFFFF);
		LED1_OFF; LED2_ON; LED3_OFF;		//LED2��  LED1,LED3��LED2��LED3 ��V5  V3��V2��V2.1���У�
		Delay(0x4FFFFF);
		LED1_OFF; LED2_OFF; LED3_ON;		//LED3��  LED1,LED2��LED2��LED3 ��V5  V3��V2��V2.1���У�
		Delay(0x4FFFFF);			
		LED1_OFF; LED2_ON; LED3_OFF;		//LED2��  LED1,LED3��LED2��LED3 ��V5  V3��V2��V2.1���У�
		Delay(0x4FFFFF);			
	}			
	if(num==3)			   //K3����ʱ����
	{
		LED1_ON; LED2_OFF; LED3_OFF;		//LED1��  LED2,LED3��LED2��LED3 ��V5  V3��V2��V2.1���У�
    Delay(0x4FFFFF);	
		LED1_OFF; LED2_OFF; LED3_ON;		//LED3��  LED1,LED2��LED2��LED3 ��V5  V3��V2��V2.1���У�
		Delay(0x4FFFFF);  
		LED1_OFF; LED2_ON; LED3_OFF;		//LED2��  LED1,LED3��LED2��LED3 ��V5  V3��V2��V2.1���У�
		Delay(0x4FFFFF);	
	}				
  }
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
	while(nCount != 0)  nCount--;//�ݼ���ʱ
}

/****************************************************************************
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void RCC_Configuration(void)
{

  SystemInit(); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);	//GPIOʱ��
}

/****************************************************************************
* ��    �ƣ�void GPIO_Configuration(void)
* ��    �ܣ�LED���ƿ��߼���������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				    //IO�ڳ�ʼ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		 			//LED1-V6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //������ģʽ����Ϊͨ���������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3;//LED2-V7, LED3-V8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //������ģʽ����Ϊͨ���������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);
     
  /* K1 ���ð����ж���PC5 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;									//K1����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* K2 ���ð����ж���PC2 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;									//K2����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* K3 ���ð����ж���PC3 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;									//K3����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* K4 ���ð����ж���PE6 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;									//K4����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //��������
  GPIO_Init(GPIOE, &GPIO_InitStructure);	

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			      //LCD�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);
//  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			                  //LCD����ر�	
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
  NVIC_InitTypeDef NVIC_InitStructure;   //�жϳ�ʼ��
  EXTI_InitTypeDef EXTI_InitStructure;   //�ⲿ�жϳ�ʼ��
  /* Configure one bit for preemption priority */
  /* ���ȼ��� ˵������ռ���ȼ����õ�λ��������Ӧ���ȼ����õ�λ��   ��������1, 3 
   0�飺  ��ռ���ȼ�ռ0λ�� ��Ӧ���ȼ�ռ4λ
   1�飺  ��ռ���ȼ�ռ1λ�� ��Ӧ���ȼ�ռ3λ
   2�飺  ��ռ���ȼ�ռ2λ�� ��Ӧ���ȼ�ռ2λ
   3�飺  ��ռ���ȼ�ռ3λ�� ��Ӧ���ȼ�ռ1λ
   4�飺  ��ռ���ȼ�ռ4λ�� ��Ӧ���ȼ�ռ0λ  */
    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//1λ��ռ���ȼ���3λ��Ӧ���ȼ�
  
  /* Enable the EXTI9-5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;			   //�ⲿ�ж�9-5
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ� 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		   //�����ȼ�1  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				   //�ж�ʹ��
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;				 //�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ� 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			 //�����ȼ�2  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				   //�ж�ʹ��
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				 //�ⲿ�ж�3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ� 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //�����ȼ�0  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				   //�ж�ʹ��
  NVIC_Init(&NVIC_InitStructure);

  //��������AFIO�ⲿ�ж����üĴ���AFIO_EXTICR1������ѡ��EXTI5�ⲿ�жϵ�����Դ��PC5��
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);     //�ⲿ�ж�����AFIO--ETXI9-5
  //��������AFIO�ⲿ�ж����üĴ���AFIO_EXTICR1������ѡ��EXTI2�ⲿ�жϵ�����Դ��PC2��
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);     //�ⲿ�ж�����AFIO--ETXI2
  //��������AFIO�ⲿ�ж����üĴ���AFIO_EXTICR1������ѡ��EXTI3�ⲿ�жϵ�����Դ��PC3��
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);     //�ⲿ�ж�����AFIO--ETXI3

  EXTI_InitStructure.EXTI_Line = EXTI_Line5;						//�ж���5��PC5����Ϊ����K1�ļ��״̬
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		  //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;						//�ж���2��PC2����Ϊ����K2�ļ��״̬
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		  //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  EXTI_InitStructure.EXTI_Line = EXTI_Line3;						//�ж���3��PC3����Ϊ����K3�ļ��״̬
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		  //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}
/******************* (C) COPYRIGHT 2013 �ܶ�STM32 *****END OF FILE****/
