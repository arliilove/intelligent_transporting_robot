#include "raspi.h"

u8 Raspibuf[1];//��ݮ�ɴ��ڻ���
UART_HandleTypeDef UART3_Handler;
Raspi Raspi_data={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
u8 Raspibuf[1];//��ݮ�ɴ��ڻ���
void UART3_Init()
{	
	UART3_Handler.Instance=USART3;					    //USART2
	UART3_Handler.Init.BaudRate=115200;				    //������
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	UART3_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&UART3_Handler);					    
	 
	HAL_UART_Receive_IT(&UART3_Handler, Raspibuf, 1);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������

}

void USART3_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART3_Handler);
	HAL_UART_Receive_IT(&UART3_Handler,Raspibuf,1);
}

int position_which(int task){
//��֪����˳��Ͱڷ�˳������ִ�еڼ������񣬷���Ӧ��ץ�����ҵ��ĸ�
//������֪����ɫ��˳�򣬷��ؽ����������ŵ�������
//0��1��2��
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


//֪ͨ��ݮ�ɣ���������巢0��ԭ����1���ӹ���2�����Ʒ��3

void inform_QR(){
//֪ͨ��ݮ�ɵ��������
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

