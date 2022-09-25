/****************************************************************************
* Copyright (C), 2018 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V5上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*							  
* 内容简述:
*       本例程提供了奋斗板配3寸屏模块(ILI9327)的驱动程序
		
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.1    2018-5-18 sun68  创建该文件
*
*/


#include "fsmc_sram.h"

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR



void LCD_Init1(void);
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);

void LCD_WR_Data(unsigned int val);

void LCD_test(void);
void LCD_clear(unsigned int p);

void Delay(__IO uint32_t nCount);

unsigned short LCD_RD_data(void);
extern void lcd_rst(void);
extern void Delay(__IO uint32_t nCount);


/****************************************************************************
* 名    称：LCD_WR_REG(unsigned int index)
* 功    能：FSMC写显示器寄存器地址函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}			 

/****************************************************************************
* 名    称：void LCD_WR_CMD(unsigned int index,unsigned int val)
* 功    能：FSMC写显示器寄存器数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}


/****************************************************************************
* 名    称：unsigned short LCD_RD_data(void)
* 功    能：FSMC读显示区16位数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
unsigned short LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //空操作
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //读出的实际16位像素数据	  
	return(a);	
}



/****************************************************************************
* 名    称：LCD_WR_Data(unsigned int val)
* 功    能：FSMC写16位数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
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
* 名    称：void LCD_Init(void)
* 功    能：2.4 寸TFT 初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_Init1(void)
{	unsigned int i;   
//    GPIO_ResetBits(GPIOE, GPIO_Pin_1);
//    Delay(0xAFFff);					   
//    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 //V7	 
//	Delay(0xAFFff);	

	LCD_WR_REG(0x11);  // SLP OUT 

	
	LCD_WR_REG(0xD1);  // Set VCOM 
	LCD_WR_Data(0x00);    
	LCD_WR_Data(0x5D);    
	LCD_WR_Data(0x15);    
	
	LCD_WR_REG(0xD0); //Set power related setting 
	LCD_WR_Data(0x07);    
	LCD_WR_Data(0x02);   //VGH:15V,VGL:-7.16V (BOE LCD: VGH:12~18V,VGL:-6~10V)  
	LCD_WR_Data(0x8B);    
	LCD_WR_Data(0x03);    
	LCD_WR_Data(0xD4);    
	
	LCD_WR_REG(0x3A); // Set_pixel_format
	LCD_WR_Data(0x55); //0x55:16bit/pixel,65k;0x66:18bit/pixel,262k;

	LCD_WR_REG(0x36); //Set_address_mode
	LCD_WR_Data((1<<7)|(1<<6)|(1<<5)|(1<<3)); 
	
	//LCD_WR_Data(0x48); 
	LCD_WR_REG(0x37); //
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x20);  

 	LCD_WR_REG(0x21); //Enter_invert_mode
	
	LCD_WR_REG(0xC1);   //Set Normal/Partial mode display timing  
	LCD_WR_Data(0x10);    
	LCD_WR_Data(0x1A);    
	LCD_WR_Data(0x02);    
	LCD_WR_Data(0x02);    
	
	LCD_WR_REG(0xD1);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x71);
	LCD_WR_Data(0x19);
	
	LCD_WR_REG(0xD0);
	LCD_WR_Data(0x07);
	LCD_WR_Data(0x01);
	LCD_WR_Data(0x08); 
	
	LCD_WR_REG(0xC0);  //Set display related setting  
	LCD_WR_Data(0x10);    
	LCD_WR_Data(0x31);    
	LCD_WR_Data(0x00);    
	LCD_WR_Data(0x00);    
	LCD_WR_Data(0x01);    
	LCD_WR_Data(0x02);    
	
	LCD_WR_REG(0xC4);  //Set waveform timing 
	LCD_WR_Data(0x01);    
	
	LCD_WR_REG(0xC5);  //Set oscillator 
	LCD_WR_Data(0x04);    //72Hz
	LCD_WR_Data(0x01); 
	
	LCD_WR_REG(0xD2);  //Set power for normal mode 
	LCD_WR_Data(0x01);    
	LCD_WR_Data(0x44);    
	
	LCD_WR_REG(0xC8); //Set Gamma
	LCD_WR_Data(0x04);
	LCD_WR_Data(0x67);
	LCD_WR_Data(0x35);
	LCD_WR_Data(0x04);
	LCD_WR_Data(0x08);
	LCD_WR_Data(0x06);
	LCD_WR_Data(0x24);
	LCD_WR_Data(0x01);
	LCD_WR_Data(0x37);
	LCD_WR_Data(0x40);
	LCD_WR_Data(0x03);
	LCD_WR_Data(0x10);
	LCD_WR_Data(0x08);
	LCD_WR_Data(0x80);
	LCD_WR_Data(0x00);
	
	LCD_WR_REG(0x2B); 
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0xeF);
	LCD_WR_REG(0x2A); 
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x01);
	LCD_WR_Data(0x8F);  
	
  

	LCD_WR_REG(0xCA);  //Set DGC LUT
	LCD_WR_Data(0x00);     
   
	LCD_WR_REG(0xEA);  //Set DGC
	LCD_WR_Data(0x80);    
	
	LCD_WR_REG(0x29);  // Set_display_on  
 
}

/******************* (C) COPYRIGHT 2018 奋斗STM32 *****END OF FILE****/

