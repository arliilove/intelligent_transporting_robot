#include "overall_control.h"

//TODO:母状态机必须要在子状态机结束后才能结束！不然子状态机最后一步不会被执行
//场地色环顺序从左到右为蓝红绿
u8 round_cnt=1;

const Target origin={0,0};
const Target taskboard={300,600};
const Target src_right={300,1050};
const Target src_middle={300,1200};
const Target src_left={300,1350};
const Target pro_right={900,1600};
const Target pro_middle={1050,1600};
const Target pro_left={1200,1600};
const Target fin_right={1600,1200};
const Target fin_middle={1600,1050};
const Target fin_left={1600,900};
const Target destination={2050,2050};

Target src_target[3];
Target pro_target[3];
Target fin_target[3];


void run_init(){

	src_target[0]=src_left;src_target[1]=src_middle;src_target[2]=src_right;
	pro_target[0]=pro_left;pro_target[1]=pro_middle;pro_target[2]=pro_right;
	fin_target[0]=fin_left;fin_target[1]=fin_middle;fin_target[2]=fin_right;
	//状态机状态要全部归零
	
	//TODO:	Raspi_data.ena=0;
	
	//临时************
	Raspi_data.ena=1;
	Raspi_data.t1=1;
	Raspi_data.t2=2;
	Raspi_data.t3=3;
	Raspi_data.t4=3;
	Raspi_data.t5=2;
	Raspi_data.t6=1;	

	Raspi_data.task1=1;
	Raspi_data.task2=2;
	Raspi_data.task3=3;
	
	Raspi_data.upperleft=2;
					Raspi_data.uppermiddle=1;
					Raspi_data.upperright=3;
					Raspi_data.lowerleft=3;
					Raspi_data.lowermiddle=2;
					Raspi_data.lowerright=1;
					
					Raspi_data.left=Raspi_data.upperleft;
					Raspi_data.middle=Raspi_data.uppermiddle;
					Raspi_data.right=Raspi_data.upperright;
	//******
	round_cnt=1;
}
void push_target(Target tar){
//tar为文件开头保存的点坐标之一，target为全局变量用来设置车的目标位置
	target.x=tar.x;
	target.y=tar.y;
	motorlock=1;//允许定时器中断启动电机
}

//src为原料区任务。task123与颜色一一对应
void src_task1(void){
//src_task1在扫描完原料区货架之后
	static u8 state=0;
	
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			push_target(src_target[position_which(1)]);
			//start_motion_group(2);//机械臂摆放到原料区准备位置   临时注释
			//if(reach_target() && motion_fin(2)) state++;  临时注释
			if(reach_target()) state++;
			break;
			//前往第一个位置
		}
		
		case 1:{
			//舵机动作组 准备位置-抓-准备位置
			if(round_cnt==1) start_motion_group(4);//上层
			else start_motion_group(5);//下层
			state++;
			//发送舵机信号
			break;
		}
		
		case 2:{
			if(motion_fin(4) || motion_fin(5)) {//舵机抓完了(准备位置-抓-准备位置)
				push_target(src_target[position_which(2)]);
				//舵机动作组 准备位置-货仓右-准备位置；
				start_motion_group(6);
				//前往第二个位置，边走边放置物块
				state++;
			}
			break;
		}
		
		case 3:{
			//等待舵机动作组执行完毕(准备位置-货仓右-准备位置)且移动到位
			if(reach_target() && motion_fin(6)) {state=0;  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);}
			break;
		}
		
		default:
			state=0;
			break;
	}
}

void src_task2(void){
//此时已经在第二个物块面前	
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//舵机动作组 准备位置-抓-准备位置
			if(round_cnt==1) start_motion_group(4);//上层
			else start_motion_group(5);//下层
			state++;
			//发送舵机信号
			break;			
		}
		
		case 1:{
			if(motion_fin(4) || motion_fin(5)) {//舵机抓完了(准备位置-抓-准备位置)
				push_target(src_target[position_which(3)]);
				//舵机动作组 准备位置-货仓中-准备位置；
				start_motion_group(7);
				//前往第三个位置，边走边放置物块
				state++;
			}
			break;
		}
		
		case 2:{
			//等待舵机动作组执行完毕(准备位置-货仓中-准备位置)且移动到位
			if(reach_target() && motion_fin(7)) state=0; 
			break;
		}
		
		default: 
			state=0;
			break;
	}
}

