/****************************************************************************
* Copyright (C), 2011 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,V2.1，V3及MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: tim.c
* 内容简述:	
*    TIM3的初始化	      
	
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-7-29 sun68  创建该文件
*
*/				 
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"

void time_ini(void);
extern unsigned int CCR1_Val;
TIM_TimeBaseInitTypeDef  TIM4_TimeBaseStructure;//定时器初始化
TIM_OCInitTypeDef  TIM4_OCInitStructure;//定时器输出比较
TIM_BDTRInitTypeDef TIM4_BDTRInitStructure;//定时器断路和死区


/****************************************************************************
* 名    称：void time_ini(void)
* 功    能：TIM3初始化
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void time_ini(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//通过ABP1对定时器4时钟使能

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					//PD12复用为TIM4的通道1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//推挽式复用输出，
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//口线速度50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //TIM4局部复用功能开启
  GPIO_PinRemapConfig(GPIO_Remap_TIM4 , ENABLE);			 
  				
  TIM4_TimeBaseStructure.TIM_Prescaler = 2;						    //预分频器TIM4_PSC=2	 
  TIM4_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器向上计数模式
  TIM4_TimeBaseStructure.TIM_Period =47999;				       //自动重装载寄存器 确定频率为500Hz 		     
  TIM4_TimeBaseStructure.TIM_ClockDivision = 0x0;					//时钟分频因子
  TIM4_TimeBaseStructure.TIM_RepetitionCounter = 0x0;

  TIM_TimeBaseInit(TIM4,&TIM4_TimeBaseStructure);					//写TIM4各寄存器参数
  
  TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 			    //PWM模式2 向上计数时大于CCR为有效电平
  TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //输入/捕获2输出允许  OC1信号输出到对应的输出引脚
  TIM4_OCInitStructure.TIM_Pulse = CCR1_Val; 					    				//确定占空比，这个值决定了有效电平的时间。
  TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	    //低电平有效 
         
  TIM_OC1Init(TIM4, &TIM4_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);		//启用TIM4_CCR1寄存器的预装载功能
  TIM_Cmd(TIM4,ENABLE);											//启动定时器4
}
/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/
