/**
 ******************************************************************************
 * @file		:bsp_ad9833.c
 * @brief		:AD9833 module driver.
 * @version	:0.1.1
 * @author	:merchant
 * @date		:--
 ******************************************************************************
 * @chip		:STM32
 * @pinset	:
 *							GPIOB_PIN_15 	---> FSYNC
 *							GPIOB_PIN_14 	---> SCK
 *							GPIOB_PIN_13 	---> DAT
 *							GPIOB_PIN_12	---> CS
 *
 * @if MULT
 *							GPIOB_PIN_15 	---> FSYNC1
 *							GPIOB_PIN_14 	---> SCK1
 *							GPIOB_PIN_13 	---> DAT1
 *							GPIOB_PIN_12	---> CS1
 *
 *						 	GPIOC_PIN_0		---> FSYNC2
 *							GPIOC_PIN_1		---> SCK2
 *							GPIOC_PIN_2		---> DAT2
 *							GPIOC_PIN_3		---> CS2
 *
 *******************************************************************************
 */
#include "bsp_ad9833.h"
#include "gpio.h"
#if		0
#include "spi.h"
#endif


/*
*********************************************************************************
*	�� �� ��: AD9833_Delay
*	����˵��: ʱ����ʱ
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************
*/
static void AD9833_Delay(void)
{
	uint16_t i;
	for (i = 0; i < 30; i++);
}
static void AD9833_Delay2(void)
{
	uint16_t i;
	for (i = 0; i < 18; i++);
}
static void AD9833_Delay3(void)
{
	uint16_t i;
	for (i = 0; i < 8; i++);
}

#if    AD9833_DEMULT
/*
**********************************************************************************
*	�� �� ��: AD9833_Write
*	����˵��: ��SPI���߷���16��bit����
*	��    ��: TxData : ����
*	�� �� ֵ: ��
***********************************************************************************
*/
void AD9833_Write(unsigned int TxData)
{
	unsigned char i;
	/*
	uint8_t SpiTxData[2] = {0};
	
	SpiTxData[0] = (uint8_t)((TxData & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(TxData &  0x00ff);
	*/
	
	SCK(_1);
	
	
	FSY(_1);
	AD9833_Delay2();
	FSY(_0);
	AD9833_Delay2();
	#if defined (SofeSpi)
	
	for(i = 0; i < 16; i++)
	{
		if (TxData & 0x8000)
			DAT(_1);
		else
			DAT(_0);
		
		AD9833_Delay2();
		SCK(_0);
		AD9833_Delay2();		
		SCK(_1);
		
		TxData <<= 1;
	}
	
	#endif
	#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
	#endif
	
	FSY(_1);
	AD9833_Delay2();
} 

/*
***********************************************************************************
*	�� �� ��: AD9833_AmpSet
*	����˵��: �ı�����źŷ���ֵ
*	��    ��: 1.amp ������ֵ  0- 255
*	�� �� ֵ: ��
***********************************************************************************
*/ 


void AD9833_AmpSet(unsigned char amp)
{
	unsigned char i;
	unsigned int temp;
   	
	temp =0x1100|amp;
	
	/*
	uint8_t SpiTxData[2] = {0};
	
	SpiTxData[0] = (uint8_t)((temp & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(temp &  0x00ff);
	*/
		
	CS(_0);
	AD9833_Delay2();

	
	#if defined (SofeSpi)
	for(i=0;i<16;i++)
	{
	    SCK(_0);	
		AD9833_Delay2();
	   if(temp&0x8000)
		 {
			 DAT(_1);
			AD9833_Delay2();
		 }
	   else
		 {
				DAT(_0);
				AD9833_Delay2();
		 }
		temp<<=1;
	    SCK(_1);
	    AD9833_Delay();
	}
	#endif
	
	#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
	#endif
	
  CS(_1);
	AD9833_Delay2();
}


