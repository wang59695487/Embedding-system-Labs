/****************************************************************************
* Copyright (C), 2013 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V2,2.1,V3,V5,MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: main.c
* ���ݼ���:
*       ��ʾ USART1 �����յ���������0x0d 0x0aʱ�������յ������ݴ��ͳ�ȥ
                       V6��V3��V5�壩 V2��MINI�壩 LED1��˸��ʾ�����շ����ݡ� 
*
���壺	
	TXD1----- PA9-US1-TX
	RXD1----- PA10-US1-RX
	���ʣ�115200,n,8,1 
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.2    2011-7-21 sun68  �������ļ�
*
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_usart.h"	//���ڹ̼���
#include "misc.h"
#include "stdarg.h"						//���ڴ�ӡ

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;   //���ڳ�ʼ��
uint8_t TxBuffer1[100] = {0}, RxBuffer1[100] = {0}; //���͡�������Ϣ
uint8_t rec_f,tx_flag;	//���͡����ձ�־λ 
__IO uint8_t TxCounter1 = 0x00;	//�Ĵ���λ�ñ�־
__IO uint8_t RxCounter1 = 0x00; 
//uint32_t Rec_Len;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void); //ϵͳʱ��
void GPIO_Configuration(void);//IO�˿�����
void NVIC_Configuration(void);//�ж�����

void Delay(__IO uint32_t nCount);		//��ʱ����
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);//��ʽ�������������
char *itoa(int value, char *string, int radix);//��������ת�ַ�������
void USART_Config(USART_TypeDef* USARTx);//�������ú���

GPIO_InitTypeDef GPIO_InitStructure;//IO�ڳ�ʼ��
USART_InitTypeDef USART_InitStruct;//���ڳ�ʼ��
USART_ClockInitTypeDef USART_ClockInitStruct;//����ʱ�ӳ�ʼ��

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
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;				  //��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

  /* Configure USART1 */
  USART_Init(USARTx, &USART_InitStructure);					//���ô��ڲ�������
 
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    //ʹ�ܽ����ж�
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);			//ʹ�ܷ��ͻ�����ж�   

  /* Enable the USART1 */	
  USART_Cmd(USART1, ENABLE);												//ʹ�ܴ���
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

   uint8_t a=0;
  /* System Clocks Configuration */
  RCC_Configuration();											  //ϵͳʱ������     
  /*Ƕ�������жϿ����� 
      ������USART1�����ȼ�����Ϊ0����ռ���ȼ�����0���ޣ� ������Ӧ���ȼ�����0��0-7�� */ 
  NVIC_Configuration();											  //�ж�Դ����

  /*�Կ���LEDָʾ�Ƶ�IO�ڽ����˳�ʼ�������˿�����Ϊ������������������ٶ�Ϊ50Mhz��PA9,PA10�˿ڸ���Ϊ����1��TX��RX��
  ������ĳ������ʱ������Ӧ�������ڵĶ˿ڵ�ʱ�ӽ���ʹ�ܡ������޷����óɹ��������õ��˶˿�B�� ���Ҫ������˿ڵ�ʱ��
  ����ʹ�ܣ�ͬʱ�����õ�����IO�ڹ����������ô��ڡ���˻�Ҫʹ��AFIO�����ù���IO��ʱ�ӡ�*/
  GPIO_Configuration();											  //�˿ڳ�ʼ��
  USART_Config(USART1);											  //����1��ʼ��
  
	USART_OUT(USART1,"Hello World!\r\n");  	//�򴮿�1���Ϳ����ַ���
	USART_OUT(USART1,"�������������: \r\n");    	  //�򴮿�1���Ϳ����ַ��� 
	USART_OUT(USART1,"\r\n"); 
	Delay(0xEFFFFF);  
 
   while (1)
  {
	Delay(100); 
	if(rec_f==1){						//�ж��Ƿ��յ�һ֡��Ч����
		rec_f=0;							//ȡ�����ձ�־λ
		USART_OUT(USART1,"\r\n�����յ�Ϊ: \r\n");    
		if(TxBuffer1[0]==0xc7){//ǧ: 0xc7,0xa7
			USART_OUT(USART1,"��");	//�ظ����յ�����Ϣ
	  	USART_OUT(USART1,"\r\n"); 
		}
		else if(TxBuffer1[0]==0xcf){//��: 0xcf,0xe0
			USART_OUT(USART1,"ߵ");	//�ظ����յ�����Ϣ
	  	USART_OUT(USART1,"\r\n"); 
		}
		else if(TxBuffer1[0]==0xb4){//��: 0xb4,0xb5
			USART_OUT(USART1,"��");	//�ظ����յ�����Ϣ
	  	USART_OUT(USART1,"\r\n"); 
		}
		else{
			USART_OUT(USART1,"��Ǹ���޴�����𰸣�");	//�ظ����յ�����Ϣ
	  	USART_OUT(USART1,"\r\n"); 
		}
		if(a==0) {GPIO_SetBits(GPIOB, GPIO_Pin_5); a=1;}  //LED1-V6������˸                
		else {GPIO_ResetBits(GPIOB, GPIO_Pin_5);a=0;  }
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
	while(nCount != 0) nCount--;//�ݼ���ʱ
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

	while(*Data!=0){						//�ж��Ƿ񵽴��ַ���������
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
				default:												//����
					Data++;
				    break;
			}					 
		}
		else if(*Data=='%'){					  	 //
			switch (*++Data){				
				case 's':										   //�ַ���
          s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
            USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);//���ڱ�־Ϊ0
					}
					Data++;
          break;
        case 'd':										  //ʮ����
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
				  {
            USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);//���ڱ�־Ϊ0
          }
					Data++;
          break;
				default:
					Data++;
				    break;
			}		 
		}
		else 
			USART_SendData(USARTx, *Data++);
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
    if (radix != 10)	//��ʮ���ƣ�ת�����Ϊ0;  
    {
        *ptr = 0;
        return string;
    }

    if (!value)				//����0
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)    //����Ϊ����
    {
        *ptr++ = '-';
        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)  //���λ��Ϊ5λ
    {
        d = value / i;

        if (d || flag)   //�ɵ�һλ��Ϊ���λ����λ
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;	//��ֹ�ַ���

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
   SystemInit(); //ϵͳʱ�ӳ�ʼ��
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
                           | RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO  , ENABLE);  //����ʱ�ӡ�IO�˿�ʱ�ӡ�����ʱ��
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

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //LED1����--PB5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);							 //B�˿� 	 

  /* Ĭ�ϸ��ù��� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		//USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		  	//A�˿� 
  /* ���ù��ܵ��������ű�������Ϊ����ģʽ������/����/������һ�֣�*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //���ø�������
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	   //A�˿� 

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			      //LCD�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);
//  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			              //LCD����ر�	
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
  NVIC_InitTypeDef NVIC_InitStructure;			//�жϳ�ʼ��

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* ���ȼ��� ˵������ռ���ȼ����õ�λ��������Ӧ���ȼ����õ�λ��   ��������0�� 4 
  0�飺  ��ռ���ȼ�ռ0λ�� ��Ӧ���ȼ�ռ4λ
  1�飺  ��ռ���ȼ�ռ1λ�� ��Ӧ���ȼ�ռ3λ
  2�飺  ��ռ���ȼ�ռ2λ�� ��Ӧ���ȼ�ռ2λ
  3�飺  ��ռ���ȼ�ռ3λ�� ��Ӧ���ȼ�ռ1λ
  4�飺  ��ռ���ȼ�ռ4λ�� ��Ӧ���ȼ�ռ0λ  
  */   			     
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	    		//��ռ���ȼ�ռ0λ,��Ӧ���ȼ�ռ4λ

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			  //���ô���1�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ� 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//�ж�ʹ��
  NVIC_Init(&NVIC_InitStructure);
}
/******************* (C) COPYRIGHT 2013 �ܶ�STM32 *****END OF FILE****/
