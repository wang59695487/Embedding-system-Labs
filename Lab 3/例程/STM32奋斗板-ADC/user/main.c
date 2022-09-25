/****************************************************************************
* Copyright (C), 2013 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V2,V2.1,V3,V5,MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: main.c
* ���ݼ���:	
*       
*	��ʾͨ������1��ʾADC1��11ͨ���Ĳ������
*	���������PC������Ҫ���д��ڵ������������
    
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
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"	 
#include "misc.h"		

#include  <stdarg.h>

#define ADC1_DR_Address    ((u32)0x4001244C)	 
static unsigned long ticks;
unsigned char Clock1s;

vu16 ADC_ConvertedValue;

void RCC_Configuration(void);  
void ADC_Configuration(void); 
void Usart1_Init(void);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
/****************************************************************************
* ��    �ƣ�int main(void)
* ��    �ܣ�������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
int main(void)
{	
  RCC_Configuration();	         //����ʱ��
  Usart1_Init();		             //����1��ʼ��  
  ADC_Configuration();				   //ADC��ʼ��
  USART_OUT(USART1,"\r\n USART1 print AD_value -------------------------- \r\n"); 
  while(1)
  {
  	 if (ticks++ >= 900000) {        //���ʱ����ʾת�����
     	ticks   = 0;
     	Clock1s = 1;
   	 }
	 if (Clock1s) {
       Clock1s = 0;      
	   USART_OUT(USART1,"The current AD value = %d , as voltage = %d mV\r\n", ADC_ConvertedValue ,(ADC_ConvertedValue * 825)>>10);
     }  
  }
}
/****************************************************************************
* ��    �ƣ�void ADC_Configuration(void)
* ��    �ܣ�ADC ���ú���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

    //����ADģ������˿�ΪPC1   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	 /* Enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
   /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

	//ʹ��DMA
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;			 //DMAͨ��1�ĵ�ַ 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue; //DMA���͵�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					     //���ͷ���:��������
	DMA_InitStructure.DMA_BufferSize = 1;								             //�����ڴ��С��1��16λ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				   //�����ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//ADC1ת����������16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���͵�Ŀ�ĵ�ַ��16λ���
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;									 //ѭ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							 //�����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;										 //û���ڴ浽�ڴ�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);	
	//ʹ��DMAͨ��1
	DMA_Cmd(DMA1_Channel1, ENABLE); 
  
	
	//ADC����
	/* ADCת��ʱ�䣺 ʱ��Ϊ56MHzʱΪ1��s(ʱ��Ϊ72MHzΪ1.17��s)   ADC������Χ0-3.3V */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                   //����ADC��ʱ��Ϊ72MHZ/6=12M 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC1�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		    //ģ��ת��������ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת������������ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת���������������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;             //�涨��˳����й���ת����ADCͨ������Ŀ��
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channels configuration [����ģʽͨ������]*/ 
	//ADC1 ����ͨ������
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);//ͨ��11����ʱ�� 55.5������	
	//ʹ��DMA 
	ADC_DMACmd(ADC1, ENABLE);
	//ʹ��ADC1
	ADC_Cmd(ADC1, ENABLE);	
	// ��ʼ��ADC1У׼�Ĵ���
	ADC_ResetCalibration(ADC1);
	//���ADC1У׼�Ĵ�����ʼ���Ƿ����
	while(ADC_GetResetCalibrationStatus(ADC1));
	//��ʼУ׼ADC1
	ADC_StartCalibration(ADC1);
	//����Ƿ����У׼
	while(ADC_GetCalibrationStatus(ADC1));
	//ADC1ת������
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	 
}
/****************************************************************************
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void RCC_Configuration(void){

  SystemInit(); 		//ϵͳʱ��
  RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//����ʱ��
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//DMAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);	//ADCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);//GPIOʱ��
}

/****************************************************************************
* ��    �ƣ�void Usart1_Init(void)
* ��    �ܣ�����1��ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void Usart1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE);//ʹ�ܴ���1ʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 //USART1 TX  - PA9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 		

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX  - PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //���ÿ�©����
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	

  USART_InitStructure.USART_BaudRate = 115200;								//����115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;				  //��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);							//���ô��ڲ�������   
   /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);	
  
}
/******************************************************
		��������ת�ַ�������
        char *itoa(int value, char *string, int radix)
		radix=10 ��ʾ��10����	��ʮ���ƣ�ת�����Ϊ0;  

	    ����d=-379;
		ִ��	itoa(d, buf, 10); ��
		
		buf="-379"							   			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */
/****************************************************************************
* ��    �ƣ�void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* ��    �ܣ���ʽ�������������
* ��ڲ�����USARTx:  ָ������
			Data��   ��������
			...:     ��������
* ���ڲ�������
* ˵    ������ʽ�������������
        	"\r"	�س���	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	���з�	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	�ַ���	   USART_OUT(USART1, "�ַ����ǣ�%s","abcdefg")
			"%d"	ʮ����	   USART_OUT(USART1, "a=%d",10)
* ���÷������� 
****************************************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 

	const char *s;
    int d;
   
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //�ж��Ƿ񵽴��ַ���������
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //���з�
					USART_SendData(USARTx, 0x0a);	
					Data++;
					break;
				
				default:
					Data++;
				    break;
			}
			
			 
		}
		else if(*Data=='%'){									  //
			switch (*++Data){				
				case 's':										  //�ַ���
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //ʮ����
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}
/******************* (C) COPYRIGHT 2013 �ܶ�STM32 *****END OF FILE****/

