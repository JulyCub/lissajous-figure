/*
 ******************************************************************************
 * @file		:bsp_lcd1602.h
 * @brief		:The board support package for LCD1602.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.28
 ******************************************************************************
 */
 
#ifndef __BSP_LCD1602_H__
#define __BSP_LCD1602_H__

#include "bsp_config.h"
#include "gpio.h"


#define LCD1602_Port1			GPIOA
#define LCD1602_Port2			GPIOB
#define LCD1602_RS				GPIO_PIN_1
#define LCD1602_RW				GPIO_PIN_2
#define LCD1602_E					GPIO_PIN_3

#define  RS(n) 		HAL_GPIO_WritePin(LCD1602_Port1, LCD1602_RS, n)
#define  RW(n)		HAL_GPIO_WritePin(LCD1602_Port1, LCD1602_RW, n)
#define  E(n)			HAL_GPIO_WritePin(LCD1602_Port1, LCD1602_RS, n)


void lcd_delay1ms(unsigned char d);		//��ʱ����
void lcd_Init();		//��ʼ��
void lcd_write_com(unsigned char com);		//дָ��
void lcd_write_dat(unsigned char dat);		//д����
void lcd_xy(unsigned char x,unsigned char y);  //дλ��
void lcd_read_busy();		//����־λ





#endif
