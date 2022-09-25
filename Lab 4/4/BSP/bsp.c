
#include "..\App\includes.h"
#include "demo.h"


#define ADC1_DR_Address    ((u32)0x4001244C)  	        //ADC1�����ַ
/* �����˴���оƬ��SPIƬѡ���� */
#define TP_CS()  GPIO_ResetBits(GPIOB,GPIO_Pin_7)	  
#define TP_DCS() GPIO_SetBits(GPIOB,GPIO_Pin_7)

void tp_Config(void) ;
u16 TPReadX(void);
u16 TPReadY(void);
void NVIC_Configuration(void);
extern void FSMC_LCD_Init(void); 
void time_ini(void);
TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;
TIM_OCInitTypeDef  TIM3_OCInitStructure;


/****************************************************************************
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void RCC_Configuration(void){

  SystemInit();	  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | 
  							RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

/****************************************************************************
* ��    �ƣ�void GPIO_Configuration(void)
* ��    �ܣ�ͨ��IO������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /*�Կ���LEDָʾ�Ƶ�IO�ڽ����˳�ʼ�������˿�����Ϊ������������������ٶ�Ϊ50Mhz��PA9,PA10�˿ڸ���Ϊ����1��TX��RX��
  ������ĳ������ʱ������Ӧ�������ڵĶ˿ڵ�ʱ�ӽ���ʹ�ܡ������޷����óɹ��������õ��˶˿�B��D,E�� ���Ҫ���⼸���˿ڵ�ʱ��
  ����ʹ�ܣ�ͬʱ�����õ�����IO�ڹ����������ô��ڡ���˻�Ҫʹ��AFIO�����ù���IO��ʱ�ӡ�*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE); 	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				                 //LED1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_6|GPIO_Pin_3;		 //BackLight, LED2, LED3
  GPIO_Init(GPIOD, &GPIO_InitStructure);

}


void ADC_Configuration(void)//ADC��ʼ��
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

    /* Enable DMA clock */
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

   /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

  	/* DMA channel1 configuration ----------------------------------------------*/
	//ʹ��DMA
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;			            //DMAͨ��1�ĵ�ַ 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	            //DMA���͵�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					            //���ͷ���
	DMA_InitStructure.DMA_BufferSize = 100;								            //�����ڴ��С��100��16λ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				            //�����ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//ADC1ת����������16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;				//���͵�Ŀ�ĵ�ַ��16λ���
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;									//ѭ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
	/* ����DMA1ͨ��1��������ж� */
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);


	//ʹ��DMAͨ��1
	DMA_Cmd(DMA1_Channel1, ENABLE); 
  
  
  //����ADģ������˿�Ϊ���� һ��2·AD ����ͨ��
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//ADC����
	/* ADCת��ʱ�䣺 �� STM32F103xx��ǿ�Ͳ�Ʒ��ʱ��Ϊ56MHzʱΪ1��s(ʱ��Ϊ72MHzΪ1.17��s)
	ADC������Χ0-3.3V    */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                   //����ADC��ʱ��Ϊ72MHZ/6=12M 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC1�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		//ģ��ת��������ɨ��ģʽ����ͨ�������ǵ��Σ���ͨ����ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת��������ɨ��ģʽ����ͨ�������ǵ��Σ���ͨ����ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;//�涨��˳����й���ת����ADCͨ������Ŀ�������Ŀ��ȡֵ��Χ��1��16
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channels configuration [����ģʽͨ������]*/ 

	//ADC1 ����ͨ������
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);	  //ͨ��10����ʱ�� 55.5����
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);	  //ͨ��11����ʱ�� 55.5����
  	

	//ʹ��ADC1 DMA 
	ADC_DMACmd(ADC1, ENABLE);
	//ʹ��ADC1
	ADC_Cmd(ADC1, ENABLE);	
	
	// ��ʼ��ADC1У׼�Ĵ���
	ADC_ResetCalibration(ADC1);
	//���ADC1У׼�Ĵ�����ʼ���Ƿ����
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	//��ʼУ׼ADC1
	ADC_StartCalibration(ADC1);
	//����Ƿ����У׼
	while(ADC_GetCalibrationStatus(ADC1));
	
	//ADC1ת������
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	 
}
/****************************************************************************
* ��    �ƣ�void USART_Config(USART_TypeDef* USARTx,u32 baud)
* ��    �ܣ���������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void USART_Config(USART_TypeDef* USARTx,u32 baud){
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  //usart_init----------------------------------------------------
  /* ���� USART1 Rx (PA.10) Ϊ�������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ���� USART1 Tx (PA.09) Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	


  USART_InitStructure.USART_BaudRate =baud;						                    //����115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		                //����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			                //ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;				                //��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

  USART_Init(USARTx, &USART_InitStructure);							                //���ô��ڲ�������
   USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);                    //ʹ�ܽ����ж�
  /* ʹ�ܴ���1 */
  USART_Cmd(USARTx, ENABLE);	
  
}

