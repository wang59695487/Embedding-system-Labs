
#define GLOBALS
#include "demo.h"

extern void Fun(void);		

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];
static  OS_STK AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE];
static  OS_STK AppTaskKbdStk[APP_TASK_KBD_STK_SIZE];
static  OS_STK Task_Com1Stk[Task_Com1_STK_SIZE]; 
static  OS_STK Task_Led1Stk[Task_Led1_STK_SIZE];
static  OS_STK Task_AdcStk[Task_Adc_STK_SIZE];
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void Task_ADC(void* p_arg);
static  void App_TaskCreate(void);
static  void App_TaskStart(void* p_arg);
static  void  AppTaskUserIF (void *p_arg);
static  void AppTaskKbd(void* p_arg);
static  void Task_Com1(void* p_arg);
static  void Task_Led1(void* p_arg);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
char *itoa(int value, char *string, int radix);
#pragma diag_suppress 870 
/****************************************************************************
* 名    称：int main(void)
* 功    能：主函数入口
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
int main(void)
{
   CPU_INT08U os_err; 
   /* 禁止所有中断 */
   CPU_IntDis();
	
   /* ucosII 初始化 */
   OSInit();                                                  

   /* 硬件平台初始化 */
   BSP_Init();             
                               
   //默认LED闪烁间隔500ms
   milsec1=500,milsec2=500,milsec3=500;			  
    ADC_TIMEOUT=0;		
   //建立主任务， 优先级最高  建立这个任务另外一个用途是为了以后使用统计任务
   os_err = OSTaskCreate((void (*) (void *)) App_TaskStart,               		    //指向任务代码的指针
                          (void *) 0,												//任务开始执行时，传递给任务的参数的指针
               		     (OS_STK *) &App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],	//分配给任务的堆栈的栈顶指针   从顶向下递减
                         (INT8U) APP_TASK_START_PRIO);								//分配给任务的优先级
             
   os_err=os_err;
   OSTimeSet(0);			 
   OSStart();               
   return (0);
}




