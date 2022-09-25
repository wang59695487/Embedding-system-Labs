#include "includes.h"
#ifdef GLOBALS 
#define EXT
#else
#define EXT extern 
#endif

#define LED_LED1_ON()   GPIO_SetBits(GPIOB, GPIO_Pin_5 );  	       //LED1  亮
#define LED_LED1_OFF()  GPIO_ResetBits(GPIOB, GPIO_Pin_5 ); 	   //LED1  灭

#define LED_LED2_ON()   GPIO_SetBits(GPIOD, GPIO_Pin_6 );  	       //LED2  亮
#define LED_LED2_OFF()  GPIO_ResetBits(GPIOD, GPIO_Pin_6 ); 	   //LED2  灭

#define LED_LED3_ON()   GPIO_SetBits(GPIOD, GPIO_Pin_3 );  	       //LED3  亮
#define LED_LED3_OFF()  GPIO_ResetBits(GPIOD, GPIO_Pin_3 ); 	   //LED3  灭 

 

EXT OS_EVENT* Com1_SEM;
EXT OS_EVENT* Com1_MBOX;  


EXT unsigned char TxBuffer1[400]; 
 
EXT unsigned char TxBuffer2[]; 
EXT unsigned char RxBuffer1[400];
EXT unsigned char RxBuffer2[];
EXT unsigned char TxCounter1;
EXT unsigned int TxCounter2,star_info[12][4],star_info_t[12][4],GPS_ATLI;
EXT volatile unsigned int RxCounter1; 
EXT volatile unsigned int RxCounter2;
EXT volatile unsigned  short int  ADC_ConvertedValue[100],ADC_ConvertedValue1[2],ADC_TIMEOUT;
EXT volatile unsigned char rec_f,tx_flag,ADC_R;
EXT volatile unsigned long Rec_Len;
EXT float jingdu,weidu;
EXT volatile unsigned  int  milsec1,milsec2,milsec3;
EXT OS_EVENT* ADC_SEM;

EXT unsigned char ADC_STR1[5];	    //ADC1 通道10的整数转字符串变量。 
EXT unsigned char ADC_STR2[5];	    //ADC1 通道10的整数转字符串变量。 