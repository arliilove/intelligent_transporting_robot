#include "underpan_control.h"
#include <stdio.h>
#include <stdlib.h>
	//1.set_target,执行完后会立刻开始移动
	//2.移动的同时转动机械臂
	//3.到达之后启动机械臂动作组（如何判定到达）
	//4.抓起第三个物块或放下第三个物块后set_target下一个目的地，同时机械臂将最后一个货物放入货仓或归位。货仓与颜色一一对应
	//  是否将机械臂归位写入下一个task
int motorlock=0;

void set_target(float x, float y){
	target.x=x;target.y=y;
	motorlock=1;//允许定时器中断启动电机
}

int reach_target(){
	//如果到达目标，关闭电机，返回1
	
	if(fabs(OPS_Encoder.pos_x-target.x)<4 && fabs(OPS_Encoder.pos_y-target.y)<4 && fabs(OPS_Encoder.zangle)<2 && motorstate.speed[0]<4 && motorstate.speed[1]<4 && motorstate.speed[2]<4 && motorstate.speed[3]<4){//判断条件
		motorlock=0;
		return 1;
	}
	else return 0;
}

int reach_origin(){
//回原点专用，条件更宽泛
	if((fabs(OPS_Encoder.pos_x-target.x)<4 && fabs(OPS_Encoder.pos_y-target.y)<4 && fabs(OPS_Encoder.zangle)<3 && motorstate.speed[0]<5 && motorstate.speed[1]<5 && motorstate.speed[2]<5 && motorstate.speed[3]<5) || (OPS_Encoder.pos_x<0 && OPS_Encoder.pos_y<0)){//判断条件
		motorlock=0;
		return 1;
	}
	else return 0;
}

int reach_dest(){
//回原点专用，条件更宽泛
	if((fabs(OPS_Encoder.pos_x-target.x)<4 && fabs(OPS_Encoder.pos_y-target.y)<4 && fabs(OPS_Encoder.zangle)<3 && motorstate.speed[0]<5 && motorstate.speed[1]<5 && motorstate.speed[2]<5 && motorstate.speed[3]<5) || (OPS_Encoder.pos_x<0 && OPS_Encoder.pos_y<0)){//判断条件
		motorlock=0;
		return 1;
	}
	else return 0;
}


//测试pid移动使用，与工程无关
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