void src_task3(void){
//此时已在第三个物块前
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//舵机动作组 准备位置-抓-准备位置
			if(round_cnt==1) start_motion_group(4);//上层
			else start_motion_group(5);//下层
			state++;
			//发送舵机信号
			break;				
		}

		
		case 1:{//需要扫描色环版本
			if(motion_fin(4) || motion_fin(5)) {//舵机抓完了(准备位置-抓-准备位置)
				push_target(pro_middle);
				//舵机动作组 准备位置-货仓左-准备位置；
				start_motion_group(8);
				//前往粗加工区中间位置，边走边放置物块
				state++;
			}
			break;			
		}
		
		case 2:{
			//等待舵机动作组执行完毕(准备位置-货仓左放-准备位置)且移动到位
			if(reach_target() && motion_fin(8)) state=0; 
			//TODO:未优化，但不会影响拍照
			break;
		}		
	
		
/*
		case 1:{//不需要扫描色环版本
//TODO:
//			if(motion_fin() || motion_fin()) {//舵机抓完了(准备位置-抓-准备位置)
				Raspi_data.left=3;//蓝
			  Raspi_data.middle=1;//红
			  Raspi_data.right=2;//绿
				push_target(pro_target[position_which(1)]);
//				//TODO:舵机动作组 准备位置-货仓左-准备位置；
//				//start_motion_group();
//				//前往粗加工区第一个区域，边走边放置物块
//				state++;
//			}
			break;			
		}
		
		case 2:{
//TODO:			if(motion_fin()){//等待舵机动作组执行完毕(准备位置-货仓左-准备位置)
//				start_motion_group();//把货仓右抓起到准备位置
//				if(reach_target()) state=0;
//			}
			break;
		}
*/

		default:
			state=0;
			break;
	}
}


void (*src_task[3])(void)={src_task1,src_task2,src_task3};
//(*src_task[task1])();	

void do_src_task(){
//此时应该到达原料区中间位置
//状态0：拍照
//状态1：抓第一个物块，放到货仓
//状态2：抓第二个物块，放到货仓
//状态3: 抓第三个物块
	static int state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
//		case 0:{
//			//TODO:if(reach_target() && motion_fin()) //应该不用判断了
//			if(round_cnt==1){
//				Raspi_data.ena=0;
//				inform_src();//告诉树莓派拍照
//			}
//			state++;
//			break;
//		}	
		case 0:{
			if(Raspi_data.ena) src_task1();	//等树莓派拍完照
			//舵机动作组执行完毕(准备位置-货仓右-准备位置)且移动到位，state++
			if(reach_target() && motion_fin(6)) state++; 
			break;
		}
		case 1:{
			src_task2();
			//舵机动作组执行完毕(准备位置-货仓中-准备位置)且移动到位，state++
			if(reach_target() && motion_fin(7)) state++; 
			break;
		}
		
		case 2:{
			src_task3();
			//等待舵机动作组执行完毕(准备位置-货仓左-准备位置)且移动到位
			if(reach_target() && motion_fin(8)) state=0; 
			break;
		}
		
		default:
			state=0;
			break;
	}
}



 //*********************************************加工区
void pro_task1(void){
	//pro_task1在加工区拍完色环之后
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			start_motion_group(11);
			state++;//等待舵机动作组把货仓右抓起到准备位置结束。若拍照的话需验证reach_target()
			break;
		}
		
		case 1:{
			if(reach_target() && motion_fin(11)){
				//舵机动作组 准备位置-放-准备位置
				start_motion_group(10);
				state++;
			}
			//发送舵机信号
			break;
		}
		
		case 2:{
			if(motion_fin(10)) {//舵机抓完了(准备位置-放-准备位置)
				push_target(pro_target[position_which(2)]);
				//舵机动作组 准备位置-货仓中-准备位置；
				start_motion_group(12);
				//前往第二个位置，边走边抓物块
				state++;
			}
			break;
		}
		
		case 3:{
			//等待舵机动作组执行完毕(准备位置-货仓中-准备位置)且移动到位
			if(reach_target() && motion_fin(12)) state=0; 
			break;
		}
		
		default:
			state=0;
			break;
	}
}

