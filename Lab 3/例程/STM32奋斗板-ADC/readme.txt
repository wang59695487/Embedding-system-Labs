/****************************************************************************
* Copyright (C), 2013 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,V2.1,V3,V5,MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: main.c
* 内容简述:	
*       
*	演示通过串口1显示ADC1的11通道的测量结果
*	辅助软件：PC机上需要运行串口调试助手软件。
    
	基于MDK版本：        3.8
	基于官方外设库版本： 3.5	
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-7-28 sun68  创建该文件
*
*/

1设计要求
利用ADC的第11通道对开发板输入的电压值作AD转换，采用连续转换模式，转换结果通过DMA通
道1读取。ADC转换的结果，每间隔1秒钟向串口发送一次。

2 硬件电路设计													
在开发板上通用I/O口PC.01与XS8的14脚相连，将PC.01映射到ADC第11通道，即可实现利用ADC_IN11
对输入电压作AD转换。

3软件程序设计
	根据设计任务要求，软件程序主要包括：
	(1)	配置GPIO口，将PC.01配置为ADC的第11采样通道；将配置GPIO中PA.09和PA.10根引脚为串口输入输出。
	(2)	设置ADC，将ADC_IN11设置为连续转换模式；
	(3)	配置DMA通道1用于ADC_IN14传输转换的结果；
	(4)	配置串口及相关发送功能；
	(5)	每隔1S向串口输出AD转换结果。

4 运行过程
(1)	使用Keil uVision3 通过JLINK仿真器连接开发板，使用串口线，连接实验板
上的UART1（XS5)和PC机的串口，打开实验例程目录下的STM32-FD-ADC.Uv2例程，编译链接工程；
(2)	在PC机上运行windows自带的超级终端串口通信程序（波特率115200、1位停止位、无校验位、无硬件流
控制）；或者使用其它串口通信程序；
(3)	点击MDK 的Debug菜单，点击Start/Stop Debug Session；
(4)	可以看到串口输出数值不断变化，正常显示结果如下所示。

usart1 print AD_value --------------------------
The current AD value = 2096 
The current AD value = 2096 
The current AD value = 2048 
The current AD value = 2087 
The current AD value = 2112 
The current AD value = 2112 
The current AD value = 2003 
The current AD value = 1998 
The current AD value = 1999 
The current AD value = 2092 
The current AD value = 2048 
The current AD value = 2051 
The current AD value = 2056 
The current AD value = 2048 
The current AD value = 2048 
The current AD value = 2096 
The current AD value = 2001 
....
....

