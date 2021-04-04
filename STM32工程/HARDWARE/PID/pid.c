#include "pid.h"
//�����ƶ���0���1��ǰ��2��ǰ��3���
Target target={0.0};//Ŀ�����꣬��λmm,(x,y)
float speed_inc[4]={0};//�ٶ�����������ÿ�����Ӳ�Ҫ�������

//temp_fsm����
//const double Yaw_KP=0.4,Yaw_KD=8;
//const double Position_KP=2.5,Position_KI=0.000005,Position_KD=78;//Position_KI=0.0024
//const double Speed_KP=0.35,Speed_KD=0.8;

const double Yaw_KP=0.5,Yaw_KD=3;
const double Position_KP=8,Position_KI=0.00001,Position_KD=80;//Position_KI=0.0024
const double Speed_KP=1.5,Speed_KD=5;
void yaw_correction(){ //���ڶ�ʱ���ж��У�
	static float Bias;
	static float Last_Bias;
	
	Bias=OPS_Encoder.zangle;
	
	//BiasΪ����˵����ͷ����ƫ������Ҫ˳ʱ����ת�����0 ��2������biasͬ��
	motorset[0]+=Yaw_KP*Bias+Yaw_KD*(Bias-Last_Bias);
	motorset[1]-=Yaw_KP*Bias+Yaw_KD*(Bias-Last_Bias);
	motorset[2]+=Yaw_KP*Bias+Yaw_KD*(Bias-Last_Bias);
	motorset[3]-=Yaw_KP*Bias+Yaw_KD*(Bias-Last_Bias);
	
	Last_Bias=Bias;
}

int maxwheel(float a,float b, float c ,float d){
	//���ĸ��������ĸ��������󣬷�������
	float max;float temp_max;
	max=a>b?0:1;temp_max=a>b?a:b;
	max=temp_max>c?max:2;temp_max=temp_max>c?temp_max:c;
	max=temp_max>d?max:3;
	return max;
}


void pid_speed(){
//������ʵ��ת�ٱƽ�����ת��
	int i;
	static float Bias[4];
	static float Last_Bias[4];
	
	for(i=0;i<4;i++) {
		Bias[i]=motorset[i]-motorstate.speed[i];
		speed_inc[i]=Speed_KP*Bias[i]+Speed_KD*(Bias[i]-Last_Bias[i]);
		Last_Bias[i]=Bias[i];
	}
}


void pid_distance(){
//�ú������ڶ�ʱ���ж��У�������Ҫһ��ȫ�ֱ�����������   Ԥ�趨ʱ���ж�10Hz
//�Ƚ����ÿһʱ������ת��֮�ȣ��ٳ���һ��pid���Ƶı���
	int i;
	static float Bias[4];
	static float Last_Bias[4];
	static float Integral_Bias[4];
	static Target Last_target;//�����жϻ������Ƿ�����
	float dx;
	float dy;
	//float velocity[4];
	//float last_vel[4];
	//float pid_scalar;
	int maxwheelindex;
	float premotorset[4];
	
		
	dx=target.x-OPS_Encoder.pos_x;
	dy=target.y-OPS_Encoder.pos_y;
	
	//����Ϊ������ǰΪ������ʱ��Ϊ��
	//������Bias��Ϣ����ת������Ϣ
	Bias[0]=dy-dx; //+w(a+b)
	Bias[1]=dy+dx; //-
	Bias[2]=dy+dx; //-
	Bias[3]=dy-dx; //+

	//�ҵ���������������
	maxwheelindex=maxwheel(fabs(Bias[0]),fabs(Bias[1]),fabs(Bias[2]),fabs(Bias[3]));
	
	if(Last_target.x!=target.x || Last_target.y!=target.y) {
		for(i=0;i<4;i++) {
			Integral_Bias[i]=0;
			Last_Bias[i]=0;
			motorset[i]=STARTUP*Bias[i]/fabs(Bias[maxwheelindex]);
		}
	}
	
	for(i=0;i<4;i++) Integral_Bias[i]+=Bias[i];

	pid_speed();
	
	for(i=0;i<4;i++){
		premotorset[i]=Bias[i]*Position_KP+Integral_Bias[i]*Position_KI+(Bias[i]-Last_Bias[i])*Position_KD;
		premotorset[i]+=speed_inc[i];
	}
	

	premotorset[maxwheelindex]=Bias[maxwheelindex]*Position_KP+Integral_Bias[maxwheelindex]*Position_KI+(Bias[maxwheelindex]-Last_Bias[maxwheelindex])*Position_KD;
	
	
	//���Ƽ��ٶȲ�����MAX_ACC
//	if(fabs(premotorset[maxwheelindex]-motorset[maxwheelindex])>MAX_ACC) {
//		for(i=0;i<4;i++){
//			premotorset[i]=motorset[i]+(premotorset[i]-motorset[i])*MAX_ACC/fabs(premotorset[maxwheelindex]-motorset[maxwheelindex]);
//		}
//		LCD_ShowString(30,470,200,16,16,"max acc");	
//	} //��bug���������������Ӽ��ٶȻ����
	for(i=0;i<4;i++){
		if(fabs(premotorset[i]-motorset[i])>MAX_ACC) 
				premotorset[i]=motorset[i]+(premotorset[i]-motorset[i])*MAX_ACC/fabs(premotorset[i]-motorset[i]);
	}

	//�����ٶȲ�����MAX_SPEED
	if(fabs(premotorset[maxwheelindex])>MAX_SPEED){
		for(i=0;i<4;i++){
			premotorset[i]*=MAX_SPEED/fabs(premotorset[maxwheelindex]);
		}	
	}

	
	for(i=0;i<4;i++){
		motorset[i]=premotorset[i];
		Last_Bias[i]=Bias[i];
	}
	
	Last_target.x=target.x;
	Last_target.y=target.y;
}
