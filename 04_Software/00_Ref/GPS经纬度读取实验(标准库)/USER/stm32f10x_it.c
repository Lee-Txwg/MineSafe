/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "gps.h"
#include "string.h"
extern void TimingDelay_Decrement(void);//Systick ʱ�������1ms����һ��
extern uint8_t USART2_RX_BUF[USART_MAX_RECV_LEN]; 				    //���ջ���,���USART_MAX_RECV_LEN���ֽ�.

 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}


uint8_t rx_index;
uint8_t rec_over;
#define RXBUFF_SIZE	200

uint8_t aRxBuffer[RXBUFF_SIZE];//���ջ���
uint16_t RX_len;//�����ֽڼ���
void USART2_IRQHandler(void)
{
	uint8_t temp;
	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET){	   //��ֹ����ж�
		USART_ReceiveData(USART2); 
	}   

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){		//USART�����ж�
        temp = USART_ReceiveData(USART2);
		if(rx_index > RXBUFF_SIZE){		   //������ٽ�������
			return;
		}
		aRxBuffer[rx_index++] = temp;
	}
 if(USART_GetFlagStatus(USART2, USART_FLAG_IDLE) != RESET){		//USART�����ж�
		USART_ReceiveData(USART2);
		RX_len=rx_index;
	  memcpy(USART2_RX_BUF,aRxBuffer,RX_len);//�ѻ����������ݣ�������Ҫ����������
	  rx_index=0;
	}

 
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
