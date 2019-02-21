/**
  ******************************************************************************
  * @file    kalman.c 
  * @author  DPL
  * @version V1.0.0
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * 										COPYRIGHT 2011 STMicroelectronics
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "kalman.h"
KALMAN kalman_tem = {0,0,0,q_tem,r_tem,0,0,0};
KALMAN kalman_hum = {0,0,0,q_hum,r_hum,0,0,0};

/**
  * @brief kalmanÂË²¨Ëã·¨
  * @param  
						
  * @retval None
  */
void kalman1_init(KALMAN *state, float init_x, float init_p)
{
	state->x = init_x;
	state->p = init_p;
	state->A = 1;
	state->H = 1;
}
	
//1 dimension kalman filter
float kalman1_filter(KALMAN *state, float z_measure)
{
	/* Predict */
	state->x = state->A * state->x;
	state->p = state->A * state->A * state->p + state->q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

	/* Measurement */
	state->gain = state->p * state->H / (state->p * state->H * state->H + state->r);
	state->x = state->x + state->gain * (z_measure - state->H * state->x);
	state->p = (1 - state->gain * state->H) * state->p;

	return state->x;
}
