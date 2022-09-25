/****************************************************************************
* Copyright (C), 2018 奋斗嵌入式工作室 ourstm.5d6d.com
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3,MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
* 奋斗板论坛：www.ourstm.net  
*
* 文件名: main.c
*       本例程演示了在3寸TFT屏是显示一副16位色图片，并在图片上透明叠加两个不同显示方向的字符串
*       图片大小范围400X240之内。 字符串取模尺寸：单色400X240 之内 取模X轴长度为8的整数倍数。
*       图片取模软件：img2lcd
*       字符取模软件：ZIMO3
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.3    2012-12-13 sun68  创建该文件
* v0.4    2016-11-12 sun68  修改该文件  驱动改为ILI9341
* v0.4    2018-06-19 sun68  修改该文件  驱动改为ILI9327
*/

#include "fsmc_sram.h"

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //显示区数据地址	  
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //显示区指令地址

unsigned long color1=0;

void LCD_Init(void);
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);
void LCD_WR_Data(unsigned int val);
unsigned char *num_pub(unsigned  int a);

void LCD_test(void);
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr); 

extern unsigned char pic[];
extern unsigned char zf1[];
extern unsigned char zf2[];	
extern unsigned char zf3[];	
extern unsigned char zm0[];
extern unsigned char zm1[];
extern unsigned char zm2[];
extern unsigned char zm3[];
extern unsigned char zm4[];
extern unsigned char zm5[];
extern unsigned char zm6[];
extern unsigned char zm7[];
extern unsigned char zm8[];
extern unsigned char zm9[];
extern unsigned char a1[];
extern unsigned char a2[];
extern unsigned char a3[];

unsigned int LCD_RD_data(void);
extern void lcd_rst(void);
extern void Delay(__IO uint32_t nCount);


/****************************************************************************
* 名    称：void ili9341_DrawPicture(u16 StartX,u16 StartY, u8 Dir，u8 *pic)
* 功    能：在指定座标范围显示一副图片
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*			Dir		   图像显示方向       
*           pic        图片头指针
* 出口参数：无
* 说    明：图片取模格式为水平扫描，16位颜色模式  取模软件img2LCD
* 调用方法：ili9341_DrawPicture(0,0,0,(u16*)demo);
****************************************************************************/
void DrawPicture(u16 StartX,u16 StartY,u8 Dir,u8 *pic)
{
  u32  i=8, len;
  u16 temp,x,y;
  	  
  x=((uint16_t)(pic[2]<<8)+pic[3])-1;	  	//从图像数组里取出图像的长度
  y=((uint16_t)(pic[4]<<8)+pic[5])-1;	  	//从图像数组里取出图像的高度
  if(Dir==0){
		LCD_WR_REG(0x36); // Memory Access Control 
		LCD_WR_Data(0x48); 	 
		LCD_WR_REG(0x37); 
		LCD_WR_Data(0x00);
		LCD_WR_Data(0x00);  

    LCD_WR_REG(0X2A); 
    LCD_WR_Data(StartX>>8);       //start 
		LCD_WR_Data(StartX&0xff);
		LCD_WR_Data((StartX+x)>>8);   //end
		LCD_WR_Data((StartX+x)&0xff);
		LCD_WR_REG(0X2B); 
		LCD_WR_Data(StartY>>8);   		//start
		LCD_WR_Data(StartY&0xff);
		LCD_WR_Data((StartY+y)>>8);   //end
		LCD_WR_Data((StartY+y)&0xff);  
  }	 
  else if(Dir==1){
  	LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_Data((1<<7)|(1<<6)|(1<<5)|(1<<3)); 
		LCD_WR_REG(0x37); //
		LCD_WR_Data(0x00);
		LCD_WR_Data(0x20);  

    LCD_WR_REG(0X2A); 
    LCD_WR_Data(StartX>>8);       //start 
		LCD_WR_Data(StartX&0xff);
		LCD_WR_Data(((StartX+x)>>8)); //end
		LCD_WR_Data(((StartX+x)&0xff));
		LCD_WR_REG(0X2B); 
		LCD_WR_Data(StartY>>8);   		//start
		LCD_WR_Data(StartY&0xff);
		LCD_WR_Data((StartY+y)>>8);   //end
		LCD_WR_Data((StartY+y)&0xff);     
  }	 
  LCD_WR_REG(0x2c);				          	//写数据到显示区

  len=2*((uint16_t)(pic[2]<<8)+pic[3])*((uint16_t)(pic[4]<<8)+pic[5]);   //计算出图像所占的字节数  
  while(i<(len+8)) {							 //从图像数组的第9位开始递增
  	temp=(uint16_t)( pic[i]<<8)+pic[i+1];		 //16位总线， 需要一次发送2个字节的数据
  	LCD_WR_Data(temp);							 //将取出的16位像素数据送入显示区
	i=i+2;										 //取模位置加2，以为获取下一个像素数据
  }
}

