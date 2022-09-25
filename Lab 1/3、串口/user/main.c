/****************************************************************************
* Copyright (C), 2013 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3,V5,MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: main.c
* 内容简述:
*       演示 USART1 当接收到结束符是0x0d 0x0a时，将接收到的数据传送出去
                       V6（V3，V5板） V2（MINI板） LED1闪烁表示正在收发数据。 
*
定义：	
	TXD1----- PA9-US1-TX
	RXD1----- PA10-US1-RX
	速率：115200,n,8,1 
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-7-21 sun68  创建该文件
*
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_usart.h"	//串口固件库
#include "misc.h"
#include "stdarg.h"						//串口打印

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;   //串口初始化
uint8_t TxBuffer1[100] = {0}, RxBuffer1[100] = {0}; //发送、接收消息
uint8_t rec_f,tx_flag;	//发送、接收标志位 
__IO uint8_t TxCounter1 = 0x00;	//寄存器位置标志
__IO uint8_t RxCounter1 = 0x00; 
//uint32_t Rec_Len;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void); //系统时钟
void GPIO_Configuration(void);//IO端口设置
void NVIC_Configuration(void);//中断设置

void Delay(__IO uint32_t nCount);		//延时函数
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);//格式化串口输出函数
char *itoa(int value, char *string, int radix);//整形数据转字符串函数
void USART_Config(USART_TypeDef* USARTx);//串口配置函数

GPIO_InitTypeDef GPIO_InitStructure;//IO口初始化
USART_InitTypeDef USART_InitStruct;//串口初始化
USART_ClockInitTypeDef USART_ClockInitStruct;//串口时钟初始化

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
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;				  //无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

  /* Configure USART1 */
  USART_Init(USARTx, &USART_InitStructure);					//配置串口参数函数
 
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    //使能接收中断
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);			//使能发送缓冲空中断   

  /* Enable the USART1 */	
  USART_Cmd(USART1, ENABLE);												//使能串口
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

   uint8_t a=0;
  /* System Clocks Configuration */
  RCC_Configuration();											  //系统时钟设置     
  /*嵌套向量中断控制器 
      配置了USART1的优先级分组为0，抢占优先级级别0（无） ，和响应优先级级别0（0-7） */ 
  NVIC_Configuration();											  //中断源配置

  /*对控制LED指示灯的IO口进行了初始化，将端口配置为推挽上拉输出，口线速度为50Mhz。PA9,PA10端口复用为串口1的TX，RX。
  在配置某个口线时，首先应对它所在的端口的时钟进行使能。否则无法配置成功，由于用到了端口B， 因此要对这个端口的时钟
  进行使能，同时由于用到复用IO口功能用于配置串口。因此还要使能AFIO（复用功能IO）时钟。*/
  GPIO_Configuration();											  //端口初始化
  USART_Config(USART1);											  //串口1初始化
  
	USART_OUT(USART1,"Hello World!\r\n");  	//向串口1发送开机字符。
	USART_OUT(USART1,"请输入你的谜语: \r\n");    	  //向串口1发送开机字符。 
	USART_OUT(USART1,"\r\n"); 
	Delay(0xEFFFFF);  
 
   while (1)
  {
	Delay(100); 
	if(rec_f==1){						//判断是否收到一帧有效数据
		rec_f=0;							//取消接收标志位
		USART_OUT(USART1,"\r\n您的谜底为: \r\n");    
		if(TxBuffer1[0]==0xc7){//千: 0xc7,0xa7
			USART_OUT(USART1,"乖");	//回复接收到的信息
	  	USART_OUT(USART1,"\r\n"); 
		}
		else if(TxBuffer1[0]==0xcf){//相: 0xcf,0xe0
			USART_OUT(USART1,"叩");	//回复接收到的信息
	  	USART_OUT(USART1,"\r\n"); 
		}
		else if(TxBuffer1[0]==0xb4){//吹: 0xb4,0xb5
			USART_OUT(USART1,"口");	//回复接收到的信息
	  	USART_OUT(USART1,"\r\n"); 
		}
		else{
			USART_OUT(USART1,"抱歉，无此谜语答案！");	//回复接收到的信息
	  	USART_OUT(USART1,"\r\n"); 
		}
		if(a==0) {GPIO_SetBits(GPIOB, GPIO_Pin_5); a=1;}  //LED1-V6明暗闪烁                
		else {GPIO_ResetBits(GPIOB, GPIO_Pin_5);a=0;  }
	}
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
  //for(; nCount != 0; nCount--);
	while(nCount != 0) nCount--;//递减延时
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

	while(*Data!=0){						//判断是否到达字符串结束符
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
				default:												//跳过
					Data++;
				    break;
			}					 
		}
		else if(*Data=='%'){					  	 //
			switch (*++Data){				
				case 's':										   //字符串
          s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
            USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);//串口标志为0
					}
					Data++;
          break;
        case 'd':										  //十进制
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
				  {
            USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);//串口标志为0
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
    if (radix != 10)	//非十进制，转换结果为0;  
    {
        *ptr = 0;
        return string;
    }

    if (!value)				//输入0
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)    //输入为负数
    {
        *ptr++ = '-';
        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)  //最大位数为5位
    {
        d = value / i;

        if (d || flag)   //由第一位不为零的位到个位
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;	//终止字符串

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
   SystemInit(); //系统时钟初始化
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
                           | RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO  , ENABLE);  //串口时钟、IO端口时钟、复用时钟
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

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //LED1控制--PB5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //口线翻转速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);							 //B端口 	 

  /* 默认复用功能 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		//USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		  	//A端口 
  /* 复用功能的输入引脚必须配置为输入模式（浮空/上拉/下拉的一种）*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //复用浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	   //A端口 

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			      //LCD背光控制
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);
//  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			              //LCD背光关闭	
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
  NVIC_InitTypeDef NVIC_InitStructure;			//中断初始化

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* 优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数   在这里是0， 4 
  0组：  抢占优先级占0位， 响应优先级占4位
  1组：  抢占优先级占1位， 响应优先级占3位
  2组：  抢占优先级占2位， 响应优先级占2位
  3组：  抢占优先级占3位， 响应优先级占1位
  4组：  抢占优先级占4位， 响应优先级占0位  
  */   			     
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	    		//抢占优先级占0位,响应优先级占4位

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			  //设置串口1中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//子优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//中断使能
  NVIC_Init(&NVIC_InitStructure);
}
/******************* (C) COPYRIGHT 2013 奋斗STM32 *****END OF FILE****/