/*
***********************************************************************************
*	�� �� ��: AD9833_WaveSeting
*	����˵��: ��SPI���߷���16��bit����
*	��    ��: 1.Freq: Ƶ��ֵ, 0.1 hz - 12Mhz
						2.Freq_SFR: 0 �� 1
						3.WaveMode: TRI_WAVE(���ǲ�),SIN_WAVE(���Ҳ�),SQU_WAVE(����)
						4.Phase : ���εĳ���λ
*	�� �� ֵ: ��
***********************************************************************************
*/ 
void AD9833_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************����Ƶ�ʵ�16����ֵ***********************************/
	
		frequence_mid=268435456/25;	//�ʺ�25M����
																//���ʱ��Ƶ�ʲ�Ϊ25MHZ���޸ĸô���Ƶ��ֵ����λMHz ��AD9833���֧��25MHz
		
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA; 			 	//���frequence_hex��ֵ��32λ��һ���ܴ�����֣���Ҫ��ֳ�����14λ���д���
		frequence_LSB=frequence_hex; 					//frequence_hex��16λ�͸�frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;		//ȥ�������λ��16λ����ȥ����λ������14λ
		frequence_MSB=frequence_hex>>14;			//frequence_hex��16λ�͸�frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;		//ȥ�������λ��16λ����ȥ����λ������14λ

	
		Phs_data=Phase|0xC000;	//��λֵ
		AD9833_Write(0x0100); 	//��λAD9833,��RESETλΪ1
		AD9833_Write(0x2100); 	//ѡ������һ��д�룬B28λ��RESETλΪ1

		if(Freq_SFR==0)		//���������õ�����Ƶ�ʼĴ���0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //ʹ��Ƶ�ʼĴ���0�������
			AD9833_Write(frequence_LSB); 	//L14��ѡ��Ƶ�ʼĴ���0�ĵ�14λ��������
			AD9833_Write(frequence_MSB); 	//H14 Ƶ�ʼĴ����ĸ�14λ��������
			AD9833_Write(Phs_data);				//������λ
			//AD9833_Write(0x2000); 			//����FSELECTλΪ0��оƬ���빤��״̬,Ƶ�ʼĴ���0�������
	    }
		if(Freq_SFR==1)		//���������õ�����Ƶ�ʼĴ���1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//ʹ��Ƶ�ʼĴ���1�������
			AD9833_Write(frequence_LSB); 	//L14��ѡ��Ƶ�ʼĴ���1�ĵ�14λ����
			AD9833_Write(frequence_MSB); 	//H14 Ƶ�ʼĴ���1Ϊ
			AD9833_Write(Phs_data);				//������λ
			//AD9833_Write(0x2800); 			//����FSELECTλΪ0������FSELECTλΪ1����ʹ��Ƶ�ʼĴ���1��ֵ��оƬ���빤��״̬,Ƶ�ʼĴ���1�������
		}

		if(WaveMode==TRI_WAVE) 	//������ǲ�����
		 	AD9833_Write(0x2002); 
		if(WaveMode==SQU_WAVE)	//�����������
			AD9833_Write(0x2028); 
		if(WaveMode==SIN_WAVE)	//������Ҳ���
			AD9833_Write(0x2000); 

}
/*
***********************************************************************************
* SPI���͵��ֽ�
* xtemp�����͵��ֽ�
***********************************************************************************
*/
#if 1
void MCP41010_SPI_SendOneByte(uint8_t xtemp)
{
	uint8_t xnum = 0;
	
	SCK(_0);
	AD9833_Delay3();
	/* DP_SCK����8�����壬��������ʱ�������� */
	for(xnum=0; xnum<8; xnum++)
	{
		//DP_SIд����
		//DP_SI = (xtemp & 0x80) >>7;
		if(xtemp & 0x80)
		{
			DAT(_1);
		}
		else DAT(_0);
		xtemp = xtemp << 1;
		
		//��ʱ�������ȶ�����DP_SCK�������������壬��������MCP41010
		AD9833_Delay3();
		SCK(_1);
		
		//DP_SCK�����½�������
		AD9833_Delay3();
		SCK(_0);
	}
}