/****************************************************************************
* 名    称：static  void App_TaskStart(void* p_arg)
* 功    能：开始任务建立
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static  void App_TaskStart(void* p_arg)
{
 
  (void) p_arg;
   //初始化ucosII时钟节拍
   OS_CPU_SysTickInit();
                               
   //使能ucos 的统计任务
#if (OS_TASK_STAT_EN > 0)
   
   OSStatInit();                //----统计任务初始化函数                                 
#endif

   App_TaskCreate();			//建立其他的任务

   while (1)
   {		      	  
	  OSTimeDlyHMSM(0, 0, 1, 100);
   }
}

/****************************************************************************
* 名    称：static  void App_TaskCreate(void)
* 功    能：建立其余任务的函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static  void App_TaskCreate(void)
{
   ADC_SEM=OSSemCreate(10);		                 //建立ADC采样信号量      
   Com1_MBOX=OSMboxCreate((void *) 0);		     //建立串口1中断的消息邮箱邮箱
   /*  建立用户界面任务 */
   OSTaskCreateExt(AppTaskUserIF,											   //指向任务代码的指针
   					(void *)0,												   //任务开始执行时，传递给任务的参数的指针
   					(OS_STK *)&AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE-1],  //分配给任务的堆栈的栈顶指针   从顶向下递减
					APP_TASK_USER_IF_PRIO,									   //分配给任务的优先级
					APP_TASK_USER_IF_PRIO,									   //预备给以后版本的特殊标识符，在现行版本同任务优先级
					(OS_STK *)&AppTaskUserIFStk[0],							   //指向任务堆栈栈底的指针，用于堆栈的检验
                    APP_TASK_USER_IF_STK_SIZE,									//指定堆栈的容量，用于堆栈的检验
                    (void *)0,													//指向用户附加的数据域的指针，用来扩展任务的任务控制块
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);					//选项，指定是否允许堆栈检验，是否将堆栈清0,任务是否要
					                                                            //进行浮点运算等等。
                    
   /*   建立触摸驱动任务 */
   OSTaskCreateExt(AppTaskKbd,
   					(void *)0,
					(OS_STK *)&AppTaskKbdStk[APP_TASK_KBD_STK_SIZE-1],
					APP_TASK_KBD_PRIO,
					APP_TASK_KBD_PRIO,
					(OS_STK *)&AppTaskKbdStk[0],
                    APP_TASK_KBD_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);	
   //串口1接收及发送任务---------------------------------------------------------    
   OSTaskCreateExt(Task_Com1,(void *)0,(OS_STK *)&Task_Com1Stk[Task_Com1_STK_SIZE-1],Task_Com1_PRIO,Task_Com1_PRIO,(OS_STK *)&Task_Com1Stk[0],
                    Task_Com1_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
   
   //LED1 闪烁任务------------------------------------------------------
   OSTaskCreateExt(Task_Led1,(void *)0,(OS_STK *)&Task_Led1Stk[Task_Led1_STK_SIZE-1],Task_Led1_PRIO,Task_Led1_PRIO,(OS_STK *)&Task_Led1Stk[0],
                    Task_Led1_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
   

	//ADC采样任务------------------------------------------------------
   OSTaskCreateExt(Task_ADC,
   					(void *)0,
					(OS_STK *)&Task_AdcStk[Task_Adc_STK_SIZE-1],
					Task_Adc_PRIO,
					Task_Adc_PRIO,
					(OS_STK *)&Task_AdcStk[0],
                    Task_Adc_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);

}

/****************************************************************************
* 名    称：static  void  AppTaskUserIF (void *p_arg)
* 功    能：用户界面任务
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static  void  AppTaskUserIF (void *p_arg)
{											   
 (void)p_arg;								    
  GUI_Init();	                //ucgui初始化 
  while(1) 
  {	
     Fun(); 					//界面主程序
  }
}
/****************************************************************************
* 名    称：static  void  AppTaskKbd (void *p_arg)
* 功    能：触摸屏坐标获取任务
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static  void  AppTaskKbd (void *p_arg)
{
  (void)p_arg;   
   while(1) 
   { 
   	
      OSTimeDlyHMSM(0,0,0,10); 	               
	  GUI_TOUCH_Exec();    
   }
}
/****************************************************************************
* 名    称：static  void Task_Led1(void* p_arg)
* 功    能：LED1闪烁任务
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static  void Task_Led1(void* p_arg)
{
   TIM_OCInitTypeDef  TIM3_OCInitStructure;


   (void) p_arg;	    
   while (1)
   {
      OSTimeDlyHMSM(0, 0, 0, 100);
			TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					  //PWM模式2 
  		TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 		  //输出禁止        
  		TIM3_OCInitStructure.TIM_Pulse = milsec1*240; 							  
  		TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
  		TIM_OC2Init(TIM3, &TIM3_OCInitStructure);
			
   }
}



static  void Task_ADC(void *p_arg){    //ADC任务
   INT8U err;
   
   (void)p_arg;	  
   while(1){   
   	OSSemPend(ADC_SEM,0,&err); 		               		  //等待ADC信号量	
	itoa((ADC_ConvertedValue[40]+ADC_ConvertedValue[42])/2, ADC_STR1,10);	          //ADC1通道10的数值转为字符串 
    USART_OUT(USART1, "\r\nADC1 通道10：%s",ADC_STR1);	  //串口1输出通道10的数值  
	USART_OUT(USART1, "\r\nADC1 通道10：%d",(ADC_ConvertedValue[0]-1250)/1650*4200);	  //串口1输出通道10的数值 	 
	USART_OUT(USART1, "\r\n");
   } 
}

/****************************************************************************
* 名    称：static  void Task_Com1(void *p_arg)
* 功    能：串口1任务
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static  void Task_Com1(void *p_arg){    
   INT8U err;	
   unsigned char * msg;
   (void)p_arg;	  
   while(1){         
	  msg=(unsigned char *)OSMboxPend(Com1_MBOX,0,&err); 		  //等待串口接收消息邮箱  	 
   	  if(msg[0]=='L'&&msg[1]==0x31){
	  	 milsec1=atoi((const char *)&msg[3]);                     //LED1 的延时毫秒  (mini and V3,V5)

		 USART_OUT(USART1,"\r\n");
	  	 USART_OUT(USART1,"LED1: %d ms 间隔闪烁",milsec1);		 
	  }
	  else if(msg[0]=='L'&&msg[1]==0x32){
	  	 milsec2=atoi((const char *)&msg[3]);		                              //LED2 的延时毫秒  (only V3,V5)
		 USART_OUT(USART1,"\r\n");
	  	 USART_OUT(USART1,"LED2: %d ms 间隔闪烁",milsec2);
	  }
	  else if(msg[0]=='L'&&msg[1]==0x33){
	  	  milsec3=atoi((const char *)&msg[3]);		                          //LED3 的延时毫秒  (only V3,V5)
		  USART_OUT(USART1,"\r\n");
	  	  USART_OUT(USART1,"LED3: %d ms 间隔闪烁",milsec3);
	  } 
   } 
}
/****************************************************************************
* 名    称：void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* 功    能：格式化串口输出函数
* 入口参数：USARTx:  指定串口
			Data：   发送数组
			...:     不定参数
* 出口参数：无
* 说    明：格式化串口输出函数
        	"\r"	回车符	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	换行符	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	字符串	   USART_OUT(USART1, "字符串是：%s","abcdefg")
			"%d"	十进制	   USART_OUT(USART1, "a=%d",10)
* 调用方法：无 
****************************************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 

	const char *s;
    int d;
   
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //判断是否到达字符串结束符
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data++;
					break;
				
				default:
					Data++;
				    break;
			}
			
			 
		}
		else if(*Data=='%'){									  //
			switch (*++Data){				
				case 's':										  //字符串
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //十进制
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}
/******************************************************
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
		buf="-379"							   			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 1;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

#if (OS_APP_HOOKS_EN > 0)
/*
*********************************************************************************************************
*                                      TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument : ptcb   is a pointer to the task control block of the task being created.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskCreateHook(OS_TCB* ptcb)
{
}

/*
*********************************************************************************************************
*                                    TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskDelHook(OS_TCB* ptcb)
{
   (void) ptcb;
}

/*
*********************************************************************************************************
*                                      IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument : none.
*
* Note     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void App_TaskIdleHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                        STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument : none.
*********************************************************************************************************
*/

void App_TaskStatHook(void)
{
}

/*
*********************************************************************************************************
*                                        TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument : none.
*
* Note     : 1 Interrupts are disabled during this call.
*
*            2  It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void App_TaskSwHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                     OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument : ptcb    is a pointer to the TCB of the task being created.
*
* Note     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void App_TCBInitHook(OS_TCB* ptcb)
{
   (void) ptcb;
}
#endif

#endif
/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/
