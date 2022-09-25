
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
* ��    �ƣ�int main(void)
* ��    �ܣ����������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
int main(void)
{
   CPU_INT08U os_err; 
   /* ��ֹ�����ж� */
   CPU_IntDis();
	
   /* ucosII ��ʼ�� */
   OSInit();                                                  

   /* Ӳ��ƽ̨��ʼ�� */
   BSP_Init();             
                               
   //Ĭ��LED��˸���500ms
   milsec1=500,milsec2=500,milsec3=500;			  
    ADC_TIMEOUT=0;		
   //���������� ���ȼ����  ���������������һ����;��Ϊ���Ժ�ʹ��ͳ������
   os_err = OSTaskCreate((void (*) (void *)) App_TaskStart,               		    //ָ����������ָ��
                          (void *) 0,												//����ʼִ��ʱ�����ݸ�����Ĳ�����ָ��
               		     (OS_STK *) &App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],	//���������Ķ�ջ��ջ��ָ��   �Ӷ����µݼ�
                         (INT8U) APP_TASK_START_PRIO);								//�������������ȼ�
             
   os_err=os_err;
   OSTimeSet(0);			 
   OSStart();               
   return (0);
}




/****************************************************************************
* ��    �ƣ�static  void App_TaskStart(void* p_arg)
* ��    �ܣ���ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
static  void App_TaskStart(void* p_arg)
{
 
  (void) p_arg;
   //��ʼ��ucosIIʱ�ӽ���
   OS_CPU_SysTickInit();
                               
   //ʹ��ucos ��ͳ������
#if (OS_TASK_STAT_EN > 0)
   
   OSStatInit();                //----ͳ�������ʼ������                                 
#endif

   App_TaskCreate();			//��������������

   while (1)
   {		      	  
	  OSTimeDlyHMSM(0, 0, 1, 100);
   }
}

/****************************************************************************
* ��    �ƣ�static  void App_TaskCreate(void)
* ��    �ܣ�������������ĺ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
static  void App_TaskCreate(void)
{
   ADC_SEM=OSSemCreate(10);		                 //����ADC�����ź���      
   Com1_MBOX=OSMboxCreate((void *) 0);		     //��������1�жϵ���Ϣ��������
   /*  �����û��������� */
   OSTaskCreateExt(AppTaskUserIF,											   //ָ����������ָ��
   					(void *)0,												   //����ʼִ��ʱ�����ݸ�����Ĳ�����ָ��
   					(OS_STK *)&AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE-1],  //���������Ķ�ջ��ջ��ָ��   �Ӷ����µݼ�
					APP_TASK_USER_IF_PRIO,									   //�������������ȼ�
					APP_TASK_USER_IF_PRIO,									   //Ԥ�����Ժ�汾�������ʶ���������а汾ͬ�������ȼ�
					(OS_STK *)&AppTaskUserIFStk[0],							   //ָ�������ջջ�׵�ָ�룬���ڶ�ջ�ļ���
                    APP_TASK_USER_IF_STK_SIZE,									//ָ����ջ�����������ڶ�ջ�ļ���
                    (void *)0,													//ָ���û����ӵ��������ָ�룬������չ�����������ƿ�
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);					//ѡ�ָ���Ƿ������ջ���飬�Ƿ񽫶�ջ��0,�����Ƿ�Ҫ
					                                                            //���и�������ȵȡ�
                    
   /*   ���������������� */
   OSTaskCreateExt(AppTaskKbd,
   					(void *)0,
					(OS_STK *)&AppTaskKbdStk[APP_TASK_KBD_STK_SIZE-1],
					APP_TASK_KBD_PRIO,
					APP_TASK_KBD_PRIO,
					(OS_STK *)&AppTaskKbdStk[0],
                    APP_TASK_KBD_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);	
   //����1���ռ���������---------------------------------------------------------    
   OSTaskCreateExt(Task_Com1,(void *)0,(OS_STK *)&Task_Com1Stk[Task_Com1_STK_SIZE-1],Task_Com1_PRIO,Task_Com1_PRIO,(OS_STK *)&Task_Com1Stk[0],
                    Task_Com1_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
   
   //LED1 ��˸����------------------------------------------------------
   OSTaskCreateExt(Task_Led1,(void *)0,(OS_STK *)&Task_Led1Stk[Task_Led1_STK_SIZE-1],Task_Led1_PRIO,Task_Led1_PRIO,(OS_STK *)&Task_Led1Stk[0],
                    Task_Led1_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
   

	//ADC��������------------------------------------------------------
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
* ��    �ƣ�static  void  AppTaskUserIF (void *p_arg)
* ��    �ܣ��û���������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
static  void  AppTaskUserIF (void *p_arg)
{											   
 (void)p_arg;								    
  GUI_Init();	                //ucgui��ʼ�� 
  while(1) 
  {	
     Fun(); 					//����������
  }
}
/****************************************************************************
* ��    �ƣ�static  void  AppTaskKbd (void *p_arg)
* ��    �ܣ������������ȡ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
* ��    �ƣ�static  void Task_Led1(void* p_arg)
* ��    �ܣ�LED1��˸����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
static  void Task_Led1(void* p_arg)
{
   TIM_OCInitTypeDef  TIM3_OCInitStructure;


   (void) p_arg;	    
   while (1)
   {
      OSTimeDlyHMSM(0, 0, 0, 100);
			TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					  //PWMģʽ2 
  		TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 		  //�����ֹ        
  		TIM3_OCInitStructure.TIM_Pulse = milsec1*240; 							  
  		TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
  		TIM_OC2Init(TIM3, &TIM3_OCInitStructure);
			
   }
}



static  void Task_ADC(void *p_arg){    //ADC����
   INT8U err;
   
   (void)p_arg;	  
   while(1){   
   	OSSemPend(ADC_SEM,0,&err); 		               		  //�ȴ�ADC�ź���	
	itoa((ADC_ConvertedValue[40]+ADC_ConvertedValue[42])/2, ADC_STR1,10);	          //ADC1ͨ��10����ֵתΪ�ַ��� 
    USART_OUT(USART1, "\r\nADC1 ͨ��10��%s",ADC_STR1);	  //����1���ͨ��10����ֵ  
	USART_OUT(USART1, "\r\nADC1 ͨ��10��%d",(ADC_ConvertedValue[0]-1250)/1650*4200);	  //����1���ͨ��10����ֵ 	 
	USART_OUT(USART1, "\r\n");
   } 
}

/****************************************************************************
* ��    �ƣ�static  void Task_Com1(void *p_arg)
* ��    �ܣ�����1����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
static  void Task_Com1(void *p_arg){    
   INT8U err;	
   unsigned char * msg;
   (void)p_arg;	  
   while(1){         
	  msg=(unsigned char *)OSMboxPend(Com1_MBOX,0,&err); 		  //�ȴ����ڽ�����Ϣ����  	 
   	  if(msg[0]=='L'&&msg[1]==0x31){
	  	 milsec1=atoi((const char *)&msg[3]);                     //LED1 ����ʱ����  (mini and V3,V5)

		 USART_OUT(USART1,"\r\n");
	  	 USART_OUT(USART1,"LED1: %d ms �����˸",milsec1);		 
	  }
	  else if(msg[0]=='L'&&msg[1]==0x32){
	  	 milsec2=atoi((const char *)&msg[3]);		                              //LED2 ����ʱ����  (only V3,V5)
		 USART_OUT(USART1,"\r\n");
	  	 USART_OUT(USART1,"LED2: %d ms �����˸",milsec2);
	  }
	  else if(msg[0]=='L'&&msg[1]==0x33){
	  	  milsec3=atoi((const char *)&msg[3]);		                          //LED3 ����ʱ����  (only V3,V5)
		  USART_OUT(USART1,"\r\n");
	  	  USART_OUT(USART1,"LED3: %d ms �����˸",milsec3);
	  } 
   } 
}
/****************************************************************************
* ��    �ƣ�void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* ��    �ܣ���ʽ�������������
* ��ڲ�����USARTx:  ָ������
			Data��   ��������
			...:     ��������
* ���ڲ�������
* ˵    ������ʽ�������������
        	"\r"	�س���	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	���з�	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	�ַ���	   USART_OUT(USART1, "�ַ����ǣ�%s","abcdefg")
			"%d"	ʮ����	   USART_OUT(USART1, "a=%d",10)
* ���÷������� 
****************************************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 

	const char *s;
    int d;
   
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //�ж��Ƿ񵽴��ַ���������
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //���з�
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
				case 's':										  //�ַ���
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //ʮ����
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
		��������ת�ַ�������
        char *itoa(int value, char *string, int radix)
		radix=10 ��ʾ��10����	��ʮ���ƣ�ת�����Ϊ0;  

	    ����d=-379;
		ִ��	itoa(d, buf, 10); ��
		
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
/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/