/*
***********************************************************************************
* ����MCP41010�Ļ�����������ͷλ��
* xRes���������ֵ
* ע��00ʱ��W��ͷ���ڳ�ͷB��λ�ã���ֵ���ڳ�ͷ���裬����ֵΪ52��
* ע��FFʱ��W��ͷ���ڳ�ͷA��λ�ã���ֵ��Ϊ9985.94������ͷʼ�ղ�����A������
* ע������10K����������˵��LSBΪ39.0625��
* ע��ÿ��оƬ�ϵ�󣬳�ͷ�Ĵ�����ֵΪ0x80
***********************************************************************************
*/

void MCP41010_SetRes(uint8_t xRes)
{
	CS(_1);
	SCK(_1);
	AD9833_Delay2();
	//MCP41010оƬʹ��
	CS(_0);
	AD9833_Delay2();
	//���͵�һ�ֽڣ����Ʒ�ʽλ
	//0x13��д���ݣ��Ե�λ��0��1����
	MCP41010_SPI_SendOneByte(0x13);
	
	//���͵ڶ��ֽڣ�������������ͷλ
	MCP41010_SPI_SendOneByte(xRes);
	
	//MCP41010оƬʧ��
	CS(_1);
}

#endif

#if			0
uint8_t	CMD_NONE = 0, CMD_WRITE = 1, CMD_SHUT_DOWN = 2;

void bsp_mcp41_init(uint8_t pot_value)
{
	bsp_mcp41_set_value(1, pot_value);
#if		0
	spi_driver_start();
	spi_driver_tx();
	spi_driver_rx();
	spi_driver_stop();
#endif
}

void bsp_mcp41_set_value(uint8_t pot_id, uint8_t pot_value)
{
	uint8_t data[2] = {0};

	/* write command & pot id */
	data[0] = (CMD_WRITE << 4) | pot_id;

	/* insert pot value */
	data[1] = pot_value;

	CS_0();//spi_driver_start();
	HAL_SPI_Transmit(&hspi1, data, 2, 1000);//spi_driver_tx_dma(data, 2);
	CS_1();//spi_driver_stop();
}

void bsp_AD9833_IO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA5 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}
void bsp_AD9833_SPI_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		__HAL_RCC_SPI1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 }
#endif
#else
/*
**********************************************************************************
*	�� �� ��: AD9833_MULT_Write
*	����˵��: ��SPI���߷���16��bit����
*	��    ��: TxData : ����
*	�� �� ֵ: ��
***********************************************************************************
*/
void AD9833_MULT_Write1(unsigned int TxData)
{
	unsigned char i;
	/*
	uint8_t SpiTxData[2] = {0};
	
	SpiTxData[0] = (uint8_t)((TxData & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(TxData &  0x00ff);
	*/
	
	SCK1(_1);
	
	
	FSY1(_1);
	AD9833_Delay2();
	FSY1(_0);
	AD9833_Delay2();
	#if defined (SofeSpi)
	
	for(i = 0; i < 16; i++)
	{
		if (TxData & 0x8000)
			DAT1(_1);
		else
			DAT1(_0);
		
		AD9833_Delay2();
		SCK1(_0);
		AD9833_Delay2();		
		SCK1(_1);
		
		TxData <<= 1;
	}
	
	#endif
	#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
	#endif
	
	FSY1(_1);
	AD9833_Delay2();
} 
void AD9833_MULT_Write2(unsigned int TxData)
{
	unsigned char i;
	/*
	uint8_t SpiTxData[2] = {0};
	
	SpiTxData[0] = (uint8_t)((TxData & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(TxData &  0x00ff);
	*/
	
	SCK2(_1);
	
	
	FSY2(_1);
	AD9833_Delay2();
	FSY2(_0);
	AD9833_Delay2();
	#if defined (SofeSpi)
	
	for(i = 0; i < 16; i++)
	{
		if (TxData & 0x8000)
			DAT2(_1);
		else
			DAT2(_0);
		
		AD9833_Delay2();
		SCK2(_0);
		AD9833_Delay2();		
		SCK2(_1);
		
		TxData <<= 1;
	}
	
	#endif
	#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
	#endif
	
	FSY2(_1);
	AD9833_Delay2();
} 
/*
***********************************************************************************
*	�� �� ��: AD9833_MULT_AmpSet
*	����˵��: �ı�����źŷ���ֵ
*	��    ��: 1.amp ������ֵ  0- 255
*	�� �� ֵ: ��
***********************************************************************************
*/ 