//写寄存器地址函数
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;  

}

//写寄存器数据函数
//输入：dbw 数据位数，1为16位，0为8位。
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

//读显示地址数据
unsigned int LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //空操作
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //读出的实际16位像素数据	  
	return(a);	
}

/****************************************************************************
* 名    称：u16 ili9320_BGR2RGB(u16 c)
* 功    能：RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* 入口参数：c - BRG 颜色值
* 出口参数：RGB 颜色值
* 说    明：内部函数调用
* 调用方法：
****************************************************************************/
u16 BGR2RGB(u16 c)
{
  u16  r, g, b;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  return( (b<<11) + (g<<5) + (r<<0) );
}

//写16位像素数据函数
void  LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}



//初始化函数
void LCD_Init(void)
{
	lcd_rst();	 				//硬件复位
  LCD_WR_REG(0x11);   //SLP OUT 
	LCD_WR_REG(0xF3);   //Set EQ 
	LCD_WR_Data(0x08);    
	LCD_WR_Data(0x20);    
	LCD_WR_Data(0x20);    
	LCD_WR_Data(0x08);    
	
	LCD_WR_REG(0xE7);  
	LCD_WR_Data(0x60);   //OPON 
	
	LCD_WR_REG(0xD1);  	 // Set VCOM 
	LCD_WR_Data(0x00);    
	LCD_WR_Data(0x5D);    
	LCD_WR_Data(0x15);    
	
	LCD_WR_REG(0xD0); //Set power related setting 
	LCD_WR_Data(0x07);    
	LCD_WR_Data(0x02);//VGH:15V,VGL:-7.16V (BOE LCD: VGH:12~18V,VGL:-6~10V)  
	LCD_WR_Data(0x8B);    
	LCD_WR_Data(0x03);    
	LCD_WR_Data(0xD4);    
	
	LCD_WR_REG(0x3A); // Set_pixel_format
	LCD_WR_Data(0x55);//0x55:16bit/pixel,65k;0x66:18bit/pixel,262k;

	LCD_WR_REG(0x36); //Set_address_mode
	LCD_WR_Data((1<<7)|(1<<6)|(1<<5)|(1<<3)); 
 	LCD_WR_REG(0x37); 
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x20);  

	LCD_WR_REG(0x21); //Enter_invert_mode
	
	LCD_WR_REG(0xC1); //Set Normal/Partial mode display timing  
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
	LCD_WR_Data(0x04); //72Hz
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
	LCD_WR_REG(0x2A); 
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0xeF);
	LCD_WR_REG(0x2B); 
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x01);
	LCD_WR_Data(0x8F);  
	
	LCD_WR_REG(0xCA);  //Set DGC LUT
	LCD_WR_Data(0x00);     
   
	LCD_WR_REG(0xEA);  //Set DGC
	LCD_WR_Data(0x80);    
	
	LCD_WR_REG(0x29);  // Set_display_on  
	
	color1=0; 
}

