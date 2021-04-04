#ifndef _RASPI_H
#define _RASPI_H

#include "usart.h"
#include "sys.h"
#include "delay.h"

typedef struct RASPI{
	//1��2��3��
	u8 task1;
	u8 task2;
	u8 task3;
	//task123Ϊ��ǰ�ִε�����
	u8 left;
	u8 middle;
	u8 right;
	///������Ϊ��ǰ�ִζ�Ӧ�Ļ����ϲ���²�ڷ�˳��
	
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
	
	u8 ena; //��ena����1ʱ��ʾ�����ݣ���Ҫ��һ�����������
} Raspi;

extern Raspi Raspi_data;//ȫ�ֱ��������ڴ�����ݮ�ɴ�����������Ϣ
extern u8 Raspibuf[1];//��ݮ�ɴ��ڻ���

void UART3_Init(void);

int position_which(int task);
void inform_QR(void);
void inform_src(void);
void inform_pro(void);
void inform_fin(void);


#endif