void AD9833_MULT_AmpSet1(unsigned char amp)
{
	unsigned char i;
	unsigned int temp;
   	
	temp =0x1100|amp;
	
	/*
	uint8_t SpiTxData[2] = {0};
	
	SpiTxData[0] = (uint8_t)((temp & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(temp &  0x00ff);
	*/
		
	CS1(_0);
	AD9833_Delay2();

	
	#if defined (SofeSpi)
	for(i=0;i<16;i++)
	{
	    SCK1(_0);	
			AD9833_Delay2();
			if(temp&0x8000)
			{
					DAT1(_1);
					AD9833_Delay2();
			}
			else
			{
					DAT1(_0);
					AD9833_Delay2();
			}
			temp<<=1;
	    SCK1(_1);
	    AD9833_Delay();
	}
	#endif
	
	#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
	#endif
	
  CS1(_1);
	AD9833_Delay2();
}
void AD9833_MULT_AmpSet2(unsigned char amp)
{
	unsigned char i;
	unsigned int temp;
   	
	temp =0x1100|amp;
	
	/*
	uint8_t SpiTxData[2] = {0};
	
	SpiTxData[0] = (uint8_t)((temp & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(temp &  0x00ff);
	*/
		
	CS2(_0);
	AD9833_Delay2();

	
	#if defined (SofeSpi)
	for(i=0;i<16;i++)
	{
	    SCK2(_0);	
		AD9833_Delay2();
	   if(temp&0x8000)
		 {
			 DAT2(_1);
			AD9833_Delay2();
		 }
	   else
		 {
				DAT2(_0);
				AD9833_Delay2();
		 }
		temp<<=1;
	    SCK2(_1);
	    AD9833_Delay();
	}
	#endif
	
	#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
	#endif
	
  CS2(_1);
	AD9833_Delay2();
}