/****************************************************************************
* ��    �ƣ�void BSP_Init(void)
* ��    �ܣ��ܶ����ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/  
void BSP_Init(void)
{
  																		   
  RCC_Configuration();  	       //ϵͳʱ�ӳ�ʼ�����˿�����ʱ��ʹ��	  
  NVIC_Configuration();			   //�ж�Դ����
  GPIO_Configuration();			   //״̬LED1�ĳ�ʼ��  	  
  USART_Config(USART1,115200);	   //����1��ʼ��
  tp_Config();					   //SPI1 ������·��ʼ��	    
  FSMC_LCD_Init();				   //FSMC TFT�ӿڳ�ʼ�� 
	ADC_Configuration();             //ADC��ʼ�� 
  time_ini();

}
void time_ini(void){//pwm��ʼ��
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;						//PB5����ΪTIM3��ͨ��2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /*TIM3�ֲ����ù��ܿ���	 ��TIM3�ľֲ����ÿ���ʱ��PB5�ᱻ����ΪTIM3_CH2*/
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);			 
  				
   /*-------------------------------------------------------------------
  TIM3CLK=72MHz  Ԥ��Ƶϵ��Prescaler=2 ������Ƶ ��ʱ��ʱ��Ϊ24MHz
  ���ݹ�ʽ ͨ�����ռ�ձ�=TIM3_CCR2/(TIM_Period+1),���Եõ�TIM_Pulse�ļ���ֵ	 
  ����/�ȽϼĴ���2 TIM3_CCR2= CCR2_Val 	     
  -------------------------------------------------------------------*/
  TIM3_TimeBaseStructure.TIM_Prescaler = 2;						    //Ԥ��Ƶ��TIM3_PSC=3	 
  TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���������ϼ���ģʽ TIM3_CR1[4]=0
  TIM3_TimeBaseStructure.TIM_Period =24000;				            //�Զ���װ�ؼĴ���TIM3_APR  ȷ��Ƶ��Ϊ1KHz 		     
  TIM3_TimeBaseStructure.TIM_ClockDivision = 0x0;					//ʱ�ӷ�Ƶ���� TIM3_CR1[9:8]=00
  TIM3_TimeBaseStructure.TIM_RepetitionCounter = 0x0;

  TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);					//дTIM3���Ĵ�������
  
  TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 			    //PWMģʽ2 TIM3_CCMR1[14:12]=111 �����ϼ���ʱ��
  																    //һ��TIMx_CNT<TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ������Ϊ��Ч��ƽ
  TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //����/����2�������  OC2�ź��������Ӧ���������PB5
  TIM3_OCInitStructure.TIM_Pulse = 1000; 					    //ȷ��ռ�ձȣ����ֵ��������Ч��ƽ��ʱ�䡣
  TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 	    //�������  �͵�ƽ��Ч TIM3_CCER[5]=1;
         
  TIM_OC2Init(TIM3, &TIM3_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_Cmd(TIM3,ENABLE);											//������ʱ��3 TIM3_CR1[0]=1; 
}
/****************************************************************************
* ��    �ƣ�void NVIC_Configuration(void)
* ��    �ܣ��ж�Դ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;		 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     	//���ô���1�ж�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�����ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
}
/****************************************************************************
* ��    �ƣ�void  OS_CPU_SysTickInit(void)
* ��    �ܣ�ucos ϵͳ����ʱ�ӳ�ʼ��  ��ʼ����Ϊ10msһ�ν���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void  OS_CPU_SysTickInit(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    INT32U         cnts;
    RCC_GetClocksFreq(&rcc_clocks);		                        //���ϵͳʱ�ӵ�ֵ	 
    cnts = (INT32U)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;	//���ʱ�ӽ��ĵ�ֵ	
	SysTick_Config(cnts);										//����ʱ�ӽ���	     
}

/****************************************************************************
* ��    �ƣ�void tp_Config(void)
* ��    �ܣ�TFT ���������Ƴ�ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void tp_Config(void) 
{ 
  GPIO_InitTypeDef  GPIO_InitStructure; 
  SPI_InitTypeDef   SPI_InitStructure; 

  /* SPI1 ʱ��ʹ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); 
 
  /* SPI1 SCK(PA5)��MISO(PA6)��MOSI(PA7) ���� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�����ٶ�50MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //����ģʽ
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	

  /* SPI1 ����оƬ��Ƭѡ�������� PB7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�����ٶ�50MHZ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//�������ģʽ
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  
  /* ����SPI1�����Ϲҽ���4�����裬������ʹ�ô�����ʱ����Ҫ��ֹ����3��SPI1 ���裬 ������������ */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           		//SPI1 SST25VF016BƬѡ 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                //SPI1 VS1003Ƭѡ 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //SPI1 ����ģ��Ƭѡ
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOC, GPIO_Pin_4);							//SPI1 SST25VF016BƬѡ�ø� 
  GPIO_SetBits(GPIOB, GPIO_Pin_12);							//SPI1 VS1003Ƭѡ�ø�  
  GPIO_SetBits(GPIOA, GPIO_Pin_4);							//SPI1 ����ģ��Ƭѡ�ø� 
  
   /* SPI1���� ���� */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��  
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //��ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8λ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						   //ʱ�Ӽ��� ����״̬ʱ��SCK���ֵ͵�ƽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //ʱ����λ ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //�������NSS
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;  //�����ʿ��� SYSCLK/64
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݸ�λ��ǰ
  SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC����ʽ�Ĵ�����ʼֵΪ7 
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* SPI1 ʹ�� */  
  SPI_Cmd(SPI1,ENABLE);  
}

