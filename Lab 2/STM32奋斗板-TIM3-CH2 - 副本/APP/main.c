/****************************************************************************
* Copyright (C), 2013 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,V2.1，V3,V5及MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: main.c
* 内容简述:	
*       
*	本例程演示了通过通用定时器3的2通道，实现12.5Hz波形的产生，用于控制LED1的闪烁：
    
	基于MDK版本：        3.8
	基于官方外设库版本： 3.5
  
    硬件连接关系 
    PB5---TIM3-CH2
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-7-29 sun68  创建该文件
*
*/

#include  "stm32f10x.h"
#include  "stm32f10x_exti.h" //外部中断
#include  "stm32f10x_tim.h" //定时器
#include  "stm32f10x_rcc.h"	//系统时钟
#include  "misc.h"

unsigned int _it0;
unsigned int CCR2_Val; 
extern void time_ini(void);

/****************************************************************************
* 名    称：void RCC_Configuration(void)
* 功    能：系统时钟配置为72MHZ， 外设时钟配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void RCC_Configuration(void){

  SystemInit(); 		//系统时钟初始化
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 	//复用时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);	//IO口时钟
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
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				 	 //4位响应优先级

  NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn ;			   	 //配置定时器中断 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //响应优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					 //中断使能
  NVIC_Init(&NVIC_InitStructure);  	
}

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
  RCC_Configuration();   	   //系统时钟设置及外设时钟使能					     
  NVIC_Configuration();			 //中断源配置
	
	CCR2_Val=5000; 
	//CR2_Val=500; 
	time_ini();								 //定时器初始化
  while(1);
}
/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/

