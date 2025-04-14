/**
 * @file         bsp_dht11.c
 * @brief        LED BSP (Board Support Package) source file
 * @details      This file implements the functions to control the LED, including
 *               initialization, turning on, turning off, and toggling the LED state.
 * @author       chan
 * @date         2025/2/18
 * @version      v1.0
 */

/* Includes -----------------------------------------------------------------*/
#include "bsp_dht11.h"

/* function prototypes ------------------------------------------------------*/
// static led_op_result_t led_init   (void);
// static led_op_result_t led_on     (void);
// static led_op_result_t led_off    (void);
// static led_op_result_t led_toggle (void);

/* Function implementation --------------------------------------------------*/
/* ���������� */
static DHT11_STATUS DHT11_DATA_RESET(void)
{
    DHT11_STATUS STA = DHT11_DAT_RESET;
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
    return STA;
}

/* ���������� */
static DHT11_STATUS DHT11_DATA_SET(void)
{
    DHT11_STATUS STA = DHT11_DAT_SET;
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
    return STA;
}
/* ��ȡ�����ߵ�״̬ */
static GPIO_PinState DHT11_DATA_STA(void)
{
    return HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin);
}

/* �������ģʽ */
static void DHT11_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ����Ϊ�������
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Pull = GPIO_PULLUP; // �����������裬��֤���߿���ʱ��Ϊ�ߵ�ƽ
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/* �ӻ�����ģʽ */
static void DHT11_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // ����Ϊ����ģʽ
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Pull = GPIO_PULLUP; // �����������裬��֤���߿���ʱ��Ϊ�ߵ�ƽ
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/* ��ʼ�ź� */
void DHT11_Send(void)
{
    DHT11_OUT();        // �л�Ϊ���ģʽ
    DHT11_DATA_RESET(); // ����������20������Ϊ��ʼ�ź�
    HAL_Delay(20);
    DHT11_DATA_SET(); // ���������߲��ȴ�20-40us
    delay_us(30);
}

/* �ӻ�Ӧ�� */
uint8_t Is_DHT11_ACK(void)
{
    uint8_t cnt = 0; // ��ʱ����
    DHT11_Send();    // ���Ϳ�ʼ�ź�
    DHT11_INPUT();   // �л�Ϊ����ģʽ
    while (DHT11_DATA_STA() == 1 && cnt < 100)
    {
        cnt++;
        delay_us(1); // �ȴ��͵�ƽ�źŵĵ���
    }

    if (cnt >= 100)
        return 1; // �����ʱ����Ϊ�ߵ�ƽ����dht11û��Ӧ
    else
        cnt = 0;
    while (DHT11_DATA_STA() == 0 && cnt < 100) // DHT11��Ӧ�ź�
    {
        cnt++;
        delay_us(1);
    }
    return (cnt > 100) ? 1 : 0; // �ж���Ӧ�ź��Ƿ�����
}
/* ��������(Bit) */
uint8_t DHT11_Data_Bit(void)
{
    uint8_t cnt = 0;
    while (1 == DHT11_DATA_STA() && cnt < 100) // dht11��Ӧ�źź󣬻��ٴ�����������80us����һ��whileѭ���ȴ������źŵĵ���
    {
        cnt++;
        delay_us(1);
    }
    if (cnt >= 100)
    {
        return 1;
    }
    else
        cnt = 0;
    while (0 == DHT11_DATA_STA() && cnt < 100)
    {
        cnt++;
        delay_us(1);
    } // DHT11ÿ����һ������λ֮ǰ���Ὣ����������50us�����whileѭ��Ҳ�������ȴ�����λ�ĵ���
    if (cnt >= 100)
    {
        return 1;
    }
    /* 
     * DHT11 defines whether the data bit is 0 or 1 in terms of the length of the high level.
     * When the 50us low level time slot is passed, the bus is pulled up. The high level lasts 
     * for 26 to 28us to indicate the data "0"; the high level lasts for 70us to indicate the 
     * data "1". 
     */
    /* 26~28us�ĸߵ�ƽ��������0��70us�ĸߵ�ƽ��������1*/
    else
        delay_us(30);                       // ��ʱ30us ����һ����ֵ���жϸߵ�ƽ�źŵ�ʱ�䣻
    return (DHT11_DATA_STA() == 1) ? 1 : 0; // �ߵ�ƽʱ�����26~28us����1����֮Ϊ0��
}

/* ��������(Byte) */
uint8_t DHT11_Data_Byte(void)
{
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= DHT11_Data_Bit();
    }
    return data;
}

uint8_t DHT11_Data(uint8_t *humi, uint8_t *temp)
{
    uint8_t buf[5]; // �洢��ʪ�ȵ���ֵ
    Is_DHT11_ACK();
    for (uint8_t i = 0; i < 5; i++)
    {
        buf[i] = DHT11_Data_Byte();
    }
    if (buf[0] + buf[1] + buf[2] + buf[3] == buf[4])
    {
        *humi = buf[0];
        *temp = buf[2];
    }
}
