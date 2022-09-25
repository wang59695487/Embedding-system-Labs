#include "stm32f10x.h"
#include "lcd.h"
#include "lcd_font.h"
#include "lcdconf.h"

extern void LCD_WR_Data(unsigned int val);
extern unsigned int LCD_RD_data(void);
extern void LCD_Init1(void);



/****************************************************************************
* ��    �ƣ�void LCD_Initializtion()
* ��    �ܣ���ʼ�� ILI9327 ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����LCD_Initializtion();
****************************************************************************/
void LCD_Initializtion()
{ 
   LCD_Init1();
}

/****************************************************************************
* ��    �ƣ�void LCD_SetCursor(u16 x,u16 y)
* ��    �ܣ�������Ļ����
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������
* ˵    ����
* ���÷�����LCD_SetCursor(10,10);
****************************************************************************/
void LCD_SetCursor(u16 x,u16 y)
{				
 
  *(__IO uint16_t *) (Bank1_LCD_C)= 0x2B;	
  *(__IO uint16_t *) (Bank1_LCD_D)= y>>8;
  *(__IO uint16_t *) (Bank1_LCD_D)= y&0xff;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x2A;	
  *(__IO uint16_t *) (Bank1_LCD_D)= x>>8;
  *(__IO uint16_t *) (Bank1_LCD_D)= x&0xff;
}

/****************************************************************************
* ��    �ƣ�void LCD_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
* ��    �ܣ����ô�������
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*           EndX       �н�������
*           EndY       �н�������
* ���ڲ�������
* ˵    ����
* ���÷�����LCD_SetWindows(0,0,100,100)��
****************************************************************************/
void LCD_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{


		
  *(__IO uint16_t *) (Bank1_LCD_C)= 0x2A;	
  *(__IO uint16_t *) (Bank1_LCD_D)= StartX>>8;
  *(__IO uint16_t *) (Bank1_LCD_D)= StartX&0xff;
  *(__IO uint16_t *) (Bank1_LCD_D)= EndX>>8;
  *(__IO uint16_t *) (Bank1_LCD_D)= EndX&0xff;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x2B;	
  *(__IO uint16_t *) (Bank1_LCD_D)= StartY>>8;
  *(__IO uint16_t *) (Bank1_LCD_D)= StartY&0xff;
  *(__IO uint16_t *) (Bank1_LCD_D)= EndY>>8;
  *(__IO uint16_t *) (Bank1_LCD_D)= EndY&0xff;
}


/****************************************************************************
* ��    �ƣ�u16 LCD_GetPoint(u16 x,u16 y)
* ��    �ܣ���ȡָ���������ɫֵ
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������ǰ������ɫֵ
* ˵    ����
* ���÷�����i=LCD_GetPoint(10,10);
****************************************************************************/
u16 LCD_GetPoint(u16 x,u16 y)
{ 
  
  *(__IO uint16_t *) (Bank1_LCD_C)= 0x2B;	
  *(__IO uint16_t *) (Bank1_LCD_D)= y>>8;
  *(__IO uint16_t *) (Bank1_LCD_D)= y&0xff;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x2A;	
  *(__IO uint16_t *) (Bank1_LCD_D)= x>>8;
  *(__IO uint16_t *) (Bank1_LCD_D)= x&0xff;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x2e;	
  //return (LCD_BGR2RGB(LCD_ReadData()));
  return (LCD_ReadData());

}

/****************************************************************************
* ��    �ƣ�void LCD_SetPoint(u16 x,u16 y,u16 point)
* ��    �ܣ���ָ�����껭��
* ��ڲ�����x      ������
*           y      ������
*           point  �����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����LCD_SetPoint(10,10,0x0fe0);
****************************************************************************/
void LCD_SetPoint(u16 x,u16 y,u16 point)
{
  *(__IO uint16_t *) (Bank1_LCD_C)= 0x2B;	
  *(__IO uint16_t *) (Bank1_LCD_D)= y>>8;
  *(__IO uint16_t *) (Bank1_LCD_D)= y&0xff;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x2A;	
  *(__IO uint16_t *) (Bank1_LCD_D)= x>>8;
  *(__IO uint16_t *) (Bank1_LCD_D)= x&0xff;

  *(__IO uint16_t *) (Bank1_LCD_C)= 44;	

  *(__IO uint16_t *) (Bank1_LCD_D)= point;

 
}


/****************************************************************************
* ��    �ƣ�void LCD_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����LCD_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void LCD_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;
  
  u8 tmp_char=0;

  for (i=0;i<16;i++)
  {
    tmp_char=ascii_8x16[((c-0x20)*16)+i];
    for (j=0;j<8;j++)
    {
      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
        {
          LCD_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
        }
        else
        {
          LCD_SetPoint(x+j,y+i,bkColor); // ������ɫ
        }
    }
  }
}



/****************************************************************************
* ��    �ƣ�u16 LCD_BGR2RGB(u16 c)
* ��    �ܣ�RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* ��ڲ�����c      BRG ��ɫֵ
* ���ڲ�����RGB ��ɫֵ
* ˵    �����ڲ���������
* ���÷�����
****************************************************************************/
u16 LCD_BGR2RGB(u16 c)
{
  u16  r, g, b;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  return( (b<<11) + (g<<5) + (r<<0) );
}



/****************************************************************************
* ��    �ƣ�u16 LCD_ReadData(void)
* ��    �ܣ���ȡ����������
* ��ڲ�������
* ���ڲ��������ض�ȡ��������
* ˵    �����ڲ�����
* ���÷�����i=LCD_ReadData();
****************************************************************************/
u16 LCD_ReadData(void)
{
  u16 val=0;
  val=LCD_RD_data();
  return val;
}

/******************* (C) COPYRIGHT 2012 �ܶ�STM32 *****END OF FILE****/
