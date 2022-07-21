/**
 ******************************************************************************
 * @file		:bsp_usart_hmi.h
 * @brief		:The board support package for USART HMI.
 * @version	:0.1.2
 * @author	:July
 * @date		:2022.06.26
 ******************************************************************************
 */

#ifndef __BSP_USART_HMI_H__
#define __BSP_USART_HMI_H__

#include "bsp_config.h"
#include "bsp_usart.h"

/* �ı��ؼ� */
typedef struct hmi_text{
	uint8_t			page;
	uint8_t			id;
	uint8_t			name;
	_Bool				drag;
	uint8_t			aph;
	uint8_t			effect;
	uint8_t			pco;
}hmitextTypedef;

/* ����/���οؼ� */
typedef struct hmi_curve{
	uint8_t 		page;		
	uint8_t 		id;
	uint8_t 		chnl[4];
	uint8_t			name;
	_Bool				drag;
	uint8_t			aph;
	uint8_t			effect;
}hmicurveTypedef;

/* ����/���οؼ�ͨ���궨�� */
#define HMI_CHANNEL_1				1U
#define HMI_CHANNEL_2				2U
#define HMI_CHANNEL_3				3U
#define HMI_CHANNEL_4				4U

/* ���ñ��� */
extern uint8_t hmi_chnl;
extern uint8_t hmi_id;
extern uint8_t hmi_page;
extern uint16_t hmi_point;
extern uint16_t hmi_point_counts;


/* ָ�����֡ */
#define __prifend														bsprif1("\xff\xff\xff")
/* �л�ҳ�� */
#define __switchpage(n)										{	bsprif1("page %d", hmi_page = n); \
																						__prifend;	}
/* ���߿ؼ�����һ���� */
#define __sendpoint(_ID,_CHNL,_POINT)			{	bsprif1("add %d,%d,%d", hmi_id = _ID, hmi_chnl = _CHNL, hmi_point = _POINT); \
																						__prifend;	}
/* ���߿ؼ���Ӷ����(͸��) */
#define __sendpoints(_ID,_CHNL,_COUNT)		{ bsprif1("addt %d,%d,%d", hmi_id = _ID, hmi_chnl = _CHNL, hmi_point_counts = _COUNT); \
																						__prifend;	}

																		
																		
/* ��ͨ������͸������ */																	
void bsp_HMI_sendCurveSingle(uint8_t id, uint8_t chnl, uint8_t points[], uint16_t len, uint8_t page);

#endif /* __BSP_USART_HMI_H__ */
