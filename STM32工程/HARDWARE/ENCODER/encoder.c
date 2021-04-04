#include "encoder.h"
#include "string.h"
UART_HandleTypeDef UART2_Handler;

OPS_ENCODER OPS_Encoder={0,0,0,0,0,0};

u8 Rxbuf[1];

float byteassemble(u8 a,u8 b,u8 c,u8 d){//a is the lowest byte
	u8 temp[4];
	//LED1=!LED1;
	temp[0]=a;temp[1]=b;temp[2]=c;temp[3]=d;
	//LCD_ShowxNum(30+4*32,470,*(float*)temp,3,16,0X80);
	return *(float*)temp;
}

void UART2_Init()
{	
	char buf[4]="ACT0";
	
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=115200;				    //波特率
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&UART2_Handler);					    
	 
	HAL_UART_Transmit(&UART2_Handler,(u8 *)buf,4,1000);
	//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);	//PE5置1，默认初始化后灯灭
	HAL_UART_Receive_IT(&UART2_Handler, Rxbuf, 1);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
		GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,0,0);			//抢占优先级3，子优先级3
	}

	
	if(huart->Instance==USART2)//如果是串口2，进行串口2 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART2_CLK_ENABLE();			//使能USART1时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_2;			//PA2
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速

		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_3;			//PA3
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		

		HAL_NVIC_EnableIRQ(USART2_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART2_IRQn,1,1);			//抢占优先级3，子优先级3

	}
	
		
		if(huart->Instance==USART3)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART3_CLK_ENABLE();			//使能USART1时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_10;			//PB10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速

		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB10

		GPIO_Initure.Pin=GPIO_PIN_11;			//PB11
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB11
		

		HAL_NVIC_EnableIRQ(USART3_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART3_IRQn,1,1);			//抢占优先级3，子优先级3

	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static u8 servoreturnbuf[8];
	if(huart->Instance==USART1){//舵机
		static u8 count=0;

		if((count!=1 && count!=0) && Servobuf[0]==0x55) 
			count=0;
		
		switch(count){
			case 0:{
				if(Servobuf[0]==0x55) count++;
				else count=0;
				break;
			}
			
			case 1:{
				if(Servobuf[0]==0x55) count++;
				else count=0;
				break;
			}
			
			case 2:{//datalength
				//Servo_Return.datalength=Servobuf[0];
				servoreturnbuf[0]=Servobuf[0];
				count++;
				break;
			}
			
			case 3:{//instruction
				//Servo_Return.instruction=Servobuf[0];
				servoreturnbuf[1]=Servobuf[0];
				if(Servobuf[0]==0x07) {
					count=0;  //无参数，直接结束接收
					Servo_Return.motion_group=0xFF;
					Servo_Return.run_times=0xFFFF;
				}
				else count++;
				break;
			}
			
			case 4:{//motion group
				//Servo_Return.motion_group=Servobuf[0];
				servoreturnbuf[2]=Servobuf[0];
				count++;
				break;
			}
			
			case 5:{//low8bit
				//Servo_Return.run_times=Servobuf[0];
				servoreturnbuf[3]=Servobuf[0];
				count++;
				break;
			}
			
			case 6:{
				u16 high8bit;
				high8bit=Servobuf[0];
				//servoreturnbuf[4]=Servobuf[0];
				Servo_Return.datalength=servoreturnbuf[0];
				Servo_Return.instruction=servoreturnbuf[1];
				Servo_Return.motion_group=servoreturnbuf[2];
				Servo_Return.run_times=servoreturnbuf[3];
				Servo_Return.run_times+=(high8bit<<8);
				count=0;
				break;
			}
			
			default:
				count=0;
				break;
		}
		
	}
	if(huart->Instance==USART2){//编码器
//		u8 buf[28];
		static union{
			u8 data[24];
			float ActVal[6];
		}posture;
		static u8 i=0;
		static u8 count=0;
		switch (count){
			case 0:
				if(Rxbuf[0]==0x0d) count++;
				else count=0;
				break;
			
			case 1:
				if(Rxbuf[0]==0x0a){
					i=0;
					count++;
				}
				else if(Rxbuf[0]==0x0d);
				else count=0;
				break;
				
			case 2:
				posture.data[i]=Rxbuf[0];
				i++;
				if(i>=24){
					i=0;
					count++;
				}
				break;
				
			case 3:
				if(Rxbuf[0]==0x0a) count++;
				else count=0;
				break;
			
			case 4:
				if(Rxbuf[0]==0x0d){
					OPS_Encoder.zangle=posture.ActVal[0];
					OPS_Encoder.xangle=posture.ActVal[1];
					OPS_Encoder.yangle=posture.ActVal[2];
					OPS_Encoder.pos_x =-posture.ActVal[3]/2;
					OPS_Encoder.pos_y =posture.ActVal[4]/2;
					OPS_Encoder.w_z =posture.ActVal[5];
					LED0=!LED0;
				}
				count=0;
				break;
				default:
				count=0;
		}
	}
	
	if(huart->Instance==USART3){
		static u8 i=0;
		static u8 count=0;
		static u8 Raspimes[8];//临时储存接收到的树莓派信息。协议校验完毕后会转存到全局变量Raspi_data
	
		switch (count){
			case 0:
				if(Raspibuf[0]==0x0d) count++;
				else count=0;
				break;
			
			case 1:
				if(Raspibuf[0]==0x0a){
					i=0;
					count++;
				}
				else if(Raspibuf[0]==0x0d);
				else count=0;
				break;
				
			case 2:
				Raspimes[i]=Raspibuf[0];
				i++;
				if(i>=7){
					i=0;
					count++;
				}
				break;
				
			case 3:
				Raspimes[7]=Raspibuf[0];
				if(Raspibuf[0]==0x0a) count++;
				else count=0;
				break;
			
			case 4:
				if(Raspibuf[0]==0x0d){
					char con=Raspimes[3];
					if(!strcmp(&con,"+")){//相等返回0
						Raspi_data.t1=Raspimes[0];
						Raspi_data.t2=Raspimes[1];
						Raspi_data.t3=Raspimes[2];
						Raspi_data.t4=Raspimes[4];
						Raspi_data.t5=Raspimes[5];
						Raspi_data.t6=Raspimes[6];	

						Raspi_data.task1=Raspimes[0];
						Raspi_data.task2=Raspimes[1];
						Raspi_data.task3=Raspimes[2];
						//为第一圈的任务信息赋值
					}
					else if(!strcmp(&con,"-")){
						Raspi_data.upperleft=Raspimes[0];
						Raspi_data.uppermiddle=Raspimes[1];
						Raspi_data.upperright=Raspimes[2];
						Raspi_data.lowerleft=Raspimes[4];
						Raspi_data.lowermiddle=Raspimes[5];
						Raspi_data.lowerright=Raspimes[6];
						
						Raspi_data.left=Raspi_data.upperleft;
					  Raspi_data.middle=Raspi_data.uppermiddle;
					  Raspi_data.right=Raspi_data.upperright;
						//为第一圈的任务信息赋值
					}		
					Raspi_data.ena=1;		
				}
				count=0;
				break;
				
			default:
				count=0;
			
		}
	}
	
}



void USART2_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART2_Handler);

	HAL_UART_Receive_IT(&UART2_Handler,Rxbuf,1);
}

void OPS_Encoder_Init(){
	char buf[4]="ACT0";
	while (USART2->SR==USART_FLAG_RXNE){};
   //UART2_Handler.Lock= HAL_LOCKED; 
//	 HAL_UART_Transmit(&UART2_Handler,(u8 *)buf,4,1000);
	 HAL_UART_Transmit(&UART1_Handler,(u8 *)buf,4,1000);
	 //__HAL_UNLOCK(&UART2_Handler);
}

