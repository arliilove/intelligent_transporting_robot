#include "underpan_control.h"
#include <stdio.h>
#include <stdlib.h>
	//1.set_target,ִ���������̿�ʼ�ƶ�
	//2.�ƶ���ͬʱת����е��
	//3.����֮��������е�۶����飨����ж����
	//4.ץ�������������µ���������set_target��һ��Ŀ�ĵأ�ͬʱ��е�۽����һ�����������ֻ��λ����������ɫһһ��Ӧ
	//  �Ƿ񽫻�е�۹�λд����һ��task
int motorlock=0;

void set_target(float x, float y){
	target.x=x;target.y=y;
	motorlock=1;//����ʱ���ж��������
}

int reach_target(){
	//�������Ŀ�꣬�رյ��������1
	
	if(fabs(OPS_Encoder.pos_x-target.x)<4 && fabs(OPS_Encoder.pos_y-target.y)<4 && fabs(OPS_Encoder.zangle)<2 && motorstate.speed[0]<4 && motorstate.speed[1]<4 && motorstate.speed[2]<4 && motorstate.speed[3]<4){//�ж�����
		motorlock=0;
		return 1;
	}
	else return 0;
}

int reach_origin(){
//��ԭ��ר�ã���������
	if((fabs(OPS_Encoder.pos_x-target.x)<4 && fabs(OPS_Encoder.pos_y-target.y)<4 && fabs(OPS_Encoder.zangle)<3 && motorstate.speed[0]<5 && motorstate.speed[1]<5 && motorstate.speed[2]<5 && motorstate.speed[3]<5) || (OPS_Encoder.pos_x<0 && OPS_Encoder.pos_y<0)){//�ж�����
		motorlock=0;
		return 1;
	}
	else return 0;
}

int reach_dest(){
//��ԭ��ר�ã���������
	if((fabs(OPS_Encoder.pos_x-target.x)<4 && fabs(OPS_Encoder.pos_y-target.y)<4 && fabs(OPS_Encoder.zangle)<3 && motorstate.speed[0]<5 && motorstate.speed[1]<5 && motorstate.speed[2]<5 && motorstate.speed[3]<5) || (OPS_Encoder.pos_x<0 && OPS_Encoder.pos_y<0)){//�ж�����
		motorlock=0;
		return 1;
	}
	else return 0;
}


//����pid�ƶ�ʹ�ã��빤���޹�
void temp_fsm(){
	static int state=0;
	switch (state){
		case 0:{
			set_target(1000,0);
			if(reach_target()) state++;
			break;
		}	
		case 1:{
			set_target(1000,1000);
			if(reach_target()) state++;
			break;
		}
		case 2:{
			set_target(0,0);
			if(reach_target()) state++;
			break;
		}
		case 3:
			break;
		
		default:
			break;
	}
}

void field_test_fsm(){
		static int state=0;
	switch (state){
		case 0:{
			set_target(300,600);
			if(reach_target()) state++;
			break;
		}	
		case 1:{
			set_target(300,1500);
			if(reach_target()) state++;
			break;
		}		
		case 2:{
			set_target(300,1650);
			if(reach_target()) state++;
			break;
		}				
		case 3:{
			set_target(300,1800);
			if(reach_target()) state++;
			break;
		}		
		case 4:{
			set_target(930,1800);
			if(reach_target()) state++;
			break;
		}				
		case 5:{
			set_target(1080,1800);
			if(reach_target()) state++;
			break;
		}	
		case 6:{
			set_target(1230,1800);
			if(reach_target()) state++;
			break;
		}		
		case 7:{
			set_target(1800,1200);
			if(reach_target()) state++;
			break;
		}	
		case 8:{
			set_target(1800,1050);
			if(reach_target()) state++;
			break;
		}		
		case 9:{
			set_target(1800,900);
			if(reach_target()) state++;
			break;
		}			
		case 10:{
			set_target(0,0);
			if(reach_target()) state++;
			break;
		}	
		default:
			break;
	}
}







