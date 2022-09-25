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
*	本例程演示了周期控制通用定时器3的2通道，实现1KHz的不同占空比波形，用于控制LED1亮度的明暗渐变：
    
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
#include  "stm32f10x_tim.h"
unsigned int CCR1_Val = 20000; 	
extern void time_ini(void);
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
static __IO uint32_t TimingDelay;


/****************************************************************************
* 名    称：void RCC_Configuration(void)
* 功    能：系统时钟配置为72MHZ， 外设时钟配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void RCC_Configuration(void){

  SystemInit(); //系统时钟初始化
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //复用时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);//IO口时钟
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
	unsigned char a=0;
	TIM_OCInitTypeDef  TIM4_OCInitStructure;
  	RCC_Configuration();  
  	time_ini();		      
    SysTick_Config(72000);  //配置SYSTICK时钟节拍为1ms一次
  	while(1){
		/*调整CCR1_Val的值来改变占空比*/
		if(a==0) CCR1_Val=CCR1_Val+10;										 
		else CCR1_Val=CCR1_Val-10;
		if(CCR1_Val>40000){ CCR1_Val=40000; a=1;} 							  
		else if(CCR1_Val<4000){ CCR1_Val=4000; a=0;} 
			
		Delay(1);		  //延时1ms	   	
		TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					  	//PWM模式2 
		TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //输出禁止        
		TIM4_OCInitStructure.TIM_Pulse = CCR1_Val; 							  			//确定占空比
		TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性  低电平有效 TIM3_CCER[5]=1;
		TIM_OC1Init(TIM4, &TIM4_OCInitStructure); 
  	}
}

/****************************************************************************
* 名    称：void Delay(__IO uint32_t nTime)
* 功    能：定时延时程序 1ms为单位
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;
  while(TimingDelay != 0);
}
/****************************************************************************
* 名    称：void TimingDelay_Decrement(void)
* 功    能：获取节拍程序
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00) 	    TimingDelay--;
}
/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/

