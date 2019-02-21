#ifndef	__KALMAN_H
#define __KALMAN_H

//¿¨¶ûÂüÂË²¨²ÎÊý
#define q_hum 1.0					
#define r_hum	3.0
#define q_tem	0.0001
#define r_tem	0.05
#define q_press 0.1
#define r_press 0.1

typedef struct
{
	float x;		//state
	float A;		//x(n)=A*x(n-1)+u(n),u(n)~N(0,q)
	float H;		//z(n)=H*x(n)+w(n),w(n)~N(0,r)
	float q;		//process(predict) noise convariance
	float r;		//measure noise convariance
	float p;		//estimated error convariance
	float gain;	//kalman gain
	float predict;
}KALMAN;
extern KALMAN kalman_tem;
extern KALMAN kalman_hum;
void kalman1_init(KALMAN *state, float init_x, float init_p);
float kalman1_filter(KALMAN *state, float z_measure);

#endif

