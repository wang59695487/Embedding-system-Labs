/****************************************************************************
* Copyright (C), 2013 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,V2.1，V3，V5及MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: main.c
* 内容简述:	
*       
*	演示的是3个蓝色LED（LED1-LED3) 轮流闪烁
    
	定义：	
	LED1-LED3 ---V6——V8 
	V6----- PB5-LED1
	V7----- PD6-LED2（仅V2，V2.1 V3  V5板）
	V8----- PD3-LED3（仅V2，V2.1 V3  V5板）
    
	基于MDK版本：        3.8
	基于官方外设库版本： 3.5
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-7-28 sun68  创建该文件
*
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

GPIO_InitTypeDef GPIO_InitStructure;//初始化IO口
#define LED1_ON GPIO_SetBits(GPIOB, GPIO_Pin_5);//PB5 = 1
#define LED1_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_5);//PB5 = 0

#define LED2_ON GPIO_SetBits(GPIOD, GPIO_Pin_6);//PD6 = 1
#define LED2_OFF GPIO_ResetBits(GPIOD, GPIO_Pin_6);//PD6 = 0 

#define LED3_ON GPIO_SetBits(GPIOD, GPIO_Pin_3);//PD3 = 1  
#define LED3_OFF GPIO_ResetBits(GPIOD, GPIO_Pin_3);//PD3 = 0   

void RCC_Configuration(void);
void LED_Config(void);
void Delay(__IO uint32_t nCount);

/****************************************************************************
* 名    称：void LED_Config(void)
* 功    能：LED 控制初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LED_Config(void){	
	//开启外设GPIOB和GPIOD时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD , ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //LED1-V6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //将引脚模式配置为通用推挽输出 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //口线翻转速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_3;	//LED2-V7, LED3-V8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//将引脚模式配置为通用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 		//口线翻转速度为50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			      //LCD背光控制
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);
//  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			              //LCD背光关闭	
}

/****************************************************************************
* 名    称：int main(void)
* 功    能：主函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
int flag;

int main(void)
{
  RCC_Configuration();   				//系统时钟配置
  LED_Config();							    //LED控制配置
	/*
  while (1) //LED 0.25s-1Hz
  {
  	LED1_ON; LED2_ON; LED3_ON;    //LED亮
		Delay(250);	                  //延时0.25s
		LED1_OFF; LED2_OFF; LED3_OFF;	//LED灭
		Delay(250);	                  //延时0.25s
  }
	*/
	///*
  flag = 0;
	while(1){
		 flag++;//一个flag代表0.01s
		 Delay(1);
		 //LED3 3HZ 1/(3*2) = 0.167
		 // 每增加167，LED转换状态
		 if(flag%334 == 0){
			 LED3_ON; 
		 }
		 if(flag%334 == 167){
			 LED3_OFF;
		 }
		 //LED2 2HZ 1/(2*2) = 0.25 
		 //每增加250，LED转换状态
		 if(flag%500 == 0){
			 LED2_ON; 
		 }
		 if(flag%500 == 250){
			 LED2_OFF; 
		 }
		 //LED1 1HZ 1/(2*1) = 0.5 
		 //每增加500，LED转换状态
		 if(flag%1000 == 0){
			 LED1_ON; 
		 }
		 if(flag%1000 == 500){
			 LED1_OFF; 
		 }
		 if(flag == 1000000){
			 flag = 0;
		 }
	}
	
	//*/
	
}

/****************************************************************************
* 名    称：void RCC_Configuration(void)
* 功    能：系统时钟配置为72MHZ
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void RCC_Configuration(void)
{   
  SystemInit();//系统时钟初始化
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
	 //while(nCount != 0)  nCount--; //通过递减实现延时
	
	__IO uint32_t temp;
	__IO uint32_t nms = nCount;//设定计时值，单位毫秒
	SysTick->LOAD = 9000*nms;//SysTick频率为72/8=9MHz
	SysTick->VAL=0X00;//清空寄存器
	SysTick->CTRL=0X01;//使能
	do
	{
		temp=SysTick->CTRL;//读取当前计数值
	}while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
  SysTick->CTRL=0x00; //关闭计数器
  SysTick->VAL =0X00; //清空计数器
}

/******************* (C) COPYRIGHT 2013 奋斗STM32 *****END OF FILE****/
