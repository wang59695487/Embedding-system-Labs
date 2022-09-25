
#include "..\App\includes.h"
#include "demo.h"


#define ADC1_DR_Address    ((u32)0x4001244C)  	        //ADC1外设地址
/* 定义了触摸芯片的SPI片选控制 */
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
* 名    称：void RCC_Configuration(void)
* 功    能：系统时钟配置为72MHZ， 外设时钟配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
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
* 名    称：void GPIO_Configuration(void)
* 功    能：通用IO口配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /*对控制LED指示灯的IO口进行了初始化，将端口配置为推挽上拉输出，口线速度为50Mhz。PA9,PA10端口复用为串口1的TX，RX。
  在配置某个口线时，首先应对它所在的端口的时钟进行使能。否则无法配置成功，由于用到了端口B，D,E， 因此要对这几个端口的时钟
  进行使能，同时由于用到复用IO口功能用于配置串口。因此还要使能AFIO（复用功能IO）时钟。*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE); 	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				                 //LED1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_6|GPIO_Pin_3;		 //BackLight, LED2, LED3
  GPIO_Init(GPIOD, &GPIO_InitStructure);

}


void ADC_Configuration(void)//ADC初始化
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

    /* Enable DMA clock */
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

   /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

  	/* DMA channel1 configuration ----------------------------------------------*/
	//使能DMA
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;			            //DMA通道1的地址 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	            //DMA传送地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					            //传送方向
	DMA_InitStructure.DMA_BufferSize = 100;								            //传送内存大小，100个16位
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				            //传送内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//ADC1转换的数据是16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;				//传送的目的地址是16位宽度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;									//循环
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
	/* 允许DMA1通道1传输结束中断 */
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);


	//使能DMA通道1
	DMA_Cmd(DMA1_Channel1, ENABLE); 
  
  
  //设置AD模拟输入端口为输入 一共2路AD 规则通道
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//ADC配置
	/* ADC转换时间： ─ STM32F103xx增强型产品：时钟为56MHz时为1μs(时钟为72MHz为1.17μs)
	ADC采样范围0-3.3V    */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                   //设置ADC的时钟为72MHZ/6=12M 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC1工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		//模数转换工作在扫描模式（多通道）还是单次（单通道）模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在扫描模式（多通道）还是单次（单通道）模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;//规定了顺序进行规则转换的ADC通道的数目。这个数目的取值范围是1到16
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channels configuration [规则模式通道配置]*/ 

	//ADC1 规则通道配置
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);	  //通道10采样时间 55.5周期
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);	  //通道11采样时间 55.5周期
  	

	//使能ADC1 DMA 
	ADC_DMACmd(ADC1, ENABLE);
	//使能ADC1
	ADC_Cmd(ADC1, ENABLE);	
	
	// 初始化ADC1校准寄存器
	ADC_ResetCalibration(ADC1);
	//检测ADC1校准寄存器初始化是否完成
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	//开始校准ADC1
	ADC_StartCalibration(ADC1);
	//检测是否完成校准
	while(ADC_GetCalibrationStatus(ADC1));
	
	//ADC1转换启动
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	 
}
/****************************************************************************
* 名    称：void USART_Config(USART_TypeDef* USARTx,u32 baud)
* 功    能：串口配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void USART_Config(USART_TypeDef* USARTx,u32 baud){
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  //usart_init----------------------------------------------------
  /* 配置 USART1 Rx (PA.10) 为浮空输入 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* 配置 USART1 Tx (PA.09) 为复用功能 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	


  USART_InitStructure.USART_BaudRate =baud;						                    //速率115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		                //数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			                //停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;				                //无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

  USART_Init(USARTx, &USART_InitStructure);							                //配置串口参数函数
   USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);                    //使能接收中断
  /* 使能串口1 */
  USART_Cmd(USARTx, ENABLE);	
  
}

