#ifndef __CAN_H
#define __CAN_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//CAN��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/9/18
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.
//#define USE_HAL_CAN_REGISTER_CALLBACKS = 1 
typedef struct MOTORSTATE{
	s16 speed[4];//ע�ⲻ���޷�������
	s16 pwm[4];
	u16 last_encode[4];
	u16 new_encode[4];
}Motorstate;

typedef struct SENSORSTATE{
	u8 boolean_value[4];
}Sensorstate;


u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode);//CAN��ʼ��
void CAN_Config(void);
u8 CAN1_Send_Msg(u8* msg,u8 len);						//��������
u8 CAN1_Receive_Msg(u8 *buf);							//��������
u8 CAN1_Send_Motor_Msg(u8* msg,u8 len,u8 ID);		
u8 CAN1_Receive_Motor_Msg(u8 *buf);
u8 CAN1_Send_Linesensor_Msg(u8* msg,u8 len,u8 ID);
u8 CAN1_Receive_Linesensor_Msg(u8 *buf);
void SENSOR_Init(void);
void Set_Motor_speed(s16* speed);
void CAN_Error_Handler(void);
#endif
