/****************************************************************************
* Copyright (C), 2013 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V3.1,V5上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: main.c
* 内容简述:	
*       
*	演示的是按动K1--K3 相应的LED1--LED3会闪
    
	基于MDK版本：        3.8
	基于官方外设库版本： 3.5
  
   LED1-LED3 ---V6——V8 
	V6----- PB5-LED1 
	V7----- PD6-LED2
	V8----- PD3-LED3
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.3    2012-2-16 sun68  创建该文件
*
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_exti.h" //外部中断
#include "stm32f10x_rcc.h"	//系统时钟
#include "misc.h"

#define LED1_ON GPIO_SetBits(GPIOB, GPIO_Pin_5)//PB5 = 1
#define LED1_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_5)//PB5 = 0
#define LED2_ON GPIO_SetBits(GPIOD, GPIO_Pin_6)//PD6 = 1
#define LED2_OFF GPIO_ResetBits(GPIOD, GPIO_Pin_6)//PD6 = 0 
#define LED3_ON GPIO_SetBits(GPIOD, GPIO_Pin_3)//PD3 = 1  
#define LED3_OFF GPIO_ResetBits(GPIOD, GPIO_Pin_3)//PD3 = 0   

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);		 //系统时钟
void GPIO_Configuration(void);	 //IO口配置
void NVIC_Configuration(void);	 //中断配置
void Delay(__IO uint32_t nCount);//延时程序
unsigned char _it0=0,num=0;			 //键盘状态标志位
/****************************************************************************
* 名    称：int main(void)
* 功    能：主函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
int main(void)
{  
  RCC_Configuration();	   //系统时钟设置及外设时钟使能	        
  NVIC_Configuration();		 //系统中断设置
  /*对控制3个LED指示灯的IO口进行了初始化，将3个端口配置为推挽上拉输出，
  口线速度为50Mhz。将中断线PC5,PC2,PC3配置为输入模式。
  在配置某个口线时，首先应对它所在的端口的时钟进行使能。否则无法配置成功，由于用到了端口B和端口D，C，E， 
  因此要对这4个端口的时钟进行使能，同时由于用到复用IO口功能用于配置外部中断。因此还要使能AFIO（复用功能IO）时钟。*/
  GPIO_Configuration();		 //IO端口设置
  while (1)
  {	  
		while(_it0)
		{
			num = _it0;
			_it0 = 0;
		}
	
	if(num==1)				 //K1按下时处理
	{	
  	LED1_ON; LED2_OFF; LED3_OFF;		//LED1亮  LED2,LED3灭（LED2，LED3 仅V5  V3，V2，V2.1板有）
    Delay(0x4FFFFF);
		LED1_OFF; LED2_ON; LED3_OFF;		//LED2亮  LED1,LED3灭（LED2，LED3 仅V5  V3，V2，V2.1板有）
		Delay(0x4FFFFF);
		LED1_OFF; LED2_OFF; LED3_ON;		//LED3亮  LED1,LED2灭（LED2，LED3 仅V5  V3，V2，V2.1板有）
		Delay(0x4FFFFF);
	}		     
	if(num==2)   			//K2按下时处理
	{
  	LED1_ON; LED2_OFF; LED3_OFF;		//LED1亮  LED2,LED3灭（LED2，LED3 仅V5  V3，V2，V2.1板有）
    Delay(0x4FFFFF);
		LED1_OFF; LED2_ON; LED3_OFF;		//LED2亮  LED1,LED3灭（LED2，LED3 仅V5  V3，V2，V2.1板有）
		Delay(0x4FFFFF);
		LED1_OFF; LED2_OFF; LED3_ON;		//LED3亮  LED1,LED2灭（LED2，LED3 仅V5  V3，V2，V2.1板有）
		Delay(0x4FFFFF);			
		LED1_OFF; LED2_ON; LED3_OFF;		//LED2亮  LED1,LED3灭（LED2，LED3 仅V5  V3，V2，V2.1板有）
		Delay(0x4FFFFF);			
	}			
	if(num==3)			   //K3按下时处理
	{
		LED1_ON; LED2_OFF; LED3_OFF;		//LED1亮  LED2,LED3灭（LED2，LED3 仅V5  V3，V2，V2.1板有）
    Delay(0x4FFFFF);	
		LED1_OFF; LED2_OFF; LED3_ON;		//LED3亮  LED1,LED2灭（LED2，LED3 仅V5  V3，V2，V2.1板有）
		Delay(0x4FFFFF);  
		LED1_OFF; LED2_ON; LED3_OFF;		//LED2亮  LED1,LED3灭（LED2，LED3 仅V5  V3，V2，V2.1板有）
		Delay(0x4FFFFF);	
	}				
  }
}

