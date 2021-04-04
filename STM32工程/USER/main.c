#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "usmart.h"
#include "can.h"
#include "timer.h"
#include "encoder.h"
#include "underpan_control.h"
#include "servo.h"
#include "overall_control.h"
/************************************************
 ALIENTEK ��ӢSTM32F103������ ʵ��25
 CANʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
	


extern Motorstate motorstate;
extern Sensorstate sensorstate;
extern s16 motorset[4];
extern Target target;

//extern int target[4];
extern OPS_ENCODER OPS_Encoder;

int main(void)
{	
  HAL_Init();                    	 	//��ʼ��HAL��    
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	KEY_Init();							//��ʼ������
	usmart_dev.init(84); 		   		//��ʼ��USMART	
	LED_Init();							//��ʼ��LED	
	UART1_Init();
	UART2_Init();//uart2Ҫ��lcd��Ļ֮��
	UART3_Init();
	CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_8TQ,CAN_BS1_9TQ,4,CAN_MODE_LOOPBACK); //CAN��ʼ��,������500Kbps      
	CAN_Config();
	start_motion_group(0);//�����ʼ��������״̬
	while(!key_start()){};
	delay_ms(1000);

	TIM3_Init(360,4000);//��ʱ��Ҫ�������ǳ�ʼ��������֮��
	SENSOR_Init();

//	set_target(0,1000);
//	while(!reach_target()){}
//	delay_ms(1000);
//	set_target(0,0);
//	while(!reach_target()){}
//	delay_ms(1000);
//		

    while(1)
    {
			run();
		} 
}	





