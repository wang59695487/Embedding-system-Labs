/****************************************************************************
* Copyright (C), 2018 �ܶ�Ƕ��ʽ������ ourstm.5d6d.com
*
* �������� �ܶ���STM32������V2,2.1,V3,MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
* �ܶ�����̳��www.ourstm.net  
*
* �ļ���: main.c
*       ��������ʾ����3��TFT������ʾһ��16λɫͼƬ������ͼƬ��͸������������ͬ��ʾ������ַ���
*       ͼƬ��С��Χ400X240֮�ڡ� �ַ���ȡģ�ߴ磺��ɫ400X240 ֮�� ȡģX�᳤��Ϊ8������������
*       ͼƬȡģ�����img2lcd
*       �ַ�ȡģ�����ZIMO3
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.3    2012-12-13 sun68  �������ļ�
* v0.4    2016-11-12 sun68  �޸ĸ��ļ�  ������ΪILI9341
* v0.4    2018-06-19 sun68  �޸ĸ��ļ�  ������ΪILI9327
*/

#include "fsmc_sram.h"

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //��ʾ�����ݵ�ַ	  
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //��ʾ��ָ���ַ

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
* ��    �ƣ�void ili9341_DrawPicture(u16 StartX,u16 StartY, u8 Dir��u8 *pic)
* ��    �ܣ���ָ�����귶Χ��ʾһ��ͼƬ
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*			Dir		   ͼ����ʾ����       
*           pic        ͼƬͷָ��
* ���ڲ�������
* ˵    ����ͼƬȡģ��ʽΪˮƽɨ�裬16λ��ɫģʽ  ȡģ���img2LCD
* ���÷�����ili9341_DrawPicture(0,0,0,(u16*)demo);
****************************************************************************/
void DrawPicture(u16 StartX,u16 StartY,u8 Dir,u8 *pic)
{
  u32  i=8, len;
  u16 temp,x,y;
  	  
  x=((uint16_t)(pic[2]<<8)+pic[3])-1;	  	//��ͼ��������ȡ��ͼ��ĳ���
  y=((uint16_t)(pic[4]<<8)+pic[5])-1;	  	//��ͼ��������ȡ��ͼ��ĸ߶�
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
  LCD_WR_REG(0x2c);				          	//д���ݵ���ʾ��

  len=2*((uint16_t)(pic[2]<<8)+pic[3])*((uint16_t)(pic[4]<<8)+pic[5]);   //�����ͼ����ռ���ֽ���  
  while(i<(len+8)) {							 //��ͼ������ĵ�9λ��ʼ����
  	temp=(uint16_t)( pic[i]<<8)+pic[i+1];		 //16λ���ߣ� ��Ҫһ�η���2���ֽڵ�����
  	LCD_WR_Data(temp);							 //��ȡ����16λ��������������ʾ��
	i=i+2;										 //ȡģλ�ü�2����Ϊ��ȡ��һ����������
  }
}

//д�Ĵ�����ַ����
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;  

}

//д�Ĵ������ݺ���
//���룺dbw ����λ����1Ϊ16λ��0Ϊ8λ��
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

//����ʾ��ַ����
unsigned int LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //�ղ���
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //������ʵ��16λ��������	  
	return(a);	
}

/****************************************************************************
* ��    �ƣ�u16 ili9320_BGR2RGB(u16 c)
* ��    �ܣ�RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* ��ڲ�����c - BRG ��ɫֵ
* ���ڲ�����RGB ��ɫֵ
* ˵    �����ڲ���������
* ���÷�����
****************************************************************************/
u16 BGR2RGB(u16 c)
{
  u16  r, g, b;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  return( (b<<11) + (g<<5) + (r<<0) );
}

//д16λ�������ݺ���
void  LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}