/****************************************************************************
* 名    称：lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr) 
* 功    能：在指定座标显示一串字符透明叠加在背景图片上
* 入口参数：StartX     行起始座标	 0-239
*           StartY     列起始座标	 0-319
*           X          长(为8的倍数）0-320
*           Y          宽			 0-240
*			Color      颜色0-65535
*           Dir		   图像显示方向
*           chr        字符串指针
* 出口参数：无
* 说    明：字符取模格式为单色字模，横向取模，字节正序  取模软件：ZIMO3
* 调用方法：lcd_wr_zf(0,0,100,100,(u16*)demo);
****************************************************************************/
//+++++++++++++++++++++++LCD写字符子程序
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr)
{	unsigned int temp=0,num,R_dis_mem=0,Size=0,x=0,y=0,i=0;

 if(Dir==0){	
		LCD_WR_REG(0x36);  // Memory Access Control 
		LCD_WR_Data(0x48); 
   	LCD_WR_REG(0X2A); 
    LCD_WR_Data(StartX>>8);        //start 
		LCD_WR_Data(StartX&0xff);
		LCD_WR_Data(((StartX+X-1)>>8));//end
		LCD_WR_Data(((StartX+X-1)&0xff));
		LCD_WR_REG(0X2B); 
		LCD_WR_Data(StartY>>8);   		//start
		LCD_WR_Data(StartY&0xff);
		LCD_WR_Data((StartY+Y-1)>>8); //end
		LCD_WR_Data((StartY+Y-1)&0xff); 

		Size=X*Y;						  //字符串或字符占用的像素尺寸
		while(i<Size){					  
			temp=*chr++;				  //一个字节代表8个像素，因此加1代表索引到下8个像素
			for(num=0; num<8; num++){	//数组的每个字节代表了8个像素	    
				if((temp&0x80)>0){		  //对字节的各位进行判断，为1的用带入参数的16位颜色值标示，写入到像素位置。						 
					LCD_WR_REG(0x2B); 
					LCD_WR_Data((StartY+y)>>8); 
					LCD_WR_Data((StartY+y)&0XFF);
					LCD_WR_Data(0x2A); 
					LCD_WR_Data((StartX+x)>>8); 
					LCD_WR_Data((StartX+x)&0XFF);
													   
					LCD_WR_REG(44);         //准备写数据显示区			
					LCD_WR_Data(Color); 		
				}
		    else{
					LCD_WR_REG(44);         //准备写数据显示区
					LCD_WR_Data(0xffff); 		//对字节的各位进行判断，为0的用当前背景像素16位颜色值标示 						
				}
				temp=temp<<1; 		  			//字节各位的移出
				x++;
				if(x>=X){x=0; y++;}				//计算像素递增为当前的x和y，为当前像素读背景颜色做准备
				i++;	
			}				  
							
		}
	}
	else if(Dir==1){
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_Data((1<<7)|(1<<6)|(1<<5)|(1<<3)); 
   	LCD_WR_REG(0X2A); 
    LCD_WR_Data(StartX>>8);         //start 
		LCD_WR_Data(StartX&0xff);
		LCD_WR_Data(((StartX+X-1)>>8));//end
		LCD_WR_Data(((StartX+X-1)&0xff));
		LCD_WR_REG(0X2B); 
		LCD_WR_Data(StartY>>8);        //start
		LCD_WR_Data(StartY&0xff);
		LCD_WR_Data((StartY+Y-1)>>8);  //end
		LCD_WR_Data((StartY+Y-1)&0xff); 
		LCD_WR_REG(0x2c);
		
		Size=X*Y;						  		//字符串或字符占用的像素尺寸
		while(i<Size){
							  
			temp=*chr++;				  	//一个字节代表8个像素，因此加1代表索引到下8个像素
			for(num=0; num<8; num++){	  	//数组的每个字节代表了8个像素	    
				if((temp&0x80)>0){		  		//对字节的各位进行判断，为1的用带入参数的16位颜色值标示，写入到像素位置。						 
					LCD_WR_REG(0x3c);
					LCD_WR_Data(Color); 		
				}
				else{			
					LCD_WR_REG(0x3c);					//准备读数据显示区
					LCD_WR_Data(0xffff); 		  //对字节的各位进行判断，为0的用当前背景像素16位颜色值标示。 							
				}
				temp=temp<<1; 		  				//字节各位的移出
				x++;
				if(x>=X){x=0; y++;}				  //计算像素递增为当前的x和y，为当前像素读背景颜色做准备
				i++;	
			}							
		}
	}
}


//演示程序
void LCD_test(void)
{
	unsigned char *p;

	DrawPicture(0,0,0,a1);	    	//在某一指定位置显示图片， 图片的尺寸要在240X400范围内。  
	Delay(0xafffff);
	
	lcd_wr_zf(400,0,32,128,color1,1,&zf1[0]); //显示字符串
	Delay(0xafffff);	
	
	DrawPicture(0,0,1,a2);	    	//在某一指定位置显示图片， 图片的尺寸要在400X240范围内。      
	Delay(0xafffff);
  
	//lcd_wr_zf(0,0,280,32,color1,1,&zf3[0]);  		//显示字符串
	
  lcd_wr_zf(0,0,264,32,color1,1,&zf2[0]);  //显示字符串
	
	Delay(0xafffff);	
	/**
	//显示5位的数字， 数值按COLOR1值周期变化
	p=num_pub((color1/10000));
	lcd_wr_zf(50,30,24,32,color1,0,p);  
	
	p=num_pub((color1%10000)/1000);
	lcd_wr_zf(74,30,24,32,color1,0,p);  
	
	p=num_pub(((color1%10000)%1000)/100);
	lcd_wr_zf(98,30,24,32,color1,0,p);  

	p=num_pub((((color1%10000)%1000)%100)/10);
	lcd_wr_zf(122,30,24,32,color1,0,p);  

	p=num_pub((color1%10));
	lcd_wr_zf(146,30,24,32,color1,0,p);  
	
	Delay(0xafffff);	**/

	
	
	//颜色渐变
	color1++; 
	if(color1>=65536) color1=0;  
		
}

//++++++++根据数值获取各位的数字字符数据首地址+++++++++++++++++++++++++++++++++++++++++++
unsigned char *num_pub(unsigned  int a){
	unsigned char *b;
	switch(a){
		case 0x01: b=&zm1[0]; break;
 		case 0x02: b=&zm2[0]; break;
		case 0x03: b=&zm3[0]; break;
		case 0x04: b=&zm4[0]; break;
		case 0x05: b=&zm5[0]; break;
		case 0x06: b=&zm6[0]; break;
		case 0x07: b=&zm7[0]; break;
		case 0x08: b=&zm8[0]; break;
		case 0x09: b=&zm9[0]; break;
		case 0x00: b=&zm0[0]; break;
		default: b=&zm0[0];break;
	}
	return(b);
}
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
