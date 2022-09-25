/****************************************************************************
* ÎÄ¼şÃû: main.c
* ÄÚÈİ¼òÊö:
*       ÑİÊ¾  ÓÉ´®¿Ú2²É¼¯ JY901 Êı¾İ£¬²¢ÓÉ ´®¿Ú1Êä³ö¡£
*
¶¨Òå£º	
	TXD1----- PA9-US1-TX
	RXD1----- PA10-US1-RX
	TXD2----- PA2-US2-TX
	RXD2----- PA3-US2-RX
	ËÙÂÊ£º9600,n,8,1 
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

struct STime		stcTime;//Ê±¼ä
struct SAcc 		stcAcc;//¼ÓËÙ¶È
struct SGyro 		stcGyro;//½ÇËÙ¶È
struct SAngle 	stcAngle;//½Ç¶È
struct SMag 		stcMag;//´Å³¡
struct SDStatus stcDStatus;//½á¿Ú
struct SPress 	stcPress;//Ñ¹Á¦
struct SLonLat 	stcLonLat;//¾­Î³¶È
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
* Ãû    ³Æ£ºUSART_Config(USART_TypeDef* USARTx)
* ¹¦    ÄÜ£ºÅäÖÃ´®¿Ú
* Èë¿Ú²ÎÊı£º
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÀıÈç: USART_Config(USART1)
****************************************************************************/
void USART_Config(USART_TypeDef* USARTx){
  USART_InitStructure.USART_BaudRate = 9600;						      //ËÙÂÊ9600bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //Êı¾İÎ»8Î»
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//Í£Ö¹Î»1Î»
  USART_InitStructure.USART_Parity = USART_Parity_No;				//ÎŞĞ£ÑéÎ»
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //ÎŞÓ²¼şÁ÷¿Ø
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//ÊÕ·¢Ä£Ê½

  /* Configure USART1 */
  USART_Init(USARTx, &USART_InitStructure);							//ÅäÖÃ´®¿Ú²ÎÊıº¯Êı
   
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);				//Ê¹ÄÜ½ÓÊÕÖĞ¶Ï
  USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);				  //Ê¹ÄÜ·¢ËÍ»º³å¿ÕÖĞ¶Ï   
	
  /* Enable the USART1 */
  USART_Cmd(USARTx, ENABLE);	
}

//CopeSerialDataÎª´®¿Ú2ÖĞ¶Ïµ÷ÓÃº¯Êı£¬´®¿ÚÃ¿ÊÕµ½Ò»¸öÊı¾İ£¬µ÷ÓÃÒ»´ÎÕâ¸öº¯Êı¡£
void CopeSerial2Data(unsigned char ucData)
{
	RxBuffer2[RxCounter2++]=ucData;	//½«ÊÕµ½µÄÊı¾İ´æÈë»º³åÇøÖĞ
	if (RxBuffer2[0]!=0x55) //Êı¾İÍ·²»¶Ô£¬ÔòÖØĞÂ¿ªÊ¼Ñ°ÕÒ0x55Êı¾İÍ·
	{
		RxCounter2=0;
		return;
	}
	if (RxCounter2<11) {return;}//Êı¾İ²»Âú11¸ö£¬Ôò·µ»Ø
	else
	{
		switch(RxBuffer2[1])//ÅĞ¶ÏÊı¾İÊÇÄÄÖÖÊı¾İ£¬È»ºó½«Æä¿½±´µ½¶ÔÓ¦µÄ½á¹¹ÌåÖĞ£¬ÓĞĞ©Êı¾İ°üĞèÒªÍ¨¹ıÉÏÎ»»ú´ò¿ª¶ÔÓ¦µÄÊä³öºó£¬²ÅÄÜ½ÓÊÕµ½Õâ¸öÊı¾İ°üµÄÊı¾İ
		{			
			case 0x51:	memcpy(&stcAcc,&RxBuffer2[2],8);break;  //memcpyÎª±àÒëÆ÷×Ô´øµÄÄÚ´æ¿½±´º¯Êı£¬ĞèÒıÓÃ"string.h"£¬½«½ÓÊÕ»º³åÇøµÄ×Ö·û¿½±´µ½Êı¾İ½á¹¹ÌåÀïÃæ£¬´Ó¶øÊµÏÖÊı¾İµÄ½âÎö¡£
			case 0x52:	memcpy(&stcGyro,&RxBuffer2[2],8);break;  //memcpyÎª±àÒëÆ÷×Ô´øµÄÄÚ´æ¿½±´º¯Êı£¬ĞèÒıÓÃ"string.h"£¬½«½ÓÊÕ»º³åÇøµÄ×Ö·û¿½±´µ½Êı¾İ½á¹¹ÌåÀïÃæ£¬´Ó¶øÊµÏÖÊı¾İµÄ½âÎö¡£
			case 0x53:	memcpy(&stcAngle,&RxBuffer2[2],8);break;  //memcpyÎª±àÒëÆ÷×Ô´øµÄÄÚ´æ¿½±´º¯Êı£¬ĞèÒıÓÃ"string.h"£¬½«½ÓÊÕ»º³åÇøµÄ×Ö·û¿½±´µ½Êı¾İ½á¹¹ÌåÀïÃæ£¬´Ó¶øÊµÏÖÊı¾İµÄ½âÎö¡£
			case 0x54:	memcpy(&stcMag,&RxBuffer2[2],8);break;  //memcpyÎª±àÒëÆ÷×Ô´øµÄÄÚ´æ¿½±´º¯Êı£¬ĞèÒıÓÃ"string.h"£¬½«½ÓÊÕ»º³åÇøµÄ×Ö·û¿½±´µ½Êı¾İ½á¹¹ÌåÀïÃæ£¬´Ó¶øÊµÏÖÊı¾İµÄ½âÎö¡£
		}
		RxCounter2=0;//Çå¿Õ»º´æÇø
	}
}