void pro_task2(void){
//此时已经在第二个色环面前	
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//舵机动作组 准备位置-放-准备位置
			start_motion_group(10);
			state++;
			//发送舵机信号
			break;			
		}
		
		case 1:{
			if(motion_fin(10)) {//舵机放完了(准备位置-放-准备位置)
				push_target(pro_target[position_which(3)]);
				//舵机动作组 准备位置-货仓左-准备位置；
				start_motion_group(13);
				//前往第三个位置，边走边抓物块
				state++;
			}
			break;
		}
		
		case 2:{
			//等待舵机动作组执行完毕(准备位置-货仓左-准备位置)且移动到位
			if(reach_target() && motion_fin(13)) state=0; 
			break;
		}
		
		default: 
			state=0;
			break;
	}
}

void pro_task3(void){
//此时已在第三个色环前
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//舵机动作组 准备位置-放-准备位置
			start_motion_group(10);
			state++;
			//发送舵机信号
			break;				
		}
		
		case 1:{
			if(motion_fin(10)) {//舵机抓完了(准备位置-放-准备位置)
				push_target(pro_target[position_which(1)]);
				//start_motion_group(14);
				//前往粗加工区第一个任务，准备把物块拿起来
				state++;
			}
			break;			
		}
		
		case 2:{
			//等待移动到位，此时无舵机动作
			if(reach_target()) state=0; 
			break;
		}
		
		default:
			state=0;
			break;
	}	
}

void pro_task4(void){
//此时已回到第一个色环前
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//舵机动作组 准备位置-抓粗加工-准备位置
			start_motion_group(14);
			state++;
			//发送舵机信号
			break;			
		}
		
		case 1:{
			if(motion_fin(14)) {//舵机抓完了(准备位置-抓粗加工-准备位置)
				push_target(pro_target[position_which(2)]);
				//舵机动作组 准备位置-货仓右-准备位置；
				start_motion_group(15);
				//前往第二个位置，边走边放物块
				state++;
			}
			break;
		}
		
		case 2:{
			//等待舵机动作组执行完毕(准备位置-货仓右放-准备位置)且移动到位
			if(reach_target() && motion_fin(15)) state=0; 
			break;
		}
		
		default: 
			state=0;
			break;
	}
}

void pro_task5(void){
//此时已回到第二个色环前
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//舵机动作组 准备位置-抓粗加工-准备位置
			start_motion_group(14);
			state++;
			//发送舵机信号
			break;			
		}
		
		case 1:{
			if(motion_fin(14)) {//舵机抓完了(准备位置-抓粗加工-准备位置)
				push_target(pro_target[position_which(3)]);
				//舵机动作组 准备位置-货仓中-准备位置；
				start_motion_group(16);
				//前往第二个位置，边走边放物块
				state++;
			}
			break;
		}
		
		case 2:{
			//等待舵机动作组执行完毕(准备位置-货仓中放-准备位置)且移动到位
			if(reach_target() && motion_fin(16)) state=0; 
			break;
		}
		
		default: 
			state=0;
			break;
	}
}

void pro_task6(void){
//此时已回到第三个色环前
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//舵机动作组 准备位置-抓粗加工-准备位置
			start_motion_group(14);
			state++;
			//发送舵机信号
			break;			
		}

		
		case 1:{//需要拍照版本
			if(motion_fin(14)) {//舵机抓完了(准备位置-抓粗加工-准备位置)
				push_target(fin_middle);
				//舵机动作组 准备位置-货仓左放-准备位置；
				start_motion_group(17);
				//前往半成品区，边走边放物块
				state++;
			}
			break;
		}
		
		case 2:{
			//等待舵机动作组执行完毕(准备位置-货仓左放-准备位置)且移动到位
			if(reach_target() && motion_fin(17)) state=0; 
			break;
		}

