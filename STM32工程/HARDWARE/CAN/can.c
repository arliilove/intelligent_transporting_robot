#include "can.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "stm32f1xx_hal.h"
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
//All rights reserved									////////////////////////////////////////////////
  
////////////////  //////////////////
CAN_HandleTypeDef   CAN1_Handler;  //CAN1句柄
CAN_TxHeaderTypeDef	TxHeader;      //发送
CAN_RxHeaderTypeDef	RxHeader;      //接收
Motorstate motorstate;
Sensorstate sensorstate;
s16 motorset[4]={0};
//电机id号为1234


////CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+tbs2+1)*brp); 其中tbs1和tbs2我们只用关注标识符上标志的序号，例如CAN_BS2_1TQ，我们就认为tbs2=1来计算即可。
//mode:CAN_MODE_NORMAL,普通模式;CAN_MODE_LOOPBACK,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_MODE_LOOPBACK);
//则波特率为:36M/((8+9+1)*4)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败; 
u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
	CAN_InitTypeDef		CAN1_InitConf;
    
    CAN1_Handler.Instance=CAN1;
	
	CAN1_Handler.Init = CAN1_InitConf;
	
    CAN1_Handler.Init.Prescaler=brp;				//分频系数(Fdiv)为brp+1
    CAN1_Handler.Init.Mode=mode;					//模式设置 
    CAN1_Handler.Init.SyncJumpWidth=tsjw;			//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN1_Handler.Init.TimeSeg1=tbs1;				//tbs1范围CAN_BS1_1TQ~CAN_BS1_16TQ
    CAN1_Handler.Init.TimeSeg2=tbs2;				//tbs2范围CAN_BS2_1TQ~CAN_BS2_8TQ
    CAN1_Handler.Init.TimeTriggeredMode=DISABLE;	//非时间触发通信模式 
    CAN1_Handler.Init.AutoBusOff=DISABLE;			//软件自动离线管理
    CAN1_Handler.Init.AutoWakeUp=DISABLE;			//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN1_Handler.Init.AutoRetransmission=ENABLE;	//禁止报文自动传送 
    CAN1_Handler.Init.ReceiveFifoLocked=DISABLE;	//报文不锁定,新的覆盖旧的 
    CAN1_Handler.Init.TransmitFifoPriority=DISABLE;	//优先级由报文标识符决定 
	
    if(HAL_CAN_Init(&CAN1_Handler)!=HAL_OK)			//初始化
		return 1;
    return 0;
}

//CAN底层驱动，引脚配置，时钟配置，中断配置
//此函数会被HAL_CAN_Init()调用
//hcan:CAN句柄
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_CAN1_CLK_ENABLE();                //使能CAN1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			    //开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_12;   			//PA12
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //初始化
	
	GPIO_Initure.Pin=GPIO_PIN_11;   			//PA11
	GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;     	//推挽复用
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //初始化
	
//	 HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
	 HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	  HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
//		HAL_CAN_IRQHandler(&CAN1_Handler);
}

