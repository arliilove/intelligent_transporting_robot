#include "can.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "stm32f1xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//CAN��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/9/18
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									////////////////////////////////////////////////
  
////////////////  //////////////////
CAN_HandleTypeDef   CAN1_Handler;  //CAN1���
CAN_TxHeaderTypeDef	TxHeader;      //����
CAN_RxHeaderTypeDef	RxHeader;      //����
Motorstate motorstate;
Sensorstate sensorstate;
s16 motorset[4]={0};
//���id��Ϊ1234


////CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+tbs2+1)*brp); ����tbs1��tbs2����ֻ�ù�ע��ʶ���ϱ�־����ţ�����CAN_BS2_1TQ�����Ǿ���Ϊtbs2=1�����㼴�ɡ�
//mode:CAN_MODE_NORMAL,��ͨģʽ;CAN_MODE_LOOPBACK,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_MODE_LOOPBACK);
//������Ϊ:36M/((8+9+1)*4)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 
u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
	CAN_InitTypeDef		CAN1_InitConf;
    
    CAN1_Handler.Instance=CAN1;
	
	CAN1_Handler.Init = CAN1_InitConf;
	
    CAN1_Handler.Init.Prescaler=brp;				//��Ƶϵ��(Fdiv)Ϊbrp+1
    CAN1_Handler.Init.Mode=mode;					//ģʽ���� 
    CAN1_Handler.Init.SyncJumpWidth=tsjw;			//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN1_Handler.Init.TimeSeg1=tbs1;				//tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ
    CAN1_Handler.Init.TimeSeg2=tbs2;				//tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ
    CAN1_Handler.Init.TimeTriggeredMode=DISABLE;	//��ʱ�䴥��ͨ��ģʽ 
    CAN1_Handler.Init.AutoBusOff=DISABLE;			//����Զ����߹���
    CAN1_Handler.Init.AutoWakeUp=DISABLE;			//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN1_Handler.Init.AutoRetransmission=ENABLE;	//��ֹ�����Զ����� 
    CAN1_Handler.Init.ReceiveFifoLocked=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ� 
    CAN1_Handler.Init.TransmitFifoPriority=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
	
    if(HAL_CAN_Init(&CAN1_Handler)!=HAL_OK)			//��ʼ��
		return 1;
    return 0;
}

//CAN�ײ��������������ã�ʱ�����ã��ж�����
//�˺����ᱻHAL_CAN_Init()����
//hcan:CAN���
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_CAN1_CLK_ENABLE();                //ʹ��CAN1ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			    //����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_12;   			//PA12
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
    GPIO_Initure.Pull=GPIO_PULLUP;              //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //��ʼ��
	
	GPIO_Initure.Pin=GPIO_PIN_11;   			//PA11
	GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;     	//���츴��
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //��ʼ��
	
//	 HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
	 HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	  HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
//		HAL_CAN_IRQHandler(&CAN1_Handler);
}

void CAN_Config(void)
{
  CAN_FilterTypeDef  sFilterConfig;

  /*����CAN������*/
  sFilterConfig.FilterBank = 0;                     //������0
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;              //32λID
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;          //32λMASK
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;//������0������FIFO0
  sFilterConfig.FilterActivation = ENABLE;          //�����˲���0
  sFilterConfig.SlaveStartFilterBank = 14;
	
  //����������
  if (HAL_CAN_ConfigFilter(&CAN1_Handler, &sFilterConfig) != HAL_OK)
  {
    while(1){}
  }

  //����CAN��Χ�豸
  if (HAL_CAN_Start(&CAN1_Handler) != HAL_OK)
  {
    while(1){}
  }
	HAL_CAN_RegisterCallback(&CAN1_Handler, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, HAL_CAN_RxFifo0MsgPendingCallback);
  //�������RX֪ͨ
  if (HAL_CAN_ActivateNotification(&CAN1_Handler, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    while(1){}
  }

  /*���ô������*/
  TxHeader.StdId = 0x321;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 2;
  TxHeader.TransmitGlobalTime = DISABLE;
}

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
    u8 i=0;
	u32 TxMailbox;
	u8 message[8];
    TxHeader.StdId=0X12;        //��׼��ʶ��
    TxHeader.ExtId=0x12;        //��չ��ʶ��(29λ)
    TxHeader.IDE=CAN_ID_STD;    //ʹ�ñ�׼֡
    TxHeader.RTR=CAN_RTR_DATA;  //����֡
    TxHeader.DLC=len;                
    for(i=0;i<len;i++)
    {
		message[i]=msg[i];
	}
    if(HAL_CAN_AddTxMessage(&CAN1_Handler, &TxHeader, message, &TxMailbox) != HAL_OK)//����
	{
		return 1;
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&CAN1_Handler) != 3) {}
    return 0;
}