/*		
		case 1:{//不需要拍照版本
//TODO:
//			if(motion_fin()) {//舵机抓完了(准备位置-抓粗加工-准备位置)
//				push_target(fin_target[position_which(1)]);
//				//TODO:舵机动作组 准备位置-货仓左放-准备位置；
//				//start_motion_group();
//				//前往半成品区，边走边放物块
//				state++;
//			}
			break;
		}		

		case 2:{
			//等待舵机动作组执行完毕(准备位置-货仓左放-准备位置)
//			if(motion_fin()) {
//				start_motion_group();//把货仓右抓起到准备位置
//				if(reach_target()) state=0;
//			}
			break;
		}
*/		
		default: 
			state=0;
			break;
	}
}


void do_pro_task(){
//此时应该到达加工区第一个点
	static int state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//if(reach_target() && motion_fin()) //应该不用判断了
			//inform_pro();//告诉树莓派拍照。如果不需要拍照则直接跳过这个状态   临时注释
			state++;
			break;
		}	
		case 1:{
			if(Raspi_data.ena) {
				pro_task1();	
			}
			//Protask1舵机动作组执行完毕(准备位置-货仓中-准备位置)且移动到位，state++
			if(reach_target() && motion_fin(12)) state++; 
			break;
		}
		case 2:{
			pro_task2();
			//舵机动作组执行完毕(准备位置-货仓左-准备位置)且移动到位，state++
			if(reach_target() && motion_fin(13)) state++; 
			break;
		}
		
		case 3:{
			pro_task3();
			//等待移动到位
			if(reach_target() && motion_fin(10)) state++;
			break;
		}
		
		case 4:{
			pro_task4();
			//等待舵机动作组执行完毕(准备位置-货仓右放-准备位置)且移动到位
			if(reach_target() && motion_fin(15)) state++; 
			break;			
		}
		
		case 5:{
			pro_task5();
			//等待舵机动作组执行完毕(准备位置-货仓中放-准备位置)且移动到位
			if(reach_target() && motion_fin(16)) state++; 
			break;
		}
		
		case 6:{
			pro_task6();
			//等待舵机动作组执行完毕(准备位置-货仓左放-准备位置)且移动到位
			if(reach_target() && motion_fin(17)) state=0; 
		}
		
		default:
			state=0;
			break;
	}
}



//***********************************************半成品区
void fin_task1(void){
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			start_motion_group(21);
			//等待舵机动作组：把货仓右抓起到准备位置
			if(reach_target() && motion_fin(21)) state++;
			break;
		}
		
		case 1:{
			//舵机动作组 准备位置-放半成品区-准备位置
			if(round_cnt==1) start_motion_group(20);//地面
			else start_motion_group(24);//码放
			state++;
			//发送舵机信号
			break;
		}
		
		case 2:{
			if(motion_fin(20) || motion_fin(24)) {//舵机抓完了(准备位置-放半成品区-准备位置）
				push_target(fin_target[position_which(2)]);
				//舵机动作组 准备位置-货仓中抓-准备位置；
				start_motion_group(22);
				//前往第二个位置，边走边抓物块
				state++;
			}
			break;
		}
		
		case 3:{
			//等待舵机动作组执行完毕(准备位置-货仓中-准备位置)且移动到位
			if(reach_target() && motion_fin(22)) state=0; 
			break;
		}
		
		default:
			state=0;
			break;
	}
}

void fin_task2(void){
//此时已经在第二个货架面前	
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//第二圈时确保清空状态机状态
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//舵机动作组 准备位置-放-准备位置
			if(round_cnt==1) start_motion_group(20);//地面
			else start_motion_group(24);//码放
			state++;
			//发送舵机信号
			break;			
		}
		
		case 1:{
			if(motion_fin(20) || motion_fin(24)) {//舵机放完了(准备位置-放-准备位置)
				push_target(fin_target[position_which(3)]);
				//舵机动作组 准备位置-货仓左-准备位置；
				start_motion_group(23);
				//前往第三个位置，边走边抓物块
				state++;
			}
			break;
		}
		
		case 2:{
			//等待舵机动作组执行完毕(准备位置-货仓左-准备位置)且移动到位
			if(reach_target() && motion_fin(23)) state=0; 
			break;
		}
		
		default: 
			state=0;
			break;
	}
}


