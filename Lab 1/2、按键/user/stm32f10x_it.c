/**
  ******************************************************************************
  * @file EXTI/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and 
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"	 
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

extern unsigned char _it0,_it1,_it2,_it3;

/** @addtogroup StdPeriph_Examples
  * @{
  */

/** @addtogroup EXTI_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
}

/****************************************************************************
* ��    �ƣ�void EXTI9_5_IRQHandler(void)
* ��    �ܣ�EXTI9-5�жϴ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)		//�б��Ƿ��м�����
  {	
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0){	 //K1����  
	   __IO uint32_t nCount = 0xfff;			
		 while(nCount != 0)  nCount--;										 //��ʱȥ����ǰ�ض���
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0){	 //�жϼ��Ƿ��԰���
		 while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0);//�Ƿ��ɿ�����	 
		 _it0 = 1;	    										  						 //�����жϱ�־
		}
	  }		
    EXTI_ClearITPendingBit(EXTI_Line5);					  //����ж������־
  }
}

/****************************************************************************
* ��    �ƣ�void EXTI2_IRQHandler(void)
* ��    �ܣ�EXTI2�жϴ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)				  //�б��Ƿ��м�����
  {
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0){		//K2����  
	   __IO uint32_t nCount = 0xfff;			
		 while(nCount != 0)  nCount--;//�ݼ���ʱ			 //��ʱȥ����ǰ�ض���
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0){		  //�жϼ��Ƿ��԰���
		 while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0);//�Ƿ��ɿ�����	 
		 _it0 = 2;	    										  //�����жϱ�־
		}
	  }
    EXTI_ClearITPendingBit(EXTI_Line2);					  //����ж������־
  }
}

/****************************************************************************
* ��    �ƣ�void EXTI3_IRQHandler(void)
* ��    �ܣ�EXTI3�жϴ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void EXTI3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)				  //�б��Ƿ��м�����
  {
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0){		//K2����  
	   __IO uint32_t nCount = 0xfff;			
		 while(nCount != 0)  nCount--;//�ݼ���ʱ			 //��ʱȥ����ǰ�ض���
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0){		  //�жϼ��Ƿ��԰���
		 while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0);//�Ƿ��ɿ�����	 
		 _it0 = 3;	    										  //�����жϱ�־
		}
	  }
    EXTI_ClearITPendingBit(EXTI_Line3);					   //����ж������־
  }
}
 
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