/****************************************************************************
* 名    称：void Delay(__IO uint32_t nCount)
* 功    能：延时程序
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void Delay(__IO uint32_t nCount)
{
  //for(; nCount != 0; nCount--);
	while(nCount != 0)  nCount--;//递减延时
}

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

  SystemInit(); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //复用时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);	//GPIO时钟
}

/****************************************************************************
* 名    称：void GPIO_Configuration(void)
* 功    能：LED控制口线及键盘设置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				    //IO口初始化

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		 			//LED1-V6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //将引脚模式配置为通用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//口线翻转速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3;//LED2-V7, LED3-V8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //将引脚模式配置为通用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//口线翻转速度为50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);
     
  /* K1 配置按键中断线PC5 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;									//K1按键
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //输入上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* K2 配置按键中断线PC2 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;									//K2按键
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //输入上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* K3 配置按键中断线PC3 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;									//K3按键
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //输入上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* K4 配置按键中断线PE6 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;									//K4按键
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //输入上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);	

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			      //LCD背光控制
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);
//  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			                  //LCD背光关闭	
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
  NVIC_InitTypeDef NVIC_InitStructure;   //中断初始化
  EXTI_InitTypeDef EXTI_InitStructure;   //外部中断初始化
  /* Configure one bit for preemption priority */
  /* 优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数   在这里是1, 3 
   0组：  抢占优先级占0位， 响应优先级占4位
   1组：  抢占优先级占1位， 响应优先级占3位
   2组：  抢占优先级占2位， 响应优先级占2位
   3组：  抢占优先级占3位， 响应优先级占1位
   4组：  抢占优先级占4位， 响应优先级占0位  */
    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//1位抢占优先级，3位响应优先级
  
  /* Enable the EXTI9-5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;			   //外部中断9-5
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		   //子优先级1  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				   //中断使能
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;				 //外部中断2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			 //子优先级2  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				   //中断使能
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				 //外部中断3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //子优先级0  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				   //中断使能
  NVIC_Init(&NVIC_InitStructure);

  //用于配置AFIO外部中断配置寄存器AFIO_EXTICR1，用于选择EXTI5外部中断的输入源是PC5。
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);     //外部中断配置AFIO--ETXI9-5
  //用于配置AFIO外部中断配置寄存器AFIO_EXTICR1，用于选择EXTI2外部中断的输入源是PC2。
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);     //外部中断配置AFIO--ETXI2
  //用于配置AFIO外部中断配置寄存器AFIO_EXTICR1，用于选择EXTI3外部中断的输入源是PC3。
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);     //外部中断配置AFIO--ETXI3

  EXTI_InitStructure.EXTI_Line = EXTI_Line5;						//中断线5（PC5）作为键盘K1的检测状态
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		  //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;						//中断线2（PC2）作为键盘K2的检测状态
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		  //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  EXTI_InitStructure.EXTI_Line = EXTI_Line3;						//中断线3（PC3）作为键盘K3的检测状态
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		  //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}
/******************* (C) COPYRIGHT 2013 奋斗STM32 *****END OF FILE****/