//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
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
    TxHeader.StdId=0X12;        //��׼��ʶ��
    TxHeader.ExtId=0x00200010+ID*0x10000;        //��չ��ʶ��(29λ)
    TxHeader.IDE=CAN_ID_EXT;    //ʹ����չ֡
    TxHeader.RTR=CAN_RTR_DATA;  //����֡
    TxHeader.DLC=len;                
    for(i=0;i<len;i++)
    {
		message[i]=msg[i];
	}
    if(HAL_CAN_AddTxMessage(&CAN1_Handler, &TxHeader, message, &TxMailbox) != HAL_OK)//����
	{
		return 1;//����1Ϊ����
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&CAN1_Handler) != 3) {}
    return 0;
}


void Set_Motor_speed(s16* speed){//����motorsetָ��
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

	if(HAL_CAN_GetRxFifoFillLevel(&CAN1_Handler, CAN_RX_FIFO0) < 1) //!!!!!!!д=1�Ῠ��������
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
    TxHeader.StdId=0X12;        //��׼��ʶ��
    TxHeader.ExtId=0x00300010+ID*0x10000;        //��չ��ʶ��(29λ)
    TxHeader.IDE=CAN_ID_EXT;    //ʹ����չ֡
    TxHeader.RTR=CAN_RTR_DATA;  //����֡
    TxHeader.DLC=len;                
    for(i=0;i<len;i++)
    {
		message[i]=msg[i];
	}
    if(HAL_CAN_AddTxMessage(&CAN1_Handler, &TxHeader, message, &TxMailbox) != HAL_OK)//����
	{
		return 1;
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&CAN1_Handler) != 3) {}
    return 0;
}

void SENSOR_Init(void){
	int i;
	u8 Data[2]={3,0};//�ر�ADֵ����
	for(i=0;i<4;i++){
		if(CAN1_Send_Linesensor_Msg(Data,2,i)) LCD_ShowString(30+80,190,200,16,16,"Failed"); //CAN_Error_Handler();
		delay_ms(1);
	}
}

u8 CAN1_Receive_Linesensor_Msg(u8 *buf) //���ܲ�����
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

	//�������0x0ff0002x
	//Ѱ������0x0ff0003x
	if(HAL_CAN_GetRxFifoFillLevel(CAN1_Handler, CAN_RX_FIFO0) < 1) //!!!!!!!д=1�Ῠ��������
	{
		CAN_Error_Handler();
	}
	if(HAL_CAN_GetRxMessage(CAN1_Handler, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	{
		CAN_Error_Handler();
	}
	ID=RxHeader.ExtId & 0x0f;
	if((RxHeader.ExtId>>4)==0x0ff0002){//���
			//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);//ָʾ��
		motorstate.speed[ID]=(RxData[0]+ (RxData[1]<<8))/2-16000;
		motorstate.pwm[ID]=RxData[2]+(RxData[3]<<8);
		motorstate.last_encode[ID]=motorstate.new_encode[ID];
		motorstate.new_encode[ID]=RxData[5]+(RxData[6]<<8);
	}
	else if((RxHeader.ExtId>>4)==0x0ff0003){//Ѱ��
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
