#include "raspi.h"

u8 Raspibuf[1];//树莓派串口缓冲
UART_HandleTypeDef UART3_Handler;
Raspi Raspi_data={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
u8 Raspibuf[1];//树莓派串口缓冲
void UART3_Init()
{	
	UART3_Handler.Instance=USART3;					    //USART2
	UART3_Handler.Init.BaudRate=115200;				    //波特率
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	UART3_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&UART3_Handler);					    
	 
	HAL_UART_Receive_IT(&UART3_Handler, Raspibuf, 1);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量

}

void USART3_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART3_Handler);
	HAL_UART_Receive_IT(&UART3_Handler,Raspibuf,1);
}

int position_which(int task){
//已知任务顺序和摆放顺序，输入执行第几个任务，返回应该抓左中右的哪个
//或者已知地上色环顺序，返回将货仓上物块放到左中右
//0左1中2右
	if(task==1){
		if(Raspi_data.task1==Raspi_data.left) return 0;
		if(Raspi_data.task1==Raspi_data.middle) return 1;
		if(Raspi_data.task1==Raspi_data.right) return 2;
	}
	
	if(task==2){
		if(Raspi_data.task2==Raspi_data.left) return 0;
		if(Raspi_data.task2==Raspi_data.middle) return 1;
		if(Raspi_data.task2==Raspi_data.right) return 2;
	}
	
	if(task==3){
		if(Raspi_data.task3==Raspi_data.left) return 0;
		if(Raspi_data.task3==Raspi_data.middle) return 1;
		if(Raspi_data.task3==Raspi_data.right) return 2;
	}	
	
	return -1;
}


//通知树莓派：到达任务板发0，原料区1，加工区2，半成品区3

void inform_QR(){
//通知树莓派到达任务板
	char reach_QR[5]="\r\n0\n\r";
	Raspi_data.ena=0;	
	HAL_UART_Transmit(&UART3_Handler,(u8 *)reach_QR,5,1000);
}

void inform_src(){
	char reach_src[5]="\r\n1\n\r";
	Raspi_data.ena=0;
	HAL_UART_Transmit(&UART3_Handler,(u8 *)reach_src,5,1000);
}

void inform_pro(){
	char reach_pro[5]="\r\n1\n\r";
	Raspi_data.ena=0;
	HAL_UART_Transmit(&UART3_Handler,(u8 *)reach_pro,5,1000);
}

void inform_fin(){
	char reach_fin[5]="\r\n1\n\r";
	Raspi_data.ena=0;
	HAL_UART_Transmit(&UART3_Handler,(u8 *)reach_fin,5,1000);
}

