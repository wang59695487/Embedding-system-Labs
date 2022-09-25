/****************************************************************************
* Copyright (C), 2011 奋斗嵌入式工作室 ourstm.5d6d.com
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3,MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
* 奋斗板论坛：ourstm.5d6d.com  
*
* 文件名: main.c
* 内容简述:
*       本例程演示了在3寸TFT屏是显示一副16位色图片，并在图片上透明叠加两个不同显示方向的字符串
*       图片大小范围400X240之内。 字符串取模尺寸：单色400X240 之内 取模X轴长度为8的整数倍数。
*       图片取模软件：img2lcd
*       字符取模软件：ZIMO3
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-06-7 sun68  创建该文件
*/
/* Includes ------------------------------------------------------------------*/
#include "fsmc_sram.h" 
#include "stm32f10x_fsmc.h"




/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);  
extern void LCD_Init(void);
extern void LCD_test(void);
void lcd_rst(void);
void Delay(__IO uint32_t nCount);

/////////////主函数////////////////////////////
int main(void)	
{  
  RCC_Configuration();   	//系统时钟配置为72MHz
  FSMC_LCD_Init();				//FSMC总线配置		  
  LCD_Init();							//液晶初始化		  
  while (1)
  {					  
  	LCD_test();		 
  }
}

//设置系统时钟，通过9倍频，将系统时钟设置为72MHz
void RCC_Configuration(void)
{   
  SystemInit();
}											

//////////FSMC 接口设置///////////////////////////////////////////
void FSMC_LCD_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;	
  GPIO_InitTypeDef GPIO_InitStructure;	    
  //使能FSMC外设时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);//使能FSMC外设时钟 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			  //LCD背光控制  PD13
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽式输出
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 		  //LCD复位   PE1
  GPIO_Init(GPIOE, &GPIO_InitStructure);   	   	
  // 复用端口为FSMC接口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽式输出
  GPIO_Init(GPIOD, &GPIO_InitStructure);   
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);    
  //FSMC NE1  LCD片选
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //FSMC RS---LCD指令 指令/数据	切换
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
  GPIO_Init(GPIOD, &GPIO_InitStructure); 	
  GPIO_SetBits(GPIOD, GPIO_Pin_13);			           //LCD背光打开
  
  //FSMC接口特性配置
  p.FSMC_AddressSetupTime = 0x02;//地址建立时间
  p.FSMC_AddressHoldTime = 0x00;//地址保持时间
  p.FSMC_DataSetupTime = 0x05;	//数据建立时间
  p.FSMC_BusTurnAroundDuration = 0x00;//总线恢复时间
  p.FSMC_CLKDivision = 0x00;//分频系数
  p.FSMC_DataLatency = 0x00;////数据保持时间
  p.FSMC_AccessMode = FSMC_AccessMode_B;//访问模式B

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//使用BANK子板块1
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;//禁止地址线复用
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;//储存器类型
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//数据宽度16位
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;//关闭突发模式访问
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;//等待信号优先级
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;//关闭Wrapped burst access mode
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;//等待信号设置
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;//使能BANK写操作
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;//关闭等待信号
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;//读写时序不一致
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;//同步写
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;//读时序参数
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;//写时序参数
 
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 		
  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}


// ++++++++++++++++TFT 复位操作
void lcd_rst(void){
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	//PE1 为LCD 复位信号
  Delay(0xAFFf);					   
  GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
	Delay(0xAFFf);	
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


