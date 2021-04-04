#ifndef ENCODER_H
#define ENCODER_H

#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "led.h"
#include "delay.h"
#include "raspi.h"
#include "servo.h"

#define RXBUFFERSIZE   28 	
extern u8 aRxBuffer[RXBUFFERSIZE];


typedef struct _OPS_ENCODER{
	float zangle; //�����  ��ʱ����תΪ��
	float xangle; //������
	float yangle; //�����
	float pos_x; //����x
	float pos_y; //����y
	float w_z; //�������
}OPS_ENCODER;

void UART2_Init(void);
void OPS_Encoder_Init(void);

#endif
