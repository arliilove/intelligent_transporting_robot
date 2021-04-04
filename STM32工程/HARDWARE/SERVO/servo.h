#ifndef _SERVO_H
#define _SERVO_H

#include "usart.h"
#include "sys.h"
#include "delay.h"

extern UART_HandleTypeDef UART1_Handler; //UART¾ä±ú
typedef struct _SERVO_RETURN{
	u8 datalength;
	u8 instruction;
	u8 motion_group;
	u16 run_times;
}SERVO_RETURN;

extern SERVO_RETURN Servo_Return;
extern u8 Servobuf[1];

void UART1_Init(void);
void start_motion_group(u8 group);
u8 motion_fin(u8 group);
void servo_move(u8 ID, u16 time_ms, u16 position);

#endif
