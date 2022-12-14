/****************************************************************************
* 文件名: main.c
* 内容简述:
*       演示  由串口2采集 JY901 数据，并由 串口1输出。
*
定义：	
	TXD1----- PA9-US1-TX
	RXD1----- PA10-US1-RX
	TXD2----- PA2-US2-TX
	RXD2----- PA3-US2-RX
	速率：9600,n,8,1 
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

struct STime		stcTime;//时间
struct SAcc 		stcAcc;//加速度
struct SGyro 		stcGyro;//角速度
struct SAngle 	stcAngle;//角度
struct SMag 		stcMag;//磁场
struct SDStatus stcDStatus;//结口
struct SPress 	stcPress;//压力
struct SLonLat 	stcLonLat;//经纬度
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
* 名    称：USART_Config(USART_TypeDef* USARTx)
* 功    能：配置串口
* 入口参数：
* 出口参数：无
* 说    明：
* 调用方法：例如: USART_Config(USART1)
****************************************************************************/
void USART_Config(USART_TypeDef* USARTx){
  USART_InitStructure.USART_BaudRate = 9600;						      //速率9600bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

  /* Configure USART1 */
  USART_Init(USARTx, &USART_InitStructure);							//配置串口参数函数
   
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);				//使能接收中断
  USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);				  //使能发送缓冲空中断   
	
  /* Enable the USART1 */
  USART_Cmd(USARTx, ENABLE);	
}

//CopeSerialData为串口2中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerial2Data(unsigned char ucData)
{
	RxBuffer2[RxCounter2++]=ucData;	//将收到的数据存入缓冲区中
	if (RxBuffer2[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		RxCounter2=0;
		return;
	}
	if (RxCounter2<11) {return;}//数据不满11个，则返回
	else
	{
		switch(RxBuffer2[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{			
			case 0x51:	memcpy(&stcAcc,&RxBuffer2[2],8);break;  //memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
			case 0x52:	memcpy(&stcGyro,&RxBuffer2[2],8);break;  //memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
			case 0x53:	memcpy(&stcAngle,&RxBuffer2[2],8);break;  //memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
			case 0x54:	memcpy(&stcMag,&RxBuffer2[2],8);break;  //memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
		}
		RxCounter2=0;//清空缓存区
	}
}

/****************************************************************************
* 名    称：int main(void)
* 功    能：奋斗板主函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
int main(void)
{

  char str1[100],str2[100],str3[100],str4[100];
	
  RCC_Configuration();											  //系统时钟设置
  NVIC_Configuration();											  //嵌套向量中断控制器配?
	GPIO_Configuration();											  //端口初始化
	
  USART_Config(USART1);											  //串口1初始化
	USART_Config(USART2);											  //串口2初始化
  
	GPIO_SetBits(GPIOB, GPIO_Pin_5);						//点亮LED1
  USART_OUT(USART1,"IMU JY901 测试程序\r\n"); //向串口1发送开机字符。
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);					//关闭LED1			
	
	Delay(15000000); //等JY901初始完成
	while (1)
  {
	  USART_OUT(USART1,"\r\n");
		//输出传入数据
		sprintf(str1,"加速度:Ax=%.3f Ay=%.3f Az=%.3f (*9.8m^2/s)温度:T=%.3f (o)\r\n",(float)stcAcc.a[0]/32768.0*16.0,(float)stcAcc.a[1]/32768.0*16.0,(float)stcAcc.a[2]/32768.0*16.0,(float)stcAcc.T/100.0);
		sprintf(str2,"角速度:Wx=%.3f Wy=%.3f Wz=%.3f (o/s)\r\n",(float)stcGyro.w[0]/32768.0*2000.0,(float)stcGyro.w[1]/32768.0*2000.0,(float)stcGyro.w[2]/32768.0*2000.0);
		sprintf(str3,"角度:Roll=%.3f Pitch=%.3f Yaw=%.3f (o)\r\n",(float)stcAngle.Angle[0]/32768.0*180.0,(float)stcAngle.Angle[1]/32768.0*180.0,(float)stcAngle.Angle[2]/32768.0*180.0);
		sprintf(str4,"磁场:Hx=%.3f Hy=%.3f Hz=%.3f (mG)\r\n",(float)stcMag.h[0],(float)stcMag.h[1],(float)stcMag.h[2]);
		USART_OUT(USART1,str1);
		USART_OUT(USART1,str2);
		USART_OUT(USART1,str3);
		USART_OUT(USART1,str4);		
		//LED1闪烁
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);	
		Delay(10000000);
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		Delay(1500000);
	}	
}

/****************************************************************************
* 名    称：void Delay(__IO uint32_t nCount)
* 功    能：延时函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

/****************************************************************************
* 名    称：void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* 功    能：格式化串口输出函数
* 入口参数：USARTx:  指定串口
			Data：   发送数组
			...:     不定参数
* 出口参数：无
* 说    明：格式化串口输出函数
        	"\r"	回车符	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	换行符	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	字符串	   USART_OUT(USART1, "字符串是：%s","abcdefg")
			"%d"	十进制	   USART_OUT(USART1, "a=%d",10)
* 调用方法：无 
****************************************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 
	const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //判断是否到达字符串结束符
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //换行符
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
				case 's':										  //字符串
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //十进制
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
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
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
* 名    称：void RCC_Configuration(void)
* 功    能：系统时钟配置为72MHZ， 外设时钟配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void RCC_Configuration(void)
{
   SystemInit(); //系统时钟
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |  RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
                           | RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO  , ENABLE); //串口1及IO口时钟
	 RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);   // USART2时钟在APB1上
}

/****************************************************************************
* 名    称：void GPIO_Configuration(void)
* 功    能：通用IO口配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
void GPIO_Configuration(void)
{
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				       //LED1控制
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 			 //PB5

  /* 默认复用功能 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 //USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		     //PA9
	
  /* 复用功能的输入引脚必须配置为输入模式（浮空/上拉/下拉的一种）*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //复用浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	   //PA10
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         		 //USART2 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 		 //PA2

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         	 	 //USART2 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //复用开漏输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	   //PA3
}

/****************************************************************************
* 名    称：void NVIC_Configuration(void)
* 功    能：中断源配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void NVIC_Configuration(void)
{
  /*  结构声明*/
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* 优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数  
  0组：  抢占优先级占0位， 响应优先级占4位
  1组：  抢占优先级占1位， 响应优先级占3位
  2组：  抢占优先级占2位， 响应优先级占2位
  3组：  抢占优先级占3位， 响应优先级占1位
  4组：  抢占优先级占4位， 响应优先级占0位  
  */   			     
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	    

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			   	//设置串口1中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//子优先级为1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					  //使能
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			    //设置串口2中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//子优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					  //使能
	NVIC_Init(&NVIC_InitStructure);
}
/******************* (C) COPYRIGHT 2013 奋斗STM32 *****END OF FILE****/
