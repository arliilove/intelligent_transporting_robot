#ifndef _UNDERPAN
#define _UNDERPAN

#include "sys.h"
#include "encoder.h"
#include "pid.h"

extern OPS_ENCODER OPS_Encoder;
extern Target target;
extern int motorlock;//当motorlock=1时定时器中断中速度设定才有效

void set_target(float x, float y);
int reach_target(void);
int reach_origin(void);
int reach_dest(void);
void temp_fsm(void);
void field_test_fsm(void);



#endif

