/*
 ******************************************************************************
 * @file		:bsp_coding.h
 * @brief		:The board support package for digital coding.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.27
 ******************************************************************************
 */

#ifndef __BSP_CODING_H__
#define __BSP_CODING_H__
 
#include "bsp_config.h"

/* -@- */
#if			0
	#define __RZ
#elif 	0
	#define __NRZ
#elif		0
	#define __MCST
#elif		0
	#define __DFMCST
#endif 

/* ʮ����ת������ */
void bsp_DecToBin(int num, uint16_t code[], unsigned char len);
/* ������ת������ */
void bsp_FloatToBin(float num, uint16_t code[]);
/* ������� */
_Bool RZ(uint16_t str[], uint16_t code[], char len);
/* ��������� */
_Bool NRZ(uint16_t str[], uint16_t code[], char len);
/* ����˹�ر��� */
_Bool MCST(uint16_t str[], uint16_t code[], char len);
/* ����˹�ؽ��� */
_Bool MCST_Decode(uint16_t code[], uint16_t decode[]);
/* �������˹�ر��� */
_Bool DFMCST(uint16_t str[], uint16_t code[], char len);

#endif
