#ifndef _UNDERPAN
#define _UNDERPAN

#include "sys.h"
#include "encoder.h"
#include "pid.h"

extern OPS_ENCODER OPS_Encoder;
extern Target target;
extern int motorlock;//��motorlock=1ʱ��ʱ���ж����ٶ��趨����Ч

void set_target(float x, float y);
int reach_target(void);
int reach_origin(void);
int reach_dest(void);
void temp_fsm(void);
void field_test_fsm(void);



#endif