/****************************************************************************
* ��    �ƣ�unsigned char SPI_WriteByte(unsigned char data) 
* ��    �ܣ�SPI1 д����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
unsigned char SPI_WriteByte(unsigned char data) 
{ 
 unsigned char Data = 0; 

  //�ȴ����ͻ�������
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET); 
  // ����һ���ֽ�  
  SPI_I2S_SendData(SPI1,data); 

   //�ȴ��Ƿ���յ�һ���ֽ� 
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET); 
  // ��ø��ֽ�
  Data = SPI_I2S_ReceiveData(SPI1); 

  // �����յ����ֽ� 
  return Data; 
}  

/****************************************************************************
* ��    �ƣ�void SpiDelay(unsigned int DelayCnt) 
* ��    �ܣ�SPI1 д��ʱ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
void SpiDelay(unsigned int DelayCnt)
{
 unsigned int i;
 for(i=0;i<DelayCnt;i++);
}

/****************************************************************************
* ��    �ƣ�u16 TPReadX(void) 
* ��    �ܣ�������X�����ݶ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
u16 TPReadX(void)
{ 
   u16 x=0;
   TP_CS();	                        //ѡ��XPT2046 
   SpiDelay(10);					//��ʱ
   SPI_WriteByte(0x90);				//����X���ȡ��־
   SpiDelay(10);					//��ʱ
   x=SPI_WriteByte(0x00);			//������ȡ16λ������ 
   x<<=8;
   x+=SPI_WriteByte(0x00);
   SpiDelay(10);					//��ֹXPT2046
   TP_DCS(); 					    								  
   x = x>>3;						//��λ�����12λ����Ч����0-4095
   return (x);
}
/****************************************************************************
* ��    �ƣ�u16 TPReadY(void)
* ��    �ܣ�������Y�����ݶ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/
u16 TPReadY(void)
{
   u16 y=0;
   TP_CS();	                        //ѡ��XPT2046 
   SpiDelay(10);					//��ʱ
   SPI_WriteByte(0xd0);				//����Y���ȡ��־
   SpiDelay(10);					//��ʱ
   y=SPI_WriteByte(0x00);			//������ȡ16λ������ 
   y<<=8;
   y+=SPI_WriteByte(0x00);
   SpiDelay(10);					//��ֹXPT2046
   TP_DCS(); 					    								  
   y = y>>3;						//��λ�����12λ����Ч����0-4095
   return (y);
}

/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/

