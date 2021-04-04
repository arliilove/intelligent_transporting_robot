#ifndef __CAN_H
#define __CAN_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//CAN驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/9/18
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	1		//0,不使能;1,使能.
//#define USE_HAL_CAN_REGISTER_CALLBACKS = 1 
typedef struct MOTORSTATE{
	s16 speed[4];//注意不是无符号数！
	s16 pwm[4];
	u16 last_encode[4];
	u16 new_encode[4];
}Motorstate;

typedef struct SENSORSTATE{
	u8 boolean_value[4];
}Sensorstate;


u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode);//CAN初始化
void CAN_Config(void);
u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据
u8 CAN1_Receive_Msg(u8 *buf);							//接收数据
u8 CAN1_Send_Motor_Msg(u8* msg,u8 len,u8 ID);		
u8 CAN1_Receive_Motor_Msg(u8 *buf);
u8 CAN1_Send_Linesensor_Msg(u8* msg,u8 len,u8 ID);
u8 CAN1_Receive_Linesensor_Msg(u8 *buf);
void SENSOR_Init(void);
void Set_Motor_speed(s16* speed);
void CAN_Error_Handler(void);
#endif
