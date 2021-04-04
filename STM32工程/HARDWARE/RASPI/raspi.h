#ifndef _RASPI_H
#define _RASPI_H

#include "usart.h"
#include "sys.h"
#include "delay.h"

typedef struct RASPI{
	//1红2绿3蓝
	u8 task1;
	u8 task2;
	u8 task3;
	//task123为当前轮次的任务
	u8 left;
	u8 middle;
	u8 right;
	///左中右为当前轮次对应的货架上层或下层摆放顺序
	
	u8 t1;
	u8 t2;
	u8 t3;
	u8 t4;
	u8 t5;
	u8 t6;
	
	u8 upperleft;
	u8 uppermiddle;
	u8 upperright;
	u8 lowerleft;
	u8 lowermiddle;
	u8 lowerright;
	
	u8 ena; //当ena被置1时表示有数据，需要在一遍跑完后清零
} Raspi;

extern Raspi Raspi_data;//全局变量，用于储存树莓派传来的任务信息
extern u8 Raspibuf[1];//树莓派串口缓冲

void UART3_Init(void);

int position_which(int task);
void inform_QR(void);
void inform_src(void);
void inform_pro(void);
void inform_fin(void);


#endif
