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
#include  <stm32f10x.h>

void time_ini(void);
extern unsigned int CCR2_Val;
TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;//定时器初始化
TIM_OCInitTypeDef  			 TIM3_OCInitStructure;//定时器输出比较
TIM_BDTRInitTypeDef 		 TIM3_BDTRInitStructure;//定时器断路和死区

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
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//通过ABP1对定时器3时钟使能
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;						  //PB5复用为TIM3的通道2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//推挽式复用输出，
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线速度50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* TIM3局部复用功能开启	 在TIM3的局部复用开启时，PB5会被复用为TIM3_CH2*/
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);	
	
  TIM3_TimeBaseStructure.TIM_Prescaler = 7199;									//预分频系数TIM3_PSC=7199	 
  TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数器向上计数模式 TIM3_CR1[4]=0
  TIM3_TimeBaseStructure.TIM_Period = 0xffff;				        		//自动重装载寄存器TIM3_APR      		     
  TIM3_TimeBaseStructure.TIM_ClockDivision = 0x0;								//时钟分频因子 TIM3_CR1[9:8]=00
  TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);							 	//写TIM3各寄存器参数
  
  TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle ;					//TIM3_CCMR1[14:12]=011  翻转 当TIM3_CCR2=TIM3_CNT时，翻转OC2REF的电平 
  TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输入/捕获2输出允许  OC2信号输出到对应的输出引脚PB5
  TIM3_OCInitStructure.TIM_Pulse =CCR2_Val;						    //若CC1通道配置为输出：CCR2是装入当前捕获/比较2 TIM3_CCR2寄存器的值（预装载值）。
																											//当前捕获/比较寄存器包含了与计数器TIM3_CNT比较的值，并且在OC端口上输出信号
  TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			//输出极性  低电平有效 TIM3_CCER[5]=1;

  TIM_OC2Init(TIM3, &TIM3_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);		//TIM3_CCMR1[1]=0  禁止TIM3_CCR2寄存器的预装载功能，可随时写入TIM3_CCR2
                                                        //且新值马上起作用
 
  TIM_Cmd(TIM3,ENABLE);															//启动定时器3 TIM3_CR1[0]=1;  
  TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE); 							//TIM3_DIER[2]=1  允许捕获/比较2中断
}

/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/
