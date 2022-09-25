/****************************************************************************
* Copyright (C), 2013 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3,V5,MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: fun.c
* 内容简述:
        STM32外设资源用到SPI，GPIO，FSMC，USART
*       本例程操作系统采用ucos2.86a版本， 建立了7个任务
			任务名											 优先级
			APP_TASK_START_PRIO                               2	        主任务
			APP_TASK_USER_IF_PRIO                             13		ucgui界面任务
            APP_TASK_KBD_PRIO                                 12		触摸屏任务
            Task_Com1_PRIO                                    4			COM1通信任务
            Task_Led1_PRIO                                    7			LED1 闪烁任务
            Task_Led2_PRIO                                    8			LED2 闪烁任务
            Task_Led3_PRIO                                    9			LED3 闪烁任务
		 当然还包含了系统任务：
		    OS_TaskIdle                  空闲任务-----------------优先级最低
			OS_TaskStat                  统计运行时间的任务-------优先级次低
*		本例程图形用户接口基于ucgui3.90a,其中的MULTIEDIT窗口部件采用了ucgui3.98的窗口部件，
*       用到的ucgui窗口部件包括
             FRAMEWIN   
*            TEXT
*            SLIDER
*            EDIT
*            FRAMEWIN
*            MULTIEDIT
*            本例程实现的功能有：
*		（1）通过界面上的滑动条控制板子上的LED1-LED3的闪烁间隔时间，范围为50-2000ms
*		（2）通过串口可以控制板子上的LED1-LED3的闪烁间隔时间，范围为50-2000ms。
*	辅助软件：PC机上需要运行串口调试助手软件。速率设置为115200，N,8,1，指令格式：
                LED1闪烁间隔：1ms--65535ms  指令L1 1F--L1 65535F
				LED2闪烁间隔：1ms--65535ms  指令L2 1F--L1 65535F 
				LED3闪烁间隔：1ms--65535ms  指令L3 1F--L1 65535F 
				注意： LED2和LED3只有V3及V5具有

*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-7-05 sun68  创建该文件
* v0.3    2013-5-11 sun68  修正了编译警告  制作此版本的例程视频讲解。
*/