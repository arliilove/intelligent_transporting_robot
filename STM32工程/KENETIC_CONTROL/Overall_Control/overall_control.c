#include "overall_control.h"

//TODO:ĸ״̬������Ҫ����״̬����������ܽ�������Ȼ��״̬�����һ�����ᱻִ��
//����ɫ��˳�������Ϊ������
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
	//״̬��״̬Ҫȫ������
	
	//TODO:	Raspi_data.ena=0;
	
	//��ʱ************
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
//tarΪ�ļ���ͷ����ĵ�����֮һ��targetΪȫ�ֱ����������ó���Ŀ��λ��
	target.x=tar.x;
	target.y=tar.y;
	motorlock=1;//����ʱ���ж��������
}

//srcΪԭ��������task123����ɫһһ��Ӧ
void src_task1(void){
//src_task1��ɨ����ԭ��������֮��
	static u8 state=0;
	
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			push_target(src_target[position_which(1)]);
			//start_motion_group(2);//��е�۰ڷŵ�ԭ����׼��λ��   ��ʱע��
			//if(reach_target() && motion_fin(2)) state++;  ��ʱע��
			if(reach_target()) state++;
			break;
			//ǰ����һ��λ��
		}
		
		case 1:{
			//��������� ׼��λ��-ץ-׼��λ��
			if(round_cnt==1) start_motion_group(4);//�ϲ�
			else start_motion_group(5);//�²�
			state++;
			//���Ͷ���ź�
			break;
		}
		
		case 2:{
			if(motion_fin(4) || motion_fin(5)) {//���ץ����(׼��λ��-ץ-׼��λ��)
				push_target(src_target[position_which(2)]);
				//��������� ׼��λ��-������-׼��λ�ã�
				start_motion_group(6);
				//ǰ���ڶ���λ�ã����߱߷������
				state++;
			}
			break;
		}
		
		case 3:{
			//�ȴ����������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(6)) {state=0;  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);}
			break;
		}
		
		default:
			state=0;
			break;
	}
}

void src_task2(void){
//��ʱ�Ѿ��ڵڶ��������ǰ	
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//��������� ׼��λ��-ץ-׼��λ��
			if(round_cnt==1) start_motion_group(4);//�ϲ�
			else start_motion_group(5);//�²�
			state++;
			//���Ͷ���ź�
			break;			
		}
		
		case 1:{
			if(motion_fin(4) || motion_fin(5)) {//���ץ����(׼��λ��-ץ-׼��λ��)
				push_target(src_target[position_which(3)]);
				//��������� ׼��λ��-������-׼��λ�ã�
				start_motion_group(7);
				//ǰ��������λ�ã����߱߷������
				state++;
			}
			break;
		}
		
		case 2:{
			//�ȴ����������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(7)) state=0; 
			break;
		}
		
		default: 
			state=0;
			break;
	}
}

void src_task3(void){
//��ʱ���ڵ��������ǰ
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//��������� ׼��λ��-ץ-׼��λ��
			if(round_cnt==1) start_motion_group(4);//�ϲ�
			else start_motion_group(5);//�²�
			state++;
			//���Ͷ���ź�
			break;				
		}

		
		case 1:{//��Ҫɨ��ɫ���汾
			if(motion_fin(4) || motion_fin(5)) {//���ץ����(׼��λ��-ץ-׼��λ��)
				push_target(pro_middle);
				//��������� ׼��λ��-������-׼��λ�ã�
				start_motion_group(8);
				//ǰ���ּӹ����м�λ�ã����߱߷������
				state++;
			}
			break;			
		}
		
		case 2:{
			//�ȴ����������ִ�����(׼��λ��-�������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(8)) state=0; 
			//TODO:δ�Ż���������Ӱ������
			break;
		}		
	
		