void fin_task3(void){
//此时已在第三个货架前
	static u8 state=0;
	switch (state){
		case 0:{
			//舵机动作组 准备位置-放半成品区-准备位置
			if(round_cnt==1) start_motion_group(20);//地面
			else start_motion_group(24);//码放
			state++;
			//发送舵机信号
			break;				
		}
		
		case 1:{
			if(motion_fin(20) || motion_fin(24)) {//舵机放完了(准备位置-放-准备位置)
				if(round_cnt==1) {
					round_cnt=2;
					state=0;
				}
				else if(round_cnt==2) {
					push_target(destination);//第二圈跑完直接回原点
					if(reach_dest()) state=0;
				}
			}
			break;			
		}
		
		default:
			state=0;
			break;
	}	
}

void do_fin_task(){
//此时移动到半成品区中间位置	
	static int state=0;
	switch (state){
		case 0:{
			//TODO:if(reach_target() && motion_fin()) //应该不用判断了
			//inform_fin();//告诉树莓派拍照。如果不需要拍照则直接跳过这个状态   临时注释
			state++;
			break;
		}	
		case 1:{
			if(Raspi_data.ena) fin_task1();	
			//Protask1舵机动作组执行完毕(准备位置-货仓中-准备位置)且移动到位，state++
			if(reach_target() && motion_fin(22)) state++; 
			break;
		}
		
		case 2:{
			fin_task2();
			//等待舵机动作组执行完毕(准备位置-货仓左-准备位置)且移动到位
			if(reach_target() && motion_fin(23)) state++; 
			break;
		}
		
		case 3:{
			fin_task3();
			if(target.x!=destination.x && target.y!=destination.y){
				if(round_cnt==2) state=0;
			}
			else{//如果target为终点
				if(reach_dest())state=0;
			}
			
			break;
		}
		
		default:
			state=0;
			break;
	}
}



//*********************************************超级状态机儿
void run(){
//此函数应当在按下开始键后，而不是按下复位键后
//编码器初始化和舵机初始姿态复位应当在复位键和开始键之间完成
	
	static int state=0;
	switch(state){
		case 0:{
			run_init();
			//机械臂转动到正前方方便摄像头拍照
			start_motion_group(1); 
			push_target(taskboard);
//			if(Raspi_data.ena) {//摄像头从上电开始就开始不断扫描二维码，因此不需要发消息启动   临时注释
//				push_target(src_middle);
//				state++;
//			}
			state++;
			break;
		}
		
		case 1:{
			if(reach_target()){
				//inform_src();//会将ena归零,开始拍摄货架   临时注释
				state++;
			}
			break;
		}
		
		case 2:{
			if(Raspi_data.ena) do_src_task();
			//等待舵机动作组执行完毕(准备位置-货仓左-准备位置)且移动到位
			if(reach_target() && motion_fin(8)) state++; 
			break;
		}
		
		case 3:{
			do_pro_task();
			//等待舵机动作组执行完毕(准备位置-货仓左放-准备位置)且移动到位 （粗加工区的准备位置，和上一状态不是同一动作组）
			if(reach_target() && motion_fin(17)) state++; 
			break;
		}
		
		case 4:{
			do_fin_task();
			if(round_cnt==2) state++;//第一圈执行完毕，此时在半成品区最后一个区域，动作组执行完毕
			break;
		}
		
		case 5:{//过渡状态	
				Raspi_data.left=Raspi_data.lowerleft;
				Raspi_data.middle=Raspi_data.lowermiddle;
				Raspi_data.right=Raspi_data.lowerright;
				Raspi_data.task1=Raspi_data.t4;
				Raspi_data.task2=Raspi_data.t5;
				Raspi_data.task3=Raspi_data.t6;
			
				push_target(src_target[position_which(1)]);//前往原料区下层的第一个
				if(reach_target())state++;
		}
		
		case 6:{
			do_src_task();
			//等待舵机动作组执行完毕(准备位置-货仓左-准备位置)且移动到位
			if(reach_target() && motion_fin(8)) state++; 
			break;
		}
		
		case 7:{
			do_pro_task();
			//等待舵机动作组执行完毕(准备位置-货仓左放-准备位置)且移动到位 （粗加工区的准备位置，和上一状态不是同一动作组）
			if(reach_target() && motion_fin(17)) state++; 
			break;
		}

		case 8:{
			do_fin_task();
			if(reach_dest()) state=0;
			break;
		}
		
		default:
			state=0;
			break;
	}
}


