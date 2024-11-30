#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#define LCD_CS_Clr() 	GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define LCD_CS_Set()	GPIO_SetBits(GPIOA, GPIO_Pin_4)

void SPI2_Init(void);			 //��ʼ��SPI2��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI2�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI2���߶�дһ���ֽ�

void SPI1_Init(void);   //��ʼ��SPI1��

void LCD_Writ_Bus(u8 dat);
		 
#endif

