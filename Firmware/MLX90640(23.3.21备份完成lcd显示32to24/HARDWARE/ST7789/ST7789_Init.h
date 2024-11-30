#ifndef __ST7789_INIT_H__
#define __ST7789_INIT_H__

#include "sys.h"

#define USE_HORIZONTAL 0  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����

#define LCD_W 240
#define LCD_H 240

#define LCD_RES_Clr()   GPIO_ResetBits(GPIOA, GPIO_Pin_15)
    
#define LCD_RES_Set()	GPIO_SetBits(GPIOA, GPIO_Pin_15)

#define LCD_DC_Clr()	GPIO_ResetBits(GPIOC, GPIO_Pin_14)
#define LCD_DC_Set()	GPIO_SetBits(GPIOC, GPIO_Pin_14)

#define LCD_BLK_Clr()	GPIO_ResetBits(GPIOC, GPIO_Pin_15)
#define LCD_BLK_Set()	GPIO_SetBits(GPIOC, GPIO_Pin_15)

void ST7789_GPIO_Init(void);

void LCD_WR_DATA8(u8 dat);//д��һ���ֽ�
void LCD_WR_DATA(u16 dat);//д�������ֽ�
void LCD_WR_REG(u8 dat);//д��һ��ָ��
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//�������꺯��

void ST7789_Init(void);

#endif