/****************************************************************************
* Ãû    ³Æ£ºint main(void)
* ¹¦    ÄÜ£º·Ü¶·°åÖ÷º¯Êı
* Èë¿Ú²ÎÊı£ºÎŞ
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎŞ 
****************************************************************************/
int main(void)
{

  char str1[100],str2[100],str3[100],str4[100];
	
  RCC_Configuration();											  //ÏµÍ³Ê±ÖÓÉèÖÃ
  NVIC_Configuration();											  //Ç¶Ì×ÏòÁ¿ÖĞ¶Ï¿ØÖÆÆ÷ÅäÖ
	GPIO_Configuration();											  //¶Ë¿Ú³õÊ¼»¯
	
  USART_Config(USART1);											  //´®¿Ú1³õÊ¼»¯
	USART_Config(USART2);											  //´®¿Ú2³õÊ¼»¯
  
	GPIO_SetBits(GPIOB, GPIO_Pin_5);						//µãÁÁLED1
  USART_OUT(USART1,"IMU JY901 ²âÊÔ³ÌĞò\r\n"); //Ïò´®¿Ú1·¢ËÍ¿ª»ú×Ö·û¡£
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);					//¹Ø±ÕLED1			
	
	Delay(15000000); //µÈJY901³õÊ¼Íê³É
	while (1)
  {
	  USART_OUT(USART1,"\r\n");
		//Êä³ö´«ÈëÊı¾İ
		sprintf(str1,"¼ÓËÙ¶È:Ax=%.3f Ay=%.3f Az=%.3f (*9.8m^2/s)ÎÂ¶È:T=%.3f (o)\r\n",(float)stcAcc.a[0]/32768.0*16.0,(float)stcAcc.a[1]/32768.0*16.0,(float)stcAcc.a[2]/32768.0*16.0,(float)stcAcc.T/100.0);
		sprintf(str2,"½ÇËÙ¶È:Wx=%.3f Wy=%.3f Wz=%.3f (o/s)\r\n",(float)stcGyro.w[0]/32768.0*2000.0,(float)stcGyro.w[1]/32768.0*2000.0,(float)stcGyro.w[2]/32768.0*2000.0);
		sprintf(str3,"½Ç¶È:Roll=%.3f Pitch=%.3f Yaw=%.3f (o)\r\n",(float)stcAngle.Angle[0]/32768.0*180.0,(float)stcAngle.Angle[1]/32768.0*180.0,(float)stcAngle.Angle[2]/32768.0*180.0);
		sprintf(str4,"´Å³¡:Hx=%.3f Hy=%.3f Hz=%.3f (mG)\r\n",(float)stcMag.h[0],(float)stcMag.h[1],(float)stcMag.h[2]);
		USART_OUT(USART1,str1);
		USART_OUT(USART1,str2);
		USART_OUT(USART1,str3);
		USART_OUT(USART1,str4);		
		//LED1ÉÁË¸
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);	
		Delay(10000000);
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		Delay(1500000);
	}	
}

/****************************************************************************
* Ãû    ³Æ£ºvoid Delay(__IO uint32_t nCount)
* ¹¦    ÄÜ£ºÑÓÊ±º¯Êı
* Èë¿Ú²ÎÊı£ºÎŞ
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎŞ 
****************************************************************************/
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