/****************************************************************************
* 名    称：void BSP_Init(void)
* 功    能：奋斗板初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void BSP_Init(void)
{
  																		   
  RCC_Configuration();  	       //系统时钟初始化及端口外设时钟使能	  
  NVIC_Configuration();			   //中断源配置
  GPIO_Configuration();			   //状态LED1的初始化  	  
  USART_Config(USART1,115200);	   //串口1初始化
  tp_Config();					   //SPI1 触摸电路初始化	    
  FSMC_LCD_Init();				   //FSMC TFT接口初始化 
	ADC_Configuration();             //ADC初始化 
  time_ini();

}
void time_ini(void){//pwm初始化
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;						//PB5复用为TIM3的通道2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /*TIM3局部复用功能开启	 在TIM3的局部复用开启时，PB5会被复用为TIM3_CH2*/
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);			 
  				
   /*-------------------------------------------------------------------
  TIM3CLK=72MHz  预分频系数Prescaler=2 经过分频 定时器时钟为24MHz
  根据公式 通道输出占空比=TIM3_CCR2/(TIM_Period+1),可以得到TIM_Pulse的计数值	 
  捕获/比较寄存器2 TIM3_CCR2= CCR2_Val 	     
  -------------------------------------------------------------------*/
  TIM3_TimeBaseStructure.TIM_Prescaler = 2;						    //预分频器TIM3_PSC=3	 
  TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//计数器向上计数模式 TIM3_CR1[4]=0
  TIM3_TimeBaseStructure.TIM_Period =24000;				            //自动重装载寄存器TIM3_APR  确定频率为1KHz 		     
  TIM3_TimeBaseStructure.TIM_ClockDivision = 0x0;					//时钟分频因子 TIM3_CR1[9:8]=00
  TIM3_TimeBaseStructure.TIM_RepetitionCounter = 0x0;

  TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);					//写TIM3各寄存器参数
  
  TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 			    //PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，
  																    //一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平
  TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //输入/捕获2输出允许  OC2信号输出到对应的输出引脚PB5
  TIM3_OCInitStructure.TIM_Pulse = 1000; 					    //确定占空比，这个值决定了有效电平的时间。
  TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 	    //输出极性  低电平有效 TIM3_CCER[5]=1;
         
  TIM_OC2Init(TIM3, &TIM3_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_Cmd(TIM3,ENABLE);											//启动定时器3 TIM3_CR1[0]=1; 
}
/****************************************************************************
* 名    称：void NVIC_Configuration(void)
* 功    能：中断源配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;		 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     	//设置串口1中断
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//子优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
}
/****************************************************************************
* 名    称：void  OS_CPU_SysTickInit(void)
* 功    能：ucos 系统节拍时钟初始化  初始设置为10ms一次节拍
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void  OS_CPU_SysTickInit(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    INT32U         cnts;
    RCC_GetClocksFreq(&rcc_clocks);		                        //获得系统时钟的值	 
    cnts = (INT32U)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;	//算出时钟节拍的值	
	SysTick_Config(cnts);										//设置时钟节拍	     
}

/****************************************************************************
* 名    称：void tp_Config(void)
* 功    能：TFT 触摸屏控制初始化
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void tp_Config(void) 
{ 
  GPIO_InitTypeDef  GPIO_InitStructure; 
  SPI_InitTypeDef   SPI_InitStructure; 

  /* SPI1 时钟使能 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); 
 
  /* SPI1 SCK(PA5)、MISO(PA6)、MOSI(PA7) 设置 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线速度50MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //复用模式
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	

  /* SPI1 触摸芯片的片选控制设置 PB7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线速度50MHZ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//推挽输出模式
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  
  /* 由于SPI1总线上挂接了4个外设，所以在使用触摸屏时，需要禁止其余3个SPI1 外设， 才能正常工作 */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           		//SPI1 SST25VF016B片选 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                //SPI1 VS1003片选 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //SPI1 网络模块片选
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOC, GPIO_Pin_4);							//SPI1 SST25VF016B片选置高 
  GPIO_SetBits(GPIOB, GPIO_Pin_12);							//SPI1 VS1003片选置高  
  GPIO_SetBits(GPIOA, GPIO_Pin_4);							//SPI1 网络模块片选置高 
  
   /* SPI1总线 配置 */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工  
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //主模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8位
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						   //时钟极性 空闲状态时，SCK保持低电平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //时钟相位 数据采样从第一个时钟边沿开始
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //软件产生NSS
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;  //波特率控制 SYSCLK/64
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据高位在前
  SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC多项式寄存器初始值为7 
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* SPI1 使能 */  
  SPI_Cmd(SPI1,ENABLE);  
}

/****************************************************************************
* 名    称：unsigned char SPI_WriteByte(unsigned char data) 
* 功    能：SPI1 写函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
unsigned char SPI_WriteByte(unsigned char data) 
{ 
 unsigned char Data = 0; 

  //等待发送缓冲区空
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET); 
  // 发送一个字节  
  SPI_I2S_SendData(SPI1,data); 

   //等待是否接收到一个字节 
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET); 
  // 获得该字节
  Data = SPI_I2S_ReceiveData(SPI1); 

  // 返回收到的字节 
  return Data; 
}  

/****************************************************************************
* 名    称：void SpiDelay(unsigned int DelayCnt) 
* 功    能：SPI1 写延时函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
void SpiDelay(unsigned int DelayCnt)
{
 unsigned int i;
 for(i=0;i<DelayCnt;i++);
}

/****************************************************************************
* 名    称：u16 TPReadX(void) 
* 功    能：触摸屏X轴数据读出
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
u16 TPReadX(void)
{ 
   u16 x=0;
   TP_CS();	                        //选择XPT2046 
   SpiDelay(10);					//延时
   SPI_WriteByte(0x90);				//设置X轴读取标志
   SpiDelay(10);					//延时
   x=SPI_WriteByte(0x00);			//连续读取16位的数据 
   x<<=8;
   x+=SPI_WriteByte(0x00);
   SpiDelay(10);					//禁止XPT2046
   TP_DCS(); 					    								  
   x = x>>3;						//移位换算成12位的有效数据0-4095
   return (x);
}
/****************************************************************************
* 名    称：u16 TPReadY(void)
* 功    能：触摸屏Y轴数据读出
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/
u16 TPReadY(void)
{
   u16 y=0;
   TP_CS();	                        //选择XPT2046 
   SpiDelay(10);					//延时
   SPI_WriteByte(0xd0);				//设置Y轴读取标志
   SpiDelay(10);					//延时
   y=SPI_WriteByte(0x00);			//连续读取16位的数据 
   y<<=8;
   y+=SPI_WriteByte(0x00);
   SpiDelay(10);					//禁止XPT2046
   TP_DCS(); 					    								  
   y = y>>3;						//移位换算成12位的有效数据0-4095
   return (y);
}

/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/

