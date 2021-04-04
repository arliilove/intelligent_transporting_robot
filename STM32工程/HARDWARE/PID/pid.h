#ifndef PID_H
#define PID_H
#include "can.h"
#include "sys.h"
#include "encoder.h"
#include "math.h"
#include "float.h"
#include "lcd.h"
#define MAX_ACC 2.5  //��һ����ʱ���жϣ�0.1s���� motorset����������
#define MAX_SPEED 150
#define STARTUP 50
extern Motorstate motorstate;
extern Sensorstate sensorstate;
extern s16 motorset[4];
extern OPS_ENCODER OPS_Encoder;

typedef struct TARGET{
	float x;
	float y;
}Target;

extern Target target;

void yaw_correction(void);
void pid_distance(void);

#endif

