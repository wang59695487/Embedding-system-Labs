/****************************************************************************
* Copyright (C), 2011 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V2,2.1,V3,MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
* �ܶ�����̳��www.ourstm.net  
*
* �ļ���: main.c
* ���ݼ���:
*       ��������ʾ����3��TFT������ʾһ��16λɫͼƬ������ͼƬ��͸������������ͬ��ʾ������ַ���
*       ͼƬ��С��Χ400X240֮�ڡ� �ַ���ȡģ�ߴ磺��ɫ400X240 ֮�� ȡģX�᳤��Ϊ8������������
*       ͼƬȡģ�����img2lcd
*       �ַ�ȡģ�����ZIMO3
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.1    2011-12-5 sun68  �������ļ�
*/


#include "fsmc_sram.h"

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //��ʾ�����ݵ�ַ	  
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //��ʾ��ָ���ַ

unsigned long color1=0;

void LCD_Init(void);
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);
void LCD_WR_Data(unsigned int val);

void LCD_test(void);
void LCD_clear(unsigned int p);
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr); 
unsigned char *num_pub(unsigned int a);


extern unsigned char zm9[];
extern unsigned char zm8[];
extern unsigned char zm7[];
extern unsigned char zm6[];
extern unsigned char zm5[];
extern unsigned char zm4[];
extern unsigned char zm3[];
extern unsigned char zm2[];
extern unsigned char zm1[];
extern unsigned char zm0[];
extern unsigned char a1[];

extern unsigned char zf2[];
extern unsigned char zf3[];	
extern unsigned char pic[];
extern unsigned char a2[];
extern unsigned char a3[];

unsigned int LCD_RD_data(void);
extern void lcd_rst(void);
extern void Delay(__IO uint32_t nCount);


/****************************************************************************
* ��    �ƣ�void lcd_DrawPicture(u16 StartX,u16 StartY, u8 Dir��u8 *pic)
* ��    �ܣ���ָ�����귶Χ��ʾһ��ͼƬ
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*			Dir		   ͼ����ʾ����       
*           pic        ͼƬͷָ��
* ���ڲ�������
* ˵    ����ͼƬȡģ��ʽΪˮƽɨ�裬16λ��ɫģʽ  ȡģ���img2LCD
* ���÷�����lcd_DrawPicture(0,0,0,(u16*)demo);
****************************************************************************/
void lcd_DrawPicture(u16 StartX,u16 StartY,u8 Dir,u8 *pic)
{
  u32  i=8, len;
  u16 temp,x,y;
  	  
  
  x=((uint16_t)(pic[2]<<8)+pic[3])-1;	  	//��ͼ��������ȡ��ͼ��ĳ���
  y=((uint16_t)(pic[4]<<8)+pic[5])-1;	  	//��ͼ��������ȡ��ͼ��ĸ߶�
  if(Dir==0){
	LCD_WR_CMD(0x0003,0x1030);   			//ͼ����ʾ����Ϊ������  �е���  �еݼ�
    LCD_WR_CMD(0x0210, StartX); 			//ˮƽ��ʾ����ʼ��ַ 0-239
  	LCD_WR_CMD(0x0211, StartX+x);           	//ˮƽ��ʾ��������ַ 0-239
  	LCD_WR_CMD(0x0212, StartY);     		//��ֱ��ʾ����ʼ��ַ 0-399
  	LCD_WR_CMD(0x0213, StartY+y);         	//��ֱ��ʾ��������ַ 0-399
  
  	LCD_WR_CMD(0x0200, StartX);		          	//ˮƽ��ʾ����ַ
  	LCD_WR_CMD(0x0201, StartY);		      		//��ֱ��ʾ����ַ
  }	 
  else if(Dir==1){
	LCD_WR_CMD(0x0003,0x1018);   			//ͼ����ʾ����Ϊ������  �е���  �еݼ�
    LCD_WR_CMD(0x0210, StartY); 			//ˮƽ��ʾ����ʼ��ַ 0-239
  	LCD_WR_CMD(0x0211, StartY+y);           //ˮƽ��ʾ��������ַ 0-239
  	LCD_WR_CMD(0x0212, 399-(x+StartX));     //��ֱ��ʾ����ʼ��ַ 0-399
  	LCD_WR_CMD(0x0213, 399-StartX);         //��ֱ��ʾ��������ַ 0-399
  
  	LCD_WR_CMD(0x200, StartY);		          	//ˮƽ��ʾ����ַ
  	LCD_WR_CMD(0x201, 399-StartX);		      	//��ֱ��ʾ����ַ
  }	 
  LCD_WR_REG(0x0202);				          	//д���ݵ���ʾ��

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
/****************************************************************************
* ��    �ƣ�u16 ili9320_BGR2RGB(u16 c)
* ��    �ܣ�RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* ��ڲ�����c      BRG ��ɫֵ
* ���ڲ�����RGB ��ɫֵ
* ˵    �����ڲ���������
* ���÷�����
****************************************************************************/
u16 lcd_BGR2RGB(u16 c)
{
  u16  r, g, b;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  return( (b<<11) + (g<<5) + (r<<0) );
}

//����ʾ��ַ����
unsigned int LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //�ղ���
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //������ʵ��16λ��������	  
	return(lcd_BGR2RGB(a));	
}