void CAN_Config(void)
{
  CAN_FilterTypeDef  sFilterConfig;

  /*配置CAN过滤器*/
  sFilterConfig.FilterBank = 0;                     //过滤器0
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;              //32位ID
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;          //32位MASK
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;//过滤器0关联到FIFO0
  sFilterConfig.FilterActivation = ENABLE;          //激活滤波器0
  sFilterConfig.SlaveStartFilterBank = 14;
	
  //过滤器配置
  if (HAL_CAN_ConfigFilter(&CAN1_Handler, &sFilterConfig) != HAL_OK)
  {
    while(1){}
  }

  //启动CAN外围设备
  if (HAL_CAN_Start(&CAN1_Handler) != HAL_OK)
  {
    while(1){}
  }
	HAL_CAN_RegisterCallback(&CAN1_Handler, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, HAL_CAN_RxFifo0MsgPendingCallback);
  //激活可以RX通知
  if (HAL_CAN_ActivateNotification(&CAN1_Handler, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    while(1){}
  }

  /*配置传输过程*/
  TxHeader.StdId = 0x321;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 2;
  TxHeader.TransmitGlobalTime = DISABLE;
}

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
    u8 i=0;
	u32 TxMailbox;
	u8 message[8];
    TxHeader.StdId=0X12;        //标准标识符
    TxHeader.ExtId=0x12;        //扩展标识符(29位)
    TxHeader.IDE=CAN_ID_STD;    //使用标准帧
    TxHeader.RTR=CAN_RTR_DATA;  //数据帧
    TxHeader.DLC=len;                
    for(i=0;i<len;i++)
    {
		message[i]=msg[i];
	}
    if(HAL_CAN_AddTxMessage(&CAN1_Handler, &TxHeader, message, &TxMailbox) != HAL_OK)//发送
	{
		return 1;
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&CAN1_Handler) != 3) {}
    return 0;
}


