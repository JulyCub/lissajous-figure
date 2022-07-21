/**
 ******************************************************************************
 * @file			:bsp_filter.h
 * @brief			:The board support package for filter.
 * @version		:0.1.0
 * @author		:July
 * @date			:2022.07.02
 ******************************************************************************
 */
 
#ifndef __BSP_FILTER_H__
#define __BSP_FILTER_H__

#include "bsp_config.h"

/* �Ƿ�����DSP��  */
#define 		arm_dsp		0


#if					arm_dsp
#include "arm_math.h"

#define LENGTH_SAMPLES  		4096
/* �������� */
#define BLOCK_SIZE					1        			/* ����һ��arm_fir_f32����Ĳ�������� */
#define NUM_TAPS						32     				/* �˲���ϵ������ */

extern float32_t Input_f32_50Hz_200Hz[LENGTH_SAMPLES]; 					/* ������ */
extern float32_t Output[LENGTH_SAMPLES];               					/* �˲������� */
extern float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];        /* ״̬���棬��СnumTaps + blockSize - 1*/

void bsp_arm_fir_f32(void);
void bsp_GettspB(void);
#endif

#define  FNUM			10

/* ƽ���˲��㷨 */
float Onepointfilter(int16_t data);
void linearSmooth3 ( double in[], double out[], int N );
void linearSmooth5 ( double in[], double out[], int N );
void linearSmooth7 ( double in[], double out[], int N );
void quadraticSmooth5(double in[], double out[], int N);
void quadraticSmooth7(double in[], double out[], int N);
void cubicSmooth5(double in[], double out[], int N);
void cubicSmooth7(double in[], double out[], int N);


#endif
