/****************************************************************************
* �ļ���: main.c
* ���ݼ���:
*       ��ʾ  �ɴ���2�ɼ� JY901 ���ݣ����� ����1�����
*
���壺	
	TXD1----- PA9-US1-TX
	RXD1----- PA10-US1-RX
	TXD2----- PA2-US2-TX
	RXD2----- PA3-US2-RX
	���ʣ�9600,n,8,1 
* 
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stdarg.h"
#include "JY901.h"
#include <string.h>
#include <stdio.h>

struct STime		stcTime;//ʱ��
struct SAcc 		stcAcc;//���ٶ�
struct SGyro 		stcGyro;//���ٶ�
struct SAngle 	stcAngle;//�Ƕ�
struct SMag 		stcMag;//�ų�
struct SDStatus stcDStatus;//���
struct SPress 	stcPress;//ѹ��
struct SLonLat 	stcLonLat;//��γ��
struct SGPSV 		stcGPSV;//
struct SQ       stcQ;

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
uint8_t RxBuffer2[250] = "Test";
__IO uint8_t RxCounter2 = 0x00; 


/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

void Delay(__IO uint32_t nCount);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
char *itoa(int value, char *string, int radix);
void USART_Config(USART_TypeDef* USARTx);

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStruct;
USART_ClockInitTypeDef USART_ClockInitStruct;

/****************************************************************************
* ��    �ƣ�USART_Config(USART_TypeDef* USARTx)
* ��    �ܣ����ô���
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ���÷���������: USART_Config(USART1)
****************************************************************************/
void USART_Config(USART_TypeDef* USARTx){
  USART_InitStructure.USART_BaudRate = 9600;						      //����9600bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

  /* Configure USART1 */
  USART_Init(USARTx, &USART_InitStructure);							//���ô��ڲ�������
   
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);				//ʹ�ܽ����ж�
  USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);				  //ʹ�ܷ��ͻ�����ж�   
	
  /* Enable the USART1 */
  USART_Cmd(USARTx, ENABLE);	
}

//CopeSerialDataΪ����2�жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
void CopeSerial2Data(unsigned char ucData)
{
	RxBuffer2[RxCounter2++]=ucData;	//���յ������ݴ��뻺������
	if (RxBuffer2[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		RxCounter2=0;
		return;
	}
	if (RxCounter2<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(RxBuffer2[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{			
			case 0x51:	memcpy(&stcAcc,&RxBuffer2[2],8);break;  //memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x52:	memcpy(&stcGyro,&RxBuffer2[2],8);break;  //memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x53:	memcpy(&stcAngle,&RxBuffer2[2],8);break;  //memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x54:	memcpy(&stcMag,&RxBuffer2[2],8);break;  //memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
		}
		RxCounter2=0;//��ջ�����
	}
}

/****************************************************************************
* ��    �ƣ�int main(void)
* ��    �ܣ��ܶ���������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
int main(void)
{

  char str1[100],str2[100],str3[100],str4[100];
	
  RCC_Configuration();											  //ϵͳʱ������
  NVIC_Configuration();											  //Ƕ�������жϿ��������
	GPIO_Configuration();											  //�˿ڳ�ʼ��
	
  USART_Config(USART1);											  //����1��ʼ��
	USART_Config(USART2);											  //����2��ʼ��
  
	GPIO_SetBits(GPIOB, GPIO_Pin_5);						//����LED1
  USART_OUT(USART1,"IMU JY901 ���Գ���\r\n"); //�򴮿�1���Ϳ����ַ���
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);					//�ر�LED1			
	
	Delay(15000000); //��JY901��ʼ���
	while (1)
  {
	  USART_OUT(USART1,"\r\n");
		//�����������
		sprintf(str1,"���ٶ�:Ax=%.3f Ay=%.3f Az=%.3f (*9.8m^2/s)�¶�:T=%.3f (o)\r\n",(float)stcAcc.a[0]/32768.0*16.0,(float)stcAcc.a[1]/32768.0*16.0,(float)stcAcc.a[2]/32768.0*16.0,(float)stcAcc.T/100.0);
		sprintf(str2,"���ٶ�:Wx=%.3f Wy=%.3f Wz=%.3f (o/s)\r\n",(float)stcGyro.w[0]/32768.0*2000.0,(float)stcGyro.w[1]/32768.0*2000.0,(float)stcGyro.w[2]/32768.0*2000.0);
		sprintf(str3,"�Ƕ�:Roll=%.3f Pitch=%.3f Yaw=%.3f (o)\r\n",(float)stcAngle.Angle[0]/32768.0*180.0,(float)stcAngle.Angle[1]/32768.0*180.0,(float)stcAngle.Angle[2]/32768.0*180.0);
		sprintf(str4,"�ų�:Hx=%.3f Hy=%.3f Hz=%.3f (mG)\r\n",(float)stcMag.h[0],(float)stcMag.h[1],(float)stcMag.h[2]);
		USART_OUT(USART1,str1);
		USART_OUT(USART1,str2);
		USART_OUT(USART1,str3);
		USART_OUT(USART1,str4);		
		//LED1��˸
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);	
		Delay(10000000);
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		Delay(1500000);
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
  for(; nCount != 0; nCount--);
}

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
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void RCC_Configuration(void)
{
   SystemInit(); //ϵͳʱ��
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |  RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
                           | RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO  , ENABLE); //����1��IO��ʱ��
	 RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);   // USART2ʱ����APB1��
}

/****************************************************************************
* ��    �ƣ�void GPIO_Configuration(void)
* ��    �ܣ�ͨ��IO������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
void GPIO_Configuration(void)
{
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				       //LED1����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 			 //PB5

  /* Ĭ�ϸ��ù��� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 //USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		     //PA9
	
  /* ���ù��ܵ��������ű�������Ϊ����ģʽ������/����/������һ�֣�*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //���ø�������
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	   //PA10
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         		 //USART2 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 		 //PA2

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         	 	 //USART2 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //���ÿ�©����
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	   //PA3
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
  /*  �ṹ����*/
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* ���ȼ��� ˵������ռ���ȼ����õ�λ��������Ӧ���ȼ����õ�λ��  
  0�飺  ��ռ���ȼ�ռ0λ�� ��Ӧ���ȼ�ռ4λ
  1�飺  ��ռ���ȼ�ռ1λ�� ��Ӧ���ȼ�ռ3λ
  2�飺  ��ռ���ȼ�ռ2λ�� ��Ӧ���ȼ�ռ2λ
  3�飺  ��ռ���ȼ�ռ3λ�� ��Ӧ���ȼ�ռ1λ
  4�飺  ��ռ���ȼ�ռ4λ�� ��Ӧ���ȼ�ռ0λ  
  */   			     
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	    

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			   	//���ô���1�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//�����ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					  //ʹ��
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			    //���ô���2�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�����ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					  //ʹ��
	NVIC_Init(&NVIC_InitStructure);
}
/******************* (C) COPYRIGHT 2013 �ܶ�STM32 *****END OF FILE****/