//д16λ�������ݺ���
void    LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}



/****************************************************************************
* ��    �ƣ�void LCD_Init(void)
* ��    �ܣ�3 ��TFT(R61509) ��ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LCD_Init(void)
{
	unsigned int i;
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	  //Ӳ����λ
    Delay(0x1AFFf);			   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 
	Delay(0x1AFFf);	

	LCD_WR_CMD(0x0702, 0x3008); 
    LCD_WR_CMD(0x0705, 0x0036);  
    LCD_WR_CMD(0x070B, 0x1213);  
 
    LCD_WR_CMD(0x0001, 0x0100); // set SS and SM bit
    LCD_WR_CMD(0x0002, 0x0100); // set 1 line inversion
    LCD_WR_CMD(0x0003, 0x1030); // set GRAM write direction and BGR=1.


    
    LCD_WR_CMD(0x0008, 0x0202); // set the back porch and front porch
    LCD_WR_CMD(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
    LCD_WR_CMD(0x000A, 0x0000); // FMARK function
    LCD_WR_CMD(0x000C, 0x0000); // RGB interface setting
    LCD_WR_CMD(0x000D, 0x0000); // Frame marker Position
    LCD_WR_CMD(0x000F, 0x0000); // RGB interface polarity
    //Power On sequence

    

    LCD_WR_CMD(0x0100, 0x0000); // // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WR_CMD(0x0102, 0x0000); // VREG1OUT voltage
    LCD_WR_CMD(0x0103, 0x0000); // VDV[4:0] for VCOM amplitude
    for(i=600;i>0;i--);
    LCD_WR_CMD(0x0100, 0x1190); // // SAP, BT[3:0], AP, DSTB, SLP, STB,0x1190
    LCD_WR_CMD(0x0101, 0x0005); // DC1[2:0], DC0[2:0], VC[2:0]//0x0227
    for(i=600;i>0;i--);
    LCD_WR_CMD(0x0102, 0x01b8); // VREG1OUT voltage
    for(i=600;i>0;i--);
    LCD_WR_CMD(0x0103, 0x3600); //DV[4:0] for VCOM amplitude
    LCD_WR_CMD(0x0281, 0x0001); // VCM[5:0] for VCOMH
    for(i=600;i>0;i--);
    LCD_WR_CMD(0x0200, 0x0000); // GRAM horizontal Address
     
    LCD_WR_CMD(0x0201, 0x0000); //GRAM horizontal Address

    //Adjust the Gamma Curve
    
    LCD_WR_CMD(0x0300, 0x0101);//
    LCD_WR_CMD(0x0301, 0x0506);//
    LCD_WR_CMD(0x0302, 0x0405);//
    
    LCD_WR_CMD(0x0305, 0x0007);//
    
    LCD_WR_CMD(0x0306, 0x0008);//
    
    LCD_WR_CMD(0x0307, 0x0203);//
    LCD_WR_CMD(0x0308, 0x0102);//
    LCD_WR_CMD(0x0309, 0x0706);//
    
    LCD_WR_CMD(0x030C, 0x0700);//
    
    LCD_WR_CMD(0x030D, 0x0008);//
    
      
    //Set GRAM area
    LCD_WR_CMD(0x0210, 0x0000); // Horizontal GRAM Start Address
    LCD_WR_CMD(0x0211, 0x00EF); // Horizontal GRAM End Address
    LCD_WR_CMD(0x0212, 0x0000); // Vertical GRAM Start Address
    LCD_WR_CMD(0x0213, 0x018F); // Vertical GRAM Start Address
    LCD_WR_CMD(0x0400, 0x3100); // Gate Scan Line
    LCD_WR_CMD(0x0401, 0x0001); // NDL,VLE, REV
    LCD_WR_CMD(0x0404, 0x0000); // set scrolling line
    //Partial Display Control
    LCD_WR_CMD(0x0500, 0x0000);
    LCD_WR_CMD(0x0501, 0x0000);
    LCD_WR_CMD(0x0502, 0x0000);
    LCD_WR_CMD(0x0503, 0x0000);
    LCD_WR_CMD(0x0504, 0x0000);
    LCD_WR_CMD(0x0505, 0x0000);
    //Panel Control
    LCD_WR_CMD(0x0010, 0x0010);
    LCD_WR_CMD(0x0011, 0x0600);
    LCD_WR_CMD(0x0020, 0x0002);
     
    LCD_WR_CMD(0x0007, 0x0173); // 262K color and display ON	
	*(__IO uint16_t *) (Bank1_LCD_C)= 0x202;	 //׼��д������ʾ��
	for(i=0;i<96000;i++)
	{
	  LCD_WR_Data(0xffff);					 //�ú�ɫ����
	}  
	color1=0; 
}



/****************************************************************************
* ��    �ƣ�lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr) 
* ��    �ܣ���ָ��������ʾһ���ַ�͸�������ڱ���ͼƬ��
* ��ڲ�����StartX     ����ʼ����	 0-239
*           StartY     ����ʼ����	 0-399
*           X          ��(Ϊ8�ı�����0-400
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


	if(Dir==2) LCD_WR_CMD(0x0003,0x1010);   //ͼ����ʾ����Ϊ������  �еݼ�  �е���  AM=0  I/D[1:0]=00	<--
	else if(Dir==3) LCD_WR_CMD(0x0003,0x1028);   //ͼ����ʾ����Ϊ������  �еݼ�  �е���  AM=1  I/D[1:0]=10	V
  	if(Dir==0){
		LCD_WR_CMD(0x0003,0x1030);		  //ͼ����ʾ����Ϊ������  �е���  �е���  AM=0  I/D[1:0]=11  -->
		LCD_WR_CMD(0x0210, StartX); 	  //ˮƽ��ʾ����ʼ��ַ 0-239
  		LCD_WR_CMD(0x0211, StartX+X-1);   //ˮƽ��ʾ��������ַ 0-239
  		LCD_WR_CMD(0x0212, StartY);       //��ֱ��ʾ����ʼ��ַ 0-399
  		LCD_WR_CMD(0x0213, StartY+Y-1);   //��ֱ��ʾ��������ַ 0-399
		LCD_WR_CMD(0x0200, StartX);		  //ˮƽ��ʾ����ַ
  		LCD_WR_CMD(0x0201, StartY);		  //��ֱ��ʾ����ַ	
		LCD_WR_REG(0x0202);               //׼��д������ʾ��
		Size=X*Y;						  //�ַ������ַ�ռ�õ����سߴ�
		while(i<Size){					  
			temp=*chr++;				  //һ���ֽڴ���8�����أ���˼�1������������8������
			for(num=0; num<8; num++){	  //�����ÿ���ֽڴ�����8������	    
				if((temp&0x80)>0){		  //���ֽڵĸ�λ�����жϣ�Ϊ1���ô��������16λ��ɫֵ��ʾ��д�뵽����λ�á�						 
				
					LCD_WR_Data(Color); 		
				}
				else{
					LCD_WR_CMD(0x0200, StartX+x);		//ˮƽ��ʾ����ַ
  					LCD_WR_CMD(0x0201, StartY+y);		//��ֱ��ʾ����ַ								
					LCD_WR_REG(0x0202);					//׼����������ʾ��
					R_dis_mem=LCD_RD_data();		  	//��ȡ����ɫ��Ϊ���Ӳ���͸��Ч����׼��	
					LCD_WR_Data(R_dis_mem); 		//���ֽڵĸ�λ�����жϣ�Ϊ0���õ�ǰ��������16λ��ɫֵ��ʾ�� 						
				}
				temp=temp<<1; 		  				//�ֽڸ�λ���Ƴ�
				x++;
				if(x>=X){x=0; y++;}				    //�������ص���Ϊ��ǰ��x��y��Ϊ��ǰ���ض�������ɫ��׼��
				i++;	
			}				  
							
		}
	}
	else if(Dir==1){
		LCD_WR_CMD(0x0003,0x1018);   	  		//ͼ����ʾ����Ϊ������  �е���  �еݼ�  AM=1  I/D[1:0]=01	A
		LCD_WR_CMD(0x0210, StartY); 	  		//ˮƽ��ʾ����ʼ��ַ 0-239
  		LCD_WR_CMD(0x0211, StartY+Y-1);    		//ˮƽ��ʾ��������ַ 0-239
  		LCD_WR_CMD(0x0212, 399-(StartX+X-1));   //��ֱ��ʾ����ʼ��ַ 0-399
  		LCD_WR_CMD(0x0213, 399-StartX);    		//��ֱ��ʾ��������ַ 0-399
		LCD_WR_CMD(0x0200, StartY);		  	  	//ˮƽ��ʾ����ַ
  		LCD_WR_CMD(0x0201, 399-StartX);	 	  	//��ֱ��ʾ����ַ	
		LCD_WR_REG(0x0202);                   	//׼��д������ʾ��

		Size=X*Y;						  		//�ַ������ַ�ռ�õ����سߴ�
		while(i<Size){					  
			temp=*chr++;				  		//һ���ֽڴ���8�����أ���˼�1������������8������
			for(num=0; num<8; num++){	  		//�����ÿ���ֽڴ�����8������	    
				if((temp&0x80)>0){		  		//���ֽڵĸ�λ�����жϣ�Ϊ1���ô��������16λ��ɫֵ��ʾ��д�뵽����λ�á�						 
				
					LCD_WR_Data(Color); 		
				}
				else{
					LCD_WR_CMD(0x0200, StartY+y);		//ˮƽ��ʾ����ַ
  					LCD_WR_CMD(0x0201, 399-(StartX+x));	//��ֱ��ʾ����ַ								
					LCD_WR_REG(0x0202);					//׼����������ʾ��
					R_dis_mem=LCD_RD_data();		  	//��ȡ����ɫ��Ϊ���Ӳ���͸��Ч����׼��	
					LCD_WR_Data(R_dis_mem); 		//���ֽڵĸ�λ�����жϣ�Ϊ0���õ�ǰ��������16λ��ɫֵ��ʾ�� 						
				}
				temp=temp<<1; 		  				//�ֽڸ�λ���Ƴ�
				x++;
				if(x>=X){x=0; y++;}				    //�������ص���Ϊ��ǰ��x��y��Ϊ��ǰ���ض�������ɫ��׼��
				i++;	
			}							
		}
	}
}


//��ʾ����
void LCD_test(void)
{

	unsigned char *p;


	lcd_DrawPicture(0,0,0,a1);	    	//��ĳһָ��λ����ʾͼƬ�� ͼƬ�ĳߴ�Ҫ��240X400��Χ�ڡ�  
	Delay(0x1afffff);
	lcd_DrawPicture(0,0,1,a2);	    	//��ĳһָ��λ����ʾͼƬ�� ͼƬ�ĳߴ�Ҫ��400X240��Χ�ڡ�      

	Delay(0x1afffff);

    lcd_wr_zf(0,0,280,32,color1,1,&zf3[0]);  		//��ʾ�ַ���
	
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


	//��ɫ����
	color1++; 
	if(color1>=65536) color1=0;  
	Delay(0xafffff);			
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