/*
		case 1:{//����Ҫɨ��ɫ���汾
//TODO:
//			if(motion_fin() || motion_fin()) {//���ץ����(׼��λ��-ץ-׼��λ��)
				Raspi_data.left=3;//��
			  Raspi_data.middle=1;//��
			  Raspi_data.right=2;//��
				push_target(pro_target[position_which(1)]);
//				//TODO:��������� ׼��λ��-������-׼��λ�ã�
//				//start_motion_group();
//				//ǰ���ּӹ�����һ�����򣬱��߱߷������
//				state++;
//			}
			break;			
		}
		
		case 2:{
//TODO:			if(motion_fin()){//�ȴ����������ִ�����(׼��λ��-������-׼��λ��)
//				start_motion_group();//�ѻ�����ץ��׼��λ��
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
//��ʱӦ�õ���ԭ�����м�λ��
//״̬0������
//״̬1��ץ��һ����飬�ŵ�����
//״̬2��ץ�ڶ�����飬�ŵ�����
//״̬3: ץ���������
	static int state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
//		case 0:{
//			//TODO:if(reach_target() && motion_fin()) //Ӧ�ò����ж���
//			if(round_cnt==1){
//				Raspi_data.ena=0;
//				inform_src();//������ݮ������
//			}
//			state++;
//			break;
//		}	
		case 0:{
			if(Raspi_data.ena) src_task1();	//����ݮ��������
			//���������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ��state++
			if(reach_target() && motion_fin(6)) state++; 
			break;
		}
		case 1:{
			src_task2();
			//���������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ��state++
			if(reach_target() && motion_fin(7)) state++; 
			break;
		}
		
		case 2:{
			src_task3();
			//�ȴ����������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(8)) state=0; 
			break;
		}
		
		default:
			state=0;
			break;
	}
}



 //*********************************************�ӹ���
void pro_task1(void){
	//pro_task1�ڼӹ�������ɫ��֮��
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			start_motion_group(11);
			state++;//�ȴ����������ѻ�����ץ��׼��λ�ý����������յĻ�����֤reach_target()
			break;
		}
		
		case 1:{
			if(reach_target() && motion_fin(11)){
				//��������� ׼��λ��-��-׼��λ��
				start_motion_group(10);
				state++;
			}
			//���Ͷ���ź�
			break;
		}
		
		case 2:{
			if(motion_fin(10)) {//���ץ����(׼��λ��-��-׼��λ��)
				push_target(pro_target[position_which(2)]);
				//��������� ׼��λ��-������-׼��λ�ã�
				start_motion_group(12);
				//ǰ���ڶ���λ�ã����߱�ץ���
				state++;
			}
			break;
		}
		
		case 3:{
			//�ȴ����������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(12)) state=0; 
			break;
		}
		
		default:
			state=0;
			break;
	}
}

void pro_task2(void){
//��ʱ�Ѿ��ڵڶ���ɫ����ǰ	
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//��������� ׼��λ��-��-׼��λ��
			start_motion_group(10);
			state++;
			//���Ͷ���ź�
			break;			
		}
		
		case 1:{
			if(motion_fin(10)) {//���������(׼��λ��-��-׼��λ��)
				push_target(pro_target[position_which(3)]);
				//��������� ׼��λ��-������-׼��λ�ã�
				start_motion_group(13);
				//ǰ��������λ�ã����߱�ץ���
				state++;
			}
			break;
		}
		
		case 2:{
			//�ȴ����������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(13)) state=0; 
			break;
		}
		
		default: 
			state=0;
			break;
	}
}

void pro_task3(void){
//��ʱ���ڵ�����ɫ��ǰ
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//��������� ׼��λ��-��-׼��λ��
			start_motion_group(10);
			state++;
			//���Ͷ���ź�
			break;				
		}
		
		case 1:{
			if(motion_fin(10)) {//���ץ����(׼��λ��-��-׼��λ��)
				push_target(pro_target[position_which(1)]);
				//start_motion_group(14);
				//ǰ���ּӹ�����һ������׼�������������
				state++;
			}
			break;			
		}
		
		case 2:{
			//�ȴ��ƶ���λ����ʱ�޶������
			if(reach_target()) state=0; 
			break;
		}
		
		default:
			state=0;
			break;
	}	
}

void pro_task4(void){
//��ʱ�ѻص���һ��ɫ��ǰ
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//��������� ׼��λ��-ץ�ּӹ�-׼��λ��
			start_motion_group(14);
			state++;
			//���Ͷ���ź�
			break;			
		}
		
		case 1:{
			if(motion_fin(14)) {//���ץ����(׼��λ��-ץ�ּӹ�-׼��λ��)
				push_target(pro_target[position_which(2)]);
				//��������� ׼��λ��-������-׼��λ�ã�
				start_motion_group(15);
				//ǰ���ڶ���λ�ã����߱߷����
				state++;
			}
			break;
		}
		
		case 2:{
			//�ȴ����������ִ�����(׼��λ��-�����ҷ�-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(15)) state=0; 
			break;
		}
		
		default: 
			state=0;
			break;
	}
}

void pro_task5(void){
//��ʱ�ѻص��ڶ���ɫ��ǰ
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//��������� ׼��λ��-ץ�ּӹ�-׼��λ��
			start_motion_group(14);
			state++;
			//���Ͷ���ź�
			break;			
		}
		
		case 1:{
			if(motion_fin(14)) {//���ץ����(׼��λ��-ץ�ּӹ�-׼��λ��)
				push_target(pro_target[position_which(3)]);
				//��������� ׼��λ��-������-׼��λ�ã�
				start_motion_group(16);
				//ǰ���ڶ���λ�ã����߱߷����
				state++;
			}
			break;
		}
		
		case 2:{
			//�ȴ����������ִ�����(׼��λ��-�����з�-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(16)) state=0; 
			break;
		}
		
		default: 
			state=0;
			break;
	}
}

void pro_task6(void){
//��ʱ�ѻص�������ɫ��ǰ
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//��������� ׼��λ��-ץ�ּӹ�-׼��λ��
			start_motion_group(14);
			state++;
			//���Ͷ���ź�
			break;			
		}

		
		case 1:{//��Ҫ���հ汾
			if(motion_fin(14)) {//���ץ����(׼��λ��-ץ�ּӹ�-׼��λ��)
				push_target(fin_middle);
				//��������� ׼��λ��-�������-׼��λ�ã�
				start_motion_group(17);
				//ǰ�����Ʒ�������߱߷����
				state++;
			}
			break;
		}
		
		case 2:{
			//�ȴ����������ִ�����(׼��λ��-�������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(17)) state=0; 
			break;
		}

/*		
		case 1:{//����Ҫ���հ汾
//TODO:
//			if(motion_fin()) {//���ץ����(׼��λ��-ץ�ּӹ�-׼��λ��)
//				push_target(fin_target[position_which(1)]);
//				//TODO:��������� ׼��λ��-�������-׼��λ�ã�
//				//start_motion_group();
//				//ǰ�����Ʒ�������߱߷����
//				state++;
//			}
			break;
		}		

		case 2:{
			//�ȴ����������ִ�����(׼��λ��-�������-׼��λ��)
//			if(motion_fin()) {
//				start_motion_group();//�ѻ�����ץ��׼��λ��
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
//��ʱӦ�õ���ӹ�����һ����
	static int state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//if(reach_target() && motion_fin()) //Ӧ�ò����ж���
			//inform_pro();//������ݮ�����ա��������Ҫ������ֱ���������״̬   ��ʱע��
			state++;
			break;
		}	
		case 1:{
			if(Raspi_data.ena) {
				pro_task1();	
			}
			//Protask1���������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ��state++
			if(reach_target() && motion_fin(12)) state++; 
			break;
		}
		case 2:{
			pro_task2();
			//���������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ��state++
			if(reach_target() && motion_fin(13)) state++; 
			break;
		}
		
		case 3:{
			pro_task3();
			//�ȴ��ƶ���λ
			if(reach_target() && motion_fin(10)) state++;
			break;
		}
		
		case 4:{
			pro_task4();
			//�ȴ����������ִ�����(׼��λ��-�����ҷ�-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(15)) state++; 
			break;			
		}
		
		case 5:{
			pro_task5();
			//�ȴ����������ִ�����(׼��λ��-�����з�-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(16)) state++; 
			break;
		}
		
		case 6:{
			pro_task6();
			//�ȴ����������ִ�����(׼��λ��-�������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(17)) state=0; 
		}
		
		default:
			state=0;
			break;
	}
}



//***********************************************���Ʒ��
void fin_task1(void){
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			start_motion_group(21);
			//�ȴ���������飺�ѻ�����ץ��׼��λ��
			if(reach_target() && motion_fin(21)) state++;
			break;
		}
		
		case 1:{
			//��������� ׼��λ��-�Ű��Ʒ��-׼��λ��
			if(round_cnt==1) start_motion_group(20);//����
			else start_motion_group(24);//���
			state++;
			//���Ͷ���ź�
			break;
		}
		
		case 2:{
			if(motion_fin(20) || motion_fin(24)) {//���ץ����(׼��λ��-�Ű��Ʒ��-׼��λ�ã�
				push_target(fin_target[position_which(2)]);
				//��������� ׼��λ��-������ץ-׼��λ�ã�
				start_motion_group(22);
				//ǰ���ڶ���λ�ã����߱�ץ���
				state++;
			}
			break;
		}
		
		case 3:{
			//�ȴ����������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(22)) state=0; 
			break;
		}
		
		default:
			state=0;
			break;
	}
}

void fin_task2(void){
//��ʱ�Ѿ��ڵڶ���������ǰ	
	static u8 state=0;
	static u8 temp_i=0;
	if(temp_i==0 && round_cnt==2) {//�ڶ�Ȧʱȷ�����״̬��״̬
		temp_i=1;
		state=0;
	}
	
	switch (state){
		case 0:{
			//��������� ׼��λ��-��-׼��λ��
			if(round_cnt==1) start_motion_group(20);//����
			else start_motion_group(24);//���
			state++;
			//���Ͷ���ź�
			break;			
		}
		
		case 1:{
			if(motion_fin(20) || motion_fin(24)) {//���������(׼��λ��-��-׼��λ��)
				push_target(fin_target[position_which(3)]);
				//��������� ׼��λ��-������-׼��λ�ã�
				start_motion_group(23);
				//ǰ��������λ�ã����߱�ץ���
				state++;
			}
			break;
		}
		
		case 2:{
			//�ȴ����������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(23)) state=0; 
			break;
		}
		
		default: 
			state=0;
			break;
	}
}


void fin_task3(void){
//��ʱ���ڵ���������ǰ
	static u8 state=0;
	switch (state){
		case 0:{
			//��������� ׼��λ��-�Ű��Ʒ��-׼��λ��
			if(round_cnt==1) start_motion_group(20);//����
			else start_motion_group(24);//���
			state++;
			//���Ͷ���ź�
			break;				
		}
		
		case 1:{
			if(motion_fin(20) || motion_fin(24)) {//���������(׼��λ��-��-׼��λ��)
				if(round_cnt==1) {
					round_cnt=2;
					state=0;
				}
				else if(round_cnt==2) {
					push_target(destination);//�ڶ�Ȧ����ֱ�ӻ�ԭ��
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
//��ʱ�ƶ������Ʒ���м�λ��	
	static int state=0;
	switch (state){
		case 0:{
			//TODO:if(reach_target() && motion_fin()) //Ӧ�ò����ж���
			//inform_fin();//������ݮ�����ա��������Ҫ������ֱ���������״̬   ��ʱע��
			state++;
			break;
		}	
		case 1:{
			if(Raspi_data.ena) fin_task1();	
			//Protask1���������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ��state++
			if(reach_target() && motion_fin(22)) state++; 
			break;
		}
		
		case 2:{
			fin_task2();
			//�ȴ����������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(23)) state++; 
			break;
		}
		
		case 3:{
			fin_task3();
			if(target.x!=destination.x && target.y!=destination.y){
				if(round_cnt==2) state=0;
			}
			else{//���targetΪ�յ�
				if(reach_dest())state=0;
			}
			
			break;
		}
		
		default:
			state=0;
			break;
	}
}



//*********************************************����״̬����
void run(){
//�˺���Ӧ���ڰ��¿�ʼ���󣬶����ǰ��¸�λ����
//��������ʼ���Ͷ����ʼ��̬��λӦ���ڸ�λ���Ϳ�ʼ��֮�����
	
	static int state=0;
	switch(state){
		case 0:{
			run_init();
			//��е��ת������ǰ����������ͷ����
			start_motion_group(1); 
			push_target(taskboard);
//			if(Raspi_data.ena) {//����ͷ���ϵ翪ʼ�Ϳ�ʼ����ɨ���ά�룬��˲���Ҫ����Ϣ����   ��ʱע��
//				push_target(src_middle);
//				state++;
//			}
			state++;
			break;
		}
		
		case 1:{
			if(reach_target()){
				//inform_src();//�Ὣena����,��ʼ�������   ��ʱע��
				state++;
			}
			break;
		}
		
		case 2:{
			if(Raspi_data.ena) do_src_task();
			//�ȴ����������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(8)) state++; 
			break;
		}
		
		case 3:{
			do_pro_task();
			//�ȴ����������ִ�����(׼��λ��-�������-׼��λ��)���ƶ���λ ���ּӹ�����׼��λ�ã�����һ״̬����ͬһ�����飩
			if(reach_target() && motion_fin(17)) state++; 
			break;
		}
		
		case 4:{
			do_fin_task();
			if(round_cnt==2) state++;//��һȦִ����ϣ���ʱ�ڰ��Ʒ�����һ�����򣬶�����ִ�����
			break;
		}
		
		case 5:{//����״̬	
				Raspi_data.left=Raspi_data.lowerleft;
				Raspi_data.middle=Raspi_data.lowermiddle;
				Raspi_data.right=Raspi_data.lowerright;
				Raspi_data.task1=Raspi_data.t4;
				Raspi_data.task2=Raspi_data.t5;
				Raspi_data.task3=Raspi_data.t6;
			
				push_target(src_target[position_which(1)]);//ǰ��ԭ�����²�ĵ�һ��
				if(reach_target())state++;
		}
		
		case 6:{
			do_src_task();
			//�ȴ����������ִ�����(׼��λ��-������-׼��λ��)���ƶ���λ
			if(reach_target() && motion_fin(8)) state++; 
			break;
		}
		
		case 7:{
			do_pro_task();
			//�ȴ����������ִ�����(׼��λ��-�������-׼��λ��)���ƶ���λ ���ּӹ�����׼��λ�ã�����һ״̬����ͬһ�����飩
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