//��ʼ������
void LCD_Init(void)
{
	lcd_rst();	 				//Ӳ����λ
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
* ��    �ƣ�lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr) 
* ��    �ܣ���ָ��������ʾһ���ַ�͸�������ڱ���ͼƬ��
* ��ڲ�����StartX     ����ʼ����	 0-239
*           StartY     ����ʼ����	 0-319
*           X          ��(Ϊ8�ı�����0-320
*           Y          ��			 0-240
*			Color      ��ɫ0-65535
*           Dir		   ͼ����ʾ����
*           chr        �ַ���ָ��
* ���ڲ�������
* ˵    �����ַ�ȡģ��ʽΪ��ɫ��ģ������ȡģ���ֽ�����  ȡģ�����ZIMO3
* ���÷�����lcd_wr_zf(0,0,100,100,(u16*)demo);
****************************************************************************/
//+++++++++++++++++++++++LCDд�ַ��ӳ���
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

		Size=X*Y;						  //�ַ������ַ�ռ�õ����سߴ�
		while(i<Size){					  
			temp=*chr++;				  //һ���ֽڴ���8�����أ���˼�1������������8������
			for(num=0; num<8; num++){	//�����ÿ���ֽڴ�����8������	    
				if((temp&0x80)>0){		  //���ֽڵĸ�λ�����жϣ�Ϊ1���ô��������16λ��ɫֵ��ʾ��д�뵽����λ�á�						 
					LCD_WR_REG(0x2B); 
					LCD_WR_Data((StartY+y)>>8); 
					LCD_WR_Data((StartY+y)&0XFF);
					LCD_WR_Data(0x2A); 
					LCD_WR_Data((StartX+x)>>8); 
					LCD_WR_Data((StartX+x)&0XFF);
													   
					LCD_WR_REG(44);         //׼��д������ʾ��			
					LCD_WR_Data(Color); 		
				}
		    else{
					LCD_WR_REG(44);         //׼��д������ʾ��
					LCD_WR_Data(0xffff); 		//���ֽڵĸ�λ�����жϣ�Ϊ0���õ�ǰ��������16λ��ɫֵ��ʾ 						
				}
				temp=temp<<1; 		  			//�ֽڸ�λ���Ƴ�
				x++;
				if(x>=X){x=0; y++;}				//�������ص���Ϊ��ǰ��x��y��Ϊ��ǰ���ض�������ɫ��׼��
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
		
		Size=X*Y;						  		//�ַ������ַ�ռ�õ����سߴ�
		while(i<Size){
							  
			temp=*chr++;				  	//һ���ֽڴ���8�����أ���˼�1������������8������
			for(num=0; num<8; num++){	  	//�����ÿ���ֽڴ�����8������	    
				if((temp&0x80)>0){		  		//���ֽڵĸ�λ�����жϣ�Ϊ1���ô��������16λ��ɫֵ��ʾ��д�뵽����λ�á�						 
					LCD_WR_REG(0x3c);
					LCD_WR_Data(Color); 		
				}
				else{			
					LCD_WR_REG(0x3c);					//׼����������ʾ��
					LCD_WR_Data(0xffff); 		  //���ֽڵĸ�λ�����жϣ�Ϊ0���õ�ǰ��������16λ��ɫֵ��ʾ�� 							
				}
				temp=temp<<1; 		  				//�ֽڸ�λ���Ƴ�
				x++;
				if(x>=X){x=0; y++;}				  //�������ص���Ϊ��ǰ��x��y��Ϊ��ǰ���ض�������ɫ��׼��
				i++;	
			}							
		}
	}
}


//��ʾ����
void LCD_test(void)
{
	unsigned char *p;

	DrawPicture(0,0,0,a1);	    	//��ĳһָ��λ����ʾͼƬ�� ͼƬ�ĳߴ�Ҫ��240X400��Χ�ڡ�  
	Delay(0xafffff);
	
	lcd_wr_zf(400,0,32,128,color1,1,&zf1[0]); //��ʾ�ַ���
	Delay(0xafffff);	
	
	DrawPicture(0,0,1,a2);	    	//��ĳһָ��λ����ʾͼƬ�� ͼƬ�ĳߴ�Ҫ��400X240��Χ�ڡ�      
	Delay(0xafffff);
  
	//lcd_wr_zf(0,0,280,32,color1,1,&zf3[0]);  		//��ʾ�ַ���
	
  lcd_wr_zf(0,0,264,32,color1,1,&zf2[0]);  //��ʾ�ַ���
	
	Delay(0xafffff);	
	/**
	//��ʾ5λ�����֣� ��ֵ��COLOR1ֵ���ڱ仯
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

	
	
	//��ɫ����
	color1++; 
	if(color1>=65536) color1=0;  
		
}

//++++++++������ֵ��ȡ��λ�������ַ������׵�ַ+++++++++++++++++++++++++++++++++++++++++++
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
