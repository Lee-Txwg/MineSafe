/*****************************************************************************
  �ļ�: main.c
  ����: Zhengyu https://gzwelink.taobao.com
  �汾: V1.0.0
  ʱ��: 20200401
	ƽ̨:MINI-STM32F103C8T6

*******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "gps.h"  

extern void GpsDataRead(void);//��ȡGPS���ݲ���ʾ

//USART2�ܽų�ʼ��
void GPIO_Config_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure USART1 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure USART1 Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void RCC_Configuration(void)
{
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  /* Enable USART1 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  
}

void USART_Config_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	
  USART_InitStructure.USART_BaudRate = 38400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  /* Enable USARTy Receive  interrupts */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);//ʹ�ܿ����ж�
  /* Enable the USART2 */
  USART_Cmd(USART2, ENABLE);
		 delay_ms(10);//�ȴ�10ms

}

void USART_BaudRate_Init(uint32_t Data)
{
	USART_InitTypeDef USART_InitStructure;
	
  USART_InitStructure.USART_BaudRate = Data;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  /* Enable USARTy Receive  interrupts */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);//ʹ�ܿ����ж�
  /* Enable the USART2 */
  USART_Cmd(USART2, ENABLE);
	delay_ms(10);//�ȴ�10ms
	
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

//�������������ⲿ8M����72Mϵͳ��Ƶ��������void SetSysClock(void)������ѡ����Ƶ������
int main(void)
 {	
		uint8_t key=0XFF;

	delay_init();	    	//��ʱ������ʼ��,ͨ��Systick�ж�ʵ��1ms��ʱ���� 
	RCC_Configuration();//USART2ʱ��ʹ�ܣ�GPIOʱ��ʹ��
	NVIC_Configuration();//�жϽ���ʹ��
	GPIO_Config_Init();	//��ʼ������2,PA2->USART2_TX��PA3->USART2_RX
	USART_Config_Init();//USART2���ã�38400�����ʣ�8λ���ݣ���У�飬1λֹͣ
	OLED_Init();//OLED��ʼ��  
	OLED_Clear();//����
	OLED_ShowCHinese(18,0,0);//��
	OLED_ShowCHinese(36,0,1);//��
	OLED_ShowCHinese(54,0,2);//��
	OLED_ShowCHinese(72,0,3);//��
	OLED_ShowCHinese(90,0,4);//��
	 delay_ms(500);//�ȴ�500ms
	if(Ublox_Cfg_Rate(1000,1)!=0)	//���ö�λ��Ϣ�����ٶ�Ϊ1000ms,˳���ж�GPSģ���Ƿ���λ. 
	{
  		OLED_ShowCHinese(18,3,10);//ģ
		OLED_ShowCHinese(36,3,11);//��
		OLED_ShowCHinese(54,3,12);//ͬ
		OLED_ShowCHinese(72,3,13);//��
		OLED_ShowCHinese(90,3,14);//��
		while((Ublox_Cfg_Rate(1000,1)!=0)&& key)	//�����ж�,ֱ�����Լ�鵽NEO-6M,�����ݱ���ɹ������ʱ�����ϵ�ʹ�ã�����9600����������������
		{
			USART_BaudRate_Init(9600);//��ʼ������2������Ϊ9600(EEPROMû�б������ݵ�ʱ��,������Ϊ9600.)

	  	Ublox_Cfg_Prt(38400);			//��������ģ��Ĳ�����Ϊ38400
			Ublox_Cfg_Tp(1000000,100000,1);	//����PPSΪ1�������1��,������Ϊ100ms	    
			key=Ublox_Cfg_Cfg_Save();		//��������  
		}	  					 
		delay_ms(500);//�ȴ�500ms
	
	}
  while (1)
  {
		OLED_Clear();//����
		OLED_ShowCHinese(18,0,0);//��
		OLED_ShowCHinese(36,0,1);//��
		OLED_ShowCHinese(54,0,2);//��
		OLED_ShowCHinese(72,0,3);//��
		OLED_ShowCHinese(90,0,4);//��
		GpsDataRead();//��ȡ��γ�ȣ�ʱ����Ϣ������Һ������ʾ
		delay_ms(800);//�ȴ�800ms
  }
 }