/****************************************************************************
* Ãû    ³Æ£ºvoid USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* ¹¦    ÄÜ£º¸ñÊ½»¯´®¿ÚÊä³öº¯Êı
* Èë¿Ú²ÎÊı£ºUSARTx:  Ö¸¶¨´®¿Ú
			Data£º   ·¢ËÍÊı×é
			...:     ²»¶¨²ÎÊı
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£º¸ñÊ½»¯´®¿ÚÊä³öº¯Êı
        	"\r"	»Ø³µ·û	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	»»ĞĞ·û	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	×Ö·û´®	   USART_OUT(USART1, "×Ö·û´®ÊÇ£º%s","abcdefg")
			"%d"	Ê®½øÖÆ	   USART_OUT(USART1, "a=%d",10)
* µ÷ÓÃ·½·¨£ºÎŞ 
****************************************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 
	const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //ÅĞ¶ÏÊÇ·ñµ½´ï×Ö·û´®½áÊø·û
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //»Ø³µ·û
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //»»ĞĞ·û
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
				case 's':										  //×Ö·û´®
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //Ê®½øÖÆ
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
		ÕûĞÎÊı¾İ×ª×Ö·û´®º¯Êı
        char *itoa(int value, char *string, int radix)
		radix=10 ±êÊ¾ÊÇ10½øÖÆ	·ÇÊ®½øÖÆ£¬×ª»»½á¹ûÎª0;  

	    Àı£ºd=-379;
		Ö´ĞĞ	itoa(d, buf, 10); ºó
		
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
* Ãû    ³Æ£ºvoid RCC_Configuration(void)
* ¹¦    ÄÜ£ºÏµÍ³Ê±ÖÓÅäÖÃÎª72MHZ£¬ ÍâÉèÊ±ÖÓÅäÖÃ
* Èë¿Ú²ÎÊı£ºÎŞ
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎŞ 
****************************************************************************/ 
void RCC_Configuration(void)
{
   SystemInit(); //ÏµÍ³Ê±ÖÓ
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |  RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
                           | RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO  , ENABLE); //´®¿Ú1¼°IO¿ÚÊ±ÖÓ
	 RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);   // USART2Ê±ÖÓÔÚAPB1ÉÏ
}

/****************************************************************************
* Ãû    ³Æ£ºvoid GPIO_Configuration(void)
* ¹¦    ÄÜ£ºÍ¨ÓÃIO¿ÚÅäÖÃ
* Èë¿Ú²ÎÊı£ºÎŞ
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£º
****************************************************************************/  
void GPIO_Configuration(void)
{
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				       //LED1¿ØÖÆ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 			 //PB5

  /* Ä¬ÈÏ¸´ÓÃ¹¦ÄÜ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 //USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		     //PA9
	
  /* ¸´ÓÃ¹¦ÄÜµÄÊäÈëÒı½Å±ØĞëÅäÖÃÎªÊäÈëÄ£Ê½£¨¸¡¿Õ/ÉÏÀ­/ÏÂÀ­µÄÒ»ÖÖ£©*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //¸´ÓÃ¸¡¿ÕÊäÈë
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	   //PA10
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         		 //USART2 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 		 //PA2

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         	 	 //USART2 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //¸´ÓÃ¿ªÂ©ÊäÈë
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	   //PA3
}

/****************************************************************************
* Ãû    ³Æ£ºvoid NVIC_Configuration(void)
* ¹¦    ÄÜ£ºÖĞ¶ÏÔ´ÅäÖÃ
* Èë¿Ú²ÎÊı£ºÎŞ
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎŞ 
****************************************************************************/
void NVIC_Configuration(void)
{
  /*  ½á¹¹ÉùÃ÷*/
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* ÓÅÏÈ¼¶×é ËµÃ÷ÁËÇÀÕ¼ÓÅÏÈ¼¶ËùÓÃµÄÎ»Êı£¬ºÍÏìÓ¦ÓÅÏÈ¼¶ËùÓÃµÄÎ»Êı  
  0×é£º  ÇÀÕ¼ÓÅÏÈ¼¶Õ¼0Î»£¬ ÏìÓ¦ÓÅÏÈ¼¶Õ¼4Î»
  1×é£º  ÇÀÕ¼ÓÅÏÈ¼¶Õ¼1Î»£¬ ÏìÓ¦ÓÅÏÈ¼¶Õ¼3Î»
  2×é£º  ÇÀÕ¼ÓÅÏÈ¼¶Õ¼2Î»£¬ ÏìÓ¦ÓÅÏÈ¼¶Õ¼2Î»
  3×é£º  ÇÀÕ¼ÓÅÏÈ¼¶Õ¼3Î»£¬ ÏìÓ¦ÓÅÏÈ¼¶Õ¼1Î»
  4×é£º  ÇÀÕ¼ÓÅÏÈ¼¶Õ¼4Î»£¬ ÏìÓ¦ÓÅÏÈ¼¶Õ¼0Î»  
  */   			     
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	    

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			   	//ÉèÖÃ´®¿Ú1ÖĞ¶Ï
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//ÇÀÕ¼ÓÅÏÈ¼¶0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//×ÓÓÅÏÈ¼¶Îª1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					  //Ê¹ÄÜ
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			    //ÉèÖÃ´®¿Ú2ÖĞ¶Ï
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//ÇÀÕ¼ÓÅÏÈ¼¶0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//×ÓÓÅÏÈ¼¶Îª0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					  //Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);
}
/******************* (C) COPYRIGHT 2013 ·Ü¶·STM32 *****END OF FILE****/
