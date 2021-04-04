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
 ALIENTEK 精英STM32F103开发板 实验25
 CAN实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
	


extern Motorstate motorstate;
extern Sensorstate sensorstate;
extern s16 motorset[4];
extern Target target;

//extern int target[4];
extern OPS_ENCODER OPS_Encoder;

int main(void)
{	
  HAL_Init();                    	 	//初始化HAL库    
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	KEY_Init();							//初始化按键
	usmart_dev.init(84); 		   		//初始化USMART	
	LED_Init();							//初始化LED	
	UART1_Init();
	UART2_Init();//uart2要在lcd屏幕之后
	UART3_Init();
	CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_8TQ,CAN_BS1_9TQ,4,CAN_MODE_LOOPBACK); //CAN初始化,波特率500Kbps      
	CAN_Config();
	start_motion_group(0);//舵机初始化到收缩状态
	while(!key_start()){};
	delay_ms(1000);

	TIM3_Init(360,4000);//定时器要在陀螺仪初始化且清零之后
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





