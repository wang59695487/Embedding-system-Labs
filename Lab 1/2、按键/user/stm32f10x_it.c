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
* 名    称：void EXTI9_5_IRQHandler(void)
* 功    能：EXTI9-5中断处理程序
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)		//判别是否有键按下
  {	
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0){	 //K1按下  
	   __IO uint32_t nCount = 0xfff;			
		 while(nCount != 0)  nCount--;										 //延时去按键前沿抖动
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0){	 //判断键是否仍按下
		 while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0);//是否松开按键	 
		 _it0 = 1;	    										  						 //按键中断标志
		}
	  }		
    EXTI_ClearITPendingBit(EXTI_Line5);					  //清除中断请求标志
  }
}

/****************************************************************************
* 名    称：void EXTI2_IRQHandler(void)
* 功    能：EXTI2中断处理程序
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)				  //判别是否有键按下
  {
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0){		//K2按下  
	   __IO uint32_t nCount = 0xfff;			
		 while(nCount != 0)  nCount--;//递减延时			 //延时去按键前沿抖动
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0){		  //判断键是否仍按下
		 while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0);//是否松开按键	 
		 _it0 = 2;	    										  //按键中断标志
		}
	  }
    EXTI_ClearITPendingBit(EXTI_Line2);					  //清除中断请求标志
  }
}

/****************************************************************************
* 名    称：void EXTI3_IRQHandler(void)
* 功    能：EXTI3中断处理程序
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void EXTI3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)				  //判别是否有键按下
  {
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0){		//K2按下  
	   __IO uint32_t nCount = 0xfff;			
		 while(nCount != 0)  nCount--;//递减延时			 //延时去按键前沿抖动
		 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0){		  //判断键是否仍按下
		 while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0);//是否松开按键	 
		 _it0 = 3;	    										  //按键中断标志
		}
	  }
    EXTI_ClearITPendingBit(EXTI_Line3);					   //清除中断请求标志
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
