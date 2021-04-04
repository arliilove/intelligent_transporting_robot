#include "servo.h"

UART_HandleTypeDef UART1_Handler; //UART���
u8 Servobuf[1];
SERVO_RETURN Servo_Return;

void UART1_Init()
{	
	UART1_Handler.Instance=USART1;					    //USART2
	UART1_Handler.Init.BaudRate=9600;				    //������ư岨����Ϊ9600����
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&UART1_Handler);					    
	 
	HAL_UART_Receive_IT(&UART1_Handler, Servobuf, 1);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������

}

void USART1_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART1_Handler);
	HAL_UART_Receive_IT(&UART1_Handler,Servobuf,1);
}

void start_motion_group(u8 group){
//���Ϸ��Ϳ�ʼָ����յ�ȷ�Ͽ�ʼ��ֹͣ����
	u8 i=0;
	u8 ServoTxbuf[8];
	ServoTxbuf[0]=0x55;	ServoTxbuf[1]=0x55;
	ServoTxbuf[2]=0x05; //datalenth=5
	ServoTxbuf[3]=0x06; //instruction coide, run a certain motion group
	ServoTxbuf[4]=group; //motiong group index
	ServoTxbuf[5]=0x01;	ServoTxbuf[6]=0x00; //run 1 time
	
//	while((Servo_Return.instruction!=0x06 || Servo_Return.motion_group!=group) && i<10) {//��ε���start_motion_group()Ҳֻ�ᷢ��һ��
//	while(Servo_Return.motion_group!=group) {//��ε���start_motion_group()Ҳֻ�ᷢ��һ��		
		HAL_UART_Transmit(&UART1_Handler,ServoTxbuf,7,1000);
//		delay_ms(20);
//		i++; //���ѭ������10�Σ������û�յ���ֱ���˳�
//	}
}

u8 motion_fin(u8 group){
//����ѯ�Ķ���������ˣ�����1�����򷵻�0
	static long int i=0; 
	static u8 last_group;
	if(i<10000 || group!=last_group){
		if(Servo_Return.instruction==0x08 && Servo_Return.motion_group==group)
			return 1;
		else{
			i++;
			last_group=group;
			return 0;
		}
	}
	else{
		i=0;
		last_group=group;
		return 1;
	}
}

void servo_move(u8 ID, u16 time_ms, u16 position){
	u8 ServoTxbuf[10];
	ServoTxbuf[0]=0x55;
	ServoTxbuf[1]=0x55;
	ServoTxbuf[2]=0x08;
	ServoTxbuf[3]=0x03;
	ServoTxbuf[4]=1;
	ServoTxbuf[5]=time_ms & 0xFF;
	ServoTxbuf[6]=time_ms>>8;
	ServoTxbuf[7]=ID;
	ServoTxbuf[8]=position & 0x00FF;
	ServoTxbuf[9]=position>>8;
	
	HAL_UART_Transmit(&UART1_Handler,ServoTxbuf,10,1000);
} 