//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN1_Receive_Msg(u8 *buf)
{
 	u32 i;
	u8	RxData[8];

	if(HAL_CAN_GetRxFifoFillLevel(&CAN1_Handler, CAN_RX_FIFO0) != 1)
	{
		return 0xF1;
	}

	if(HAL_CAN_GetRxMessage(&CAN1_Handler, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	{
		return 0xF2;
	}
    for(i=0;i<RxHeader.DLC;i++)
    buf[i]=RxData[i];
	
	return RxHeader.DLC;
}

u8 CAN1_Send_Motor_Msg(u8* msg,u8 len,u8 ID)
{	
    u8 i=0;
	u32 TxMailbox;
	u8 message[8];
    TxHeader.StdId=0X12;        //标准标识符
    TxHeader.ExtId=0x00200010+ID*0x10000;        //扩展标识符(29位)
    TxHeader.IDE=CAN_ID_EXT;    //使用扩展帧
    TxHeader.RTR=CAN_RTR_DATA;  //数据帧
    TxHeader.DLC=len;                
    for(i=0;i<len;i++)
    {
		message[i]=msg[i];
	}
    if(HAL_CAN_AddTxMessage(&CAN1_Handler, &TxHeader, message, &TxMailbox) != HAL_OK)//发送
	{
		return 1;//返回1为出错
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&CAN1_Handler) != 3) {}
    return 0;
}


void Set_Motor_speed(s16* speed){//传入motorset指针
	u8 canbuf[8]={0,0,0,0,0,0,0,0};
	s16 val;
	int i;
	for(i=0;i<4;i++) {
		val=(speed[i]+16000)*2;
		canbuf[0]=val;
		canbuf[1]=val>>8;
		if(CAN1_Send_Motor_Msg(canbuf,8,i)) LCD_ShowString(30+80,190,200,16,16,"Failed");//CAN_Error_Handler();
		delay_ms(1);
	}
}

u8 CAN1_Receive_Motor_Msg(u8 *buf)
{
 	u32 i;
	u8	RxData[8];

	if(HAL_CAN_GetRxFifoFillLevel(&CAN1_Handler, CAN_RX_FIFO0) < 1) //!!!!!!!写=1会卡死！！！
	{
		return 0xF1;
	}

	if(HAL_CAN_GetRxMessage(&CAN1_Handler, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	{
		return 0xF2;
	}
    for(i=0;i<RxHeader.DLC;i++)
    buf[i]=RxData[i];
	
	buf[i]= (RxHeader.ExtId & 0x0f);
	return RxHeader.DLC;
}

u8 CAN1_Send_Linesensor_Msg(u8* msg,u8 len,u8 ID)
{	
    u8 i=0;
	u32 TxMailbox;
	u8 message[8];
    TxHeader.StdId=0X12;        //标准标识符
    TxHeader.ExtId=0x00300010+ID*0x10000;        //扩展标识符(29位)
    TxHeader.IDE=CAN_ID_EXT;    //使用扩展帧
    TxHeader.RTR=CAN_RTR_DATA;  //数据帧
    TxHeader.DLC=len;                
    for(i=0;i<len;i++)
    {
		message[i]=msg[i];
	}
    if(HAL_CAN_AddTxMessage(&CAN1_Handler, &TxHeader, message, &TxMailbox) != HAL_OK)//发送
	{
		return 1;
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&CAN1_Handler) != 3) {}
    return 0;
}

void SENSOR_Init(void){
	int i;
	u8 Data[2]={3,0};//关闭AD值反馈
	for(i=0;i<4;i++){
		if(CAN1_Send_Linesensor_Msg(Data,2,i)) LCD_ShowString(30+80,190,200,16,16,"Failed"); //CAN_Error_Handler();
		delay_ms(1);
	}
}

u8 CAN1_Receive_Linesensor_Msg(u8 *buf) //接受布尔量
{
 	u32 i;
	u8	RxData[8];

	if(HAL_CAN_GetRxFifoFillLevel(&CAN1_Handler, CAN_RX_FIFO0) < 1)
	{
		return 0xF1;
	}

	if(HAL_CAN_GetRxMessage(&CAN1_Handler, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	{
		return 0xF2;
	}
    for(i=0;i<RxHeader.DLC;i++)
    buf[i]=RxData[i];
	
	buf[9]=(RxHeader.ExtId & 0x0f);
	return RxHeader.DLC;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CAN1_Handler){
//	u32 i;

	u8	RxData[8];
	u8  ID;

	//电机驱动0x0ff0002x
	//寻迹布尔0x0ff0003x
	if(HAL_CAN_GetRxFifoFillLevel(CAN1_Handler, CAN_RX_FIFO0) < 1) //!!!!!!!写=1会卡死！！！
	{
		CAN_Error_Handler();
	}
	if(HAL_CAN_GetRxMessage(CAN1_Handler, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	{
		CAN_Error_Handler();
	}
	ID=RxHeader.ExtId & 0x0f;
	if((RxHeader.ExtId>>4)==0x0ff0002){//电机
			//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);//指示灯
		motorstate.speed[ID]=(RxData[0]+ (RxData[1]<<8))/2-16000;
		motorstate.pwm[ID]=RxData[2]+(RxData[3]<<8);
		motorstate.last_encode[ID]=motorstate.new_encode[ID];
		motorstate.new_encode[ID]=RxData[5]+(RxData[6]<<8);
	}
	else if((RxHeader.ExtId>>4)==0x0ff0003){//寻迹
		sensorstate.boolean_value[ID]=RxData[0];
	}
	
		 // for(i=0;i<4;i++)
		//LCD_ShowxNum(30+i*32,270,RxData[i],3,16,0X80);
	//LCD_ShowxNum(30,270,motorstate.speed[ID-1],3,16,0X80);
	
}

//void HAL_CAN_IRQHandler(CAN_HandleTypeDef *CAN1_Handler){
//		u8	RxData[8];
//	u8  ID;
//	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
//	if(HAL_CAN_GetRxMessage(CAN1_Handler, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
//	{
//		CAN_Error_Handler();
//	}
//	ID=RxHeader.ExtId & 0x0f;
//	motorstate.speed[ID-1]=RxData[0]+RxData[1]*2^8;
//	motorstate.pwm[ID-1]=RxData[2]+RxData[3]*2^8;
//	motorstate.last_encode[ID-1]=motorstate.new_encode[ID-1];
//	motorstate.new_encode[ID-1]=RxData[5]+RxData[6]*2^8;
//	
//	LCD_ShowxNum(30,270,motorstate.speed[ID-1],3,16,0X80);
//}
void CAN_Error_Handler(void){
	LCD_ShowString(30,130,400,16,16, "CAN ERROR");
}

void CAN1_RX0_IRQHandler(void){
	HAL_CAN_IRQHandler(&CAN1_Handler);
}
