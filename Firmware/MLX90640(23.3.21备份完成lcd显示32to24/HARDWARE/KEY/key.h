#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"  

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) //PA0

/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define KEY0 		PAin(0)   	//PA0
*/

#define KEY0_PRES 	1


void KEY_Init(void);	//IO��ʼ��
u8 KEY_Scan(u8);  		//����ɨ�躯��	

#endif