/*
***********************************************************************************
*	�� �� ��: AD9833_MULT_WaveSeting
*	����˵��: ��SPI���߷���16��bit����
*	��    ��: 1.Freq: Ƶ��ֵ, 0.1 hz - 12Mhz
						2.Freq_SFR: 0 �� 1
						3.WaveMode: TRI_WAVE(���ǲ�),SIN_WAVE(���Ҳ�),SQU_WAVE(����)
						4.Phase : ���εĳ���λ
*	�� �� ֵ: ��
***********************************************************************************
*/ 
void AD9833_MULT_WaveSeting1(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************����Ƶ�ʵ�16����ֵ***********************************/
	
		frequence_mid=268435456/25;	//�ʺ�25M����
																//���ʱ��Ƶ�ʲ�Ϊ25MHZ���޸ĸô���Ƶ��ֵ����λMHz ��AD9833���֧��25MHz
		
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA; 			 	//���frequence_hex��ֵ��32λ��һ���ܴ�����֣���Ҫ��ֳ�����14λ���д���
		frequence_LSB=frequence_hex; 					//frequence_hex��16λ�͸�frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;		//ȥ�������λ��16λ����ȥ����λ������14λ
		frequence_MSB=frequence_hex>>14;			//frequence_hex��16λ�͸�frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;		//ȥ�������λ��16λ����ȥ����λ������14λ

	
		Phs_data=Phase|0xC000;	//��λֵ
		AD9833_MULT_Write1(0x0100); 	//��λAD9833,��RESETλΪ1
		AD9833_MULT_Write1(0x2100); 	//ѡ������һ��д�룬B28λ��RESETλΪ1

		if(Freq_SFR==0)		//���������õ�����Ƶ�ʼĴ���0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //ʹ��Ƶ�ʼĴ���0�������
			AD9833_MULT_Write1(frequence_LSB); 	//L14��ѡ��Ƶ�ʼĴ���0�ĵ�14λ��������
			AD9833_MULT_Write1(frequence_MSB); 	//H14 Ƶ�ʼĴ����ĸ�14λ��������
			AD9833_MULT_Write1(Phs_data);				//������λ
			//AD9833_MULT_Write1(0x2000); 			//����FSELECTλΪ0��оƬ���빤��״̬,Ƶ�ʼĴ���0�������
	    }
		if(Freq_SFR==1)		//���������õ�����Ƶ�ʼĴ���1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//ʹ��Ƶ�ʼĴ���1�������
			AD9833_MULT_Write1(frequence_LSB); 	//L14��ѡ��Ƶ�ʼĴ���1�ĵ�14λ����
			AD9833_MULT_Write1(frequence_MSB); 	//H14 Ƶ�ʼĴ���1Ϊ
			AD9833_MULT_Write1(Phs_data);				//������λ
			//AD9833_MULT_Write1(0x2800); 			//����FSELECTλΪ0������FSELECTλΪ1����ʹ��Ƶ�ʼĴ���1��ֵ��оƬ���빤��״̬,Ƶ�ʼĴ���1�������
		}

		if(WaveMode==TRI_WAVE) 	//������ǲ�����
		 	AD9833_MULT_Write1(0x2002); 
		if(WaveMode==SQU_WAVE)	//�����������
			AD9833_MULT_Write1(0x2028); 
		if(WaveMode==SIN_WAVE)	//������Ҳ���
			AD9833_MULT_Write1(0x2000); 

}
void AD9833_MULT_WaveSeting2(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************����Ƶ�ʵ�16����ֵ***********************************/
	
		frequence_mid=268435456/25;	//�ʺ�25M����
																//���ʱ��Ƶ�ʲ�Ϊ25MHZ���޸ĸô���Ƶ��ֵ����λMHz ��AD9833���֧��25MHz
		
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA; 			 	//���frequence_hex��ֵ��32λ��һ���ܴ�����֣���Ҫ��ֳ�����14λ���д���
		frequence_LSB=frequence_hex; 					//frequence_hex��16λ�͸�frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;		//ȥ�������λ��16λ����ȥ����λ������14λ
		frequence_MSB=frequence_hex>>14;			//frequence_hex��16λ�͸�frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;		//ȥ�������λ��16λ����ȥ����λ������14λ

	
		Phs_data=Phase|0xC000;	//��λֵ
		AD9833_MULT_Write2(0x0100); 	//��λAD9833,��RESETλΪ1
		AD9833_MULT_Write2(0x2100); 	//ѡ������һ��д�룬B28λ��RESETλΪ1

		if(Freq_SFR==0)		//���������õ�����Ƶ�ʼĴ���0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //ʹ��Ƶ�ʼĴ���0�������
			AD9833_MULT_Write2(frequence_LSB); 	//L14��ѡ��Ƶ�ʼĴ���0�ĵ�14λ��������
			AD9833_MULT_Write2(frequence_MSB); 	//H14 Ƶ�ʼĴ����ĸ�14λ��������
			AD9833_MULT_Write2(Phs_data);				//������λ
			//AD9833_MULT_Write2(0x2000); 			//����FSELECTλΪ0��оƬ���빤��״̬,Ƶ�ʼĴ���0�������
	    }
		if(Freq_SFR==1)		//���������õ�����Ƶ�ʼĴ���1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//ʹ��Ƶ�ʼĴ���1�������
			AD9833_MULT_Write2(frequence_LSB); 	//L14��ѡ��Ƶ�ʼĴ���1�ĵ�14λ����
			AD9833_MULT_Write2(frequence_MSB); 	//H14 Ƶ�ʼĴ���1Ϊ
			AD9833_MULT_Write2(Phs_data);				//������λ
			//AD9833_MULT_Write2(0x2800); 			//����FSELECTλΪ0������FSELECTλΪ1����ʹ��Ƶ�ʼĴ���1��ֵ��оƬ���빤��״̬,Ƶ�ʼĴ���1�������
		}

		if(WaveMode==TRI_WAVE) 	//������ǲ�����
		 	AD9833_MULT_Write2(0x2002); 
		if(WaveMode==SQU_WAVE)	//�����������
			AD9833_MULT_Write2(0x2028); 
		if(WaveMode==SIN_WAVE)	//������Ҳ���
			AD9833_MULT_Write2(0x2000); 

}

#endif


