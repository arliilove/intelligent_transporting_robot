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
	float zangle; //航向角  逆时针旋转为正
	float xangle; //俯仰角
	float yangle; //横滚角
	float pos_x; //坐标x
	float pos_y; //坐标y
	float w_z; //航向角速
}OPS_ENCODER;

void UART2_Init(void);
void OPS_Encoder_Init(void);

#endif
