/****************************************************************************
* Copyright (C), 2011 �ܶ�Ƕ��ʽ������ ourstm.5d6d.com
*
* �������� �ܶ���STM32������V2,2.1,V3,MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
* �ܶ�����̳��ourstm.5d6d.com  
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
* v0.2    2011-06-7 sun68  �������ļ�
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

/////////////������////////////////////////////
int main(void)	
{  
  RCC_Configuration();   	//ϵͳʱ������Ϊ72MHz
  FSMC_LCD_Init();				//FSMC��������		  
  LCD_Init();							//Һ����ʼ��		  
  while (1)
  {					  
  	LCD_test();		 
  }
}

//����ϵͳʱ�ӣ�ͨ��9��Ƶ����ϵͳʱ������Ϊ72MHz
void RCC_Configuration(void)
{   
  SystemInit();
}											

//////////FSMC �ӿ�����///////////////////////////////////////////
void FSMC_LCD_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;	
  GPIO_InitTypeDef GPIO_InitStructure;	    
  //ʹ��FSMC����ʱ��
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);//ʹ��FSMC����ʱ�� 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			  //LCD�������  PD13
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //����ʽ���
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 		  //LCD��λ   PE1
  GPIO_Init(GPIOE, &GPIO_InitStructure);   	   	
  // ���ö˿�ΪFSMC�ӿ�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //��������ʽ���
  GPIO_Init(GPIOD, &GPIO_InitStructure);   
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);    
  //FSMC NE1  LCDƬѡ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //FSMC RS---LCDָ�� ָ��/����	�л�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
  GPIO_Init(GPIOD, &GPIO_InitStructure); 	
  GPIO_SetBits(GPIOD, GPIO_Pin_13);			           //LCD�����
  
  //FSMC�ӿ���������
  p.FSMC_AddressSetupTime = 0x02;//��ַ����ʱ��
  p.FSMC_AddressHoldTime = 0x00;//��ַ����ʱ��
  p.FSMC_DataSetupTime = 0x05;	//���ݽ���ʱ��
  p.FSMC_BusTurnAroundDuration = 0x00;//���߻ָ�ʱ��
  p.FSMC_CLKDivision = 0x00;//��Ƶϵ��
  p.FSMC_DataLatency = 0x00;////���ݱ���ʱ��
  p.FSMC_AccessMode = FSMC_AccessMode_B;//����ģʽB

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//ʹ��BANK�Ӱ��1
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;//��ֹ��ַ�߸���
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;//����������
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//���ݿ��16λ
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;//�ر�ͻ��ģʽ����
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;//�ȴ��ź����ȼ�
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;//�ر�Wrapped burst access mode
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;//�ȴ��ź�����
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;//ʹ��BANKд����
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;//�رյȴ��ź�
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;//��дʱ��һ��
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;//ͬ��д
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;//��ʱ�����
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;//дʱ�����
 
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 		
  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}


// ++++++++++++++++TFT ��λ����
void lcd_rst(void){
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	//PE1 ΪLCD ��λ�ź�
  Delay(0xAFFf);					   
  GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
	Delay(0xAFFf);	
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


