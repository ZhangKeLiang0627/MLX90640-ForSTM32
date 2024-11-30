#include "sys.h" 
#include "WS2812B.h"
#include "delay.h"

/*�����������Ҫ��������ݶ�Ӧ�ıȽ�ֵ����*/
uint32_t Single_WS2812B_Buffer[DATA_SIZE*WS2812B_NUM+50] ={0};//24bit*�������� + Reset_Data(1.25us*50>50us)

/**
  * @brief TIM2_PWM_CH2&DMA1&PA1��ʼ��
  * @param  arr��װ��ֵ��105 - 1��
  * @retval ��
  */
void PWM_WS2812B_Init(uint16_t arr)
{
	/*��ʼ���ṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	/*ʹ��rccʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/*��ʼ��gpio���ſ�*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*����PA1ΪTIM2_CH2*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);

	TIM_DeInit(TIM2);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ʱ�ӷָ�
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = arr;		//arr
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;		//psc��1��Ƶ������Ƶ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	//�߼���ʱ��ר��

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	
	/*��ʼ��TIM2_CH2 PWMģʽ*/
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���

	TIM_Cmd(TIM2, DISABLE);//ʧ��TIM2����ֹ��һ�������쳣
	
	TIM_DMACmd(TIM2, TIM_DMA_CC2, ENABLE);//ʹ��TIM2_CH2��DMA����(CC2��Ӧͨ��2)
	
	DMA_DeInit(DMA1_Stream6);
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_BufferSize = DATA_SIZE;
	DMA_InitStructure.DMA_Channel = DMA_Channel_3;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Single_WS2812B_Buffer;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM2->CCR2);
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);

	DMA_Cmd(DMA1_Stream6, DISABLE);//ʧ��DMA1��6ͨ������Ϊһ��ʹ�ܾͿ�ʼ����
}

void WS2812B_Write_24Bits(uint16_t num, uint32_t GRB_Data)
{
	uint8_t i, j;
	for(j = 0; j < num; j++)
  	{
		for(i = 0; i < DATA_SIZE; i++)
		{
			/*��Ϊ���ݷ��͵�˳����GRB����λ�ȷ������ԴӸ�λ��ʼ�жϣ��жϺ�Ƚ�ֵ�ȷ��뻺������*/
			Single_WS2812B_Buffer[i+j*DATA_SIZE] = ((GRB_Data << i) & 0x800000) ? T1H : T0H;
		}
  	}
}

void WS2812B_Write_24Bits_independence(uint16_t num, uint32_t *GRB_Data)
{
	uint8_t i, j;
	for(j = 0; j < num; j++)
  	{
		for(i = 0; i < DATA_SIZE; i++)
		{
			/*��Ϊ���ݷ��͵�˳����GRB����λ�ȷ������ԴӸ�λ��ʼ�жϣ��жϺ�Ƚ�ֵ�ȷ��뻺������*/
			Single_WS2812B_Buffer[i+j*DATA_SIZE] = ((GRB_Data[j] << i) & 0x800000) ? T1H : T0H;
		}
  	}
}

void WS2812B_Show(void)
{
	/* ��ֲʱ�˴���Ӧ��ͨ�����жϱ�־����Ҫ���� */

	DMA_SetCurrDataCounter(DMA1_Stream6, DATA_SIZE*WS2812B_NUM+50);
	
	/*����DMA��TIM2��ʼ����*/
	DMA_Cmd(DMA1_Stream6, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	if(DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6))
	{
		DMA_Cmd(DMA1_Stream6, DISABLE);
		DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
		TIM_Cmd(TIM2, DISABLE);
	}
}


//N�����鷢���
void PWM_WS2812B_Red(uint16_t num)
{
	WS2812B_Write_24Bits(num,0x00ff00);
	WS2812B_Show();
}
//N�����鷢�̹�
void PWM_WS2812B_Green(uint16_t num)
{
	WS2812B_Write_24Bits(num,0xff0000);
	WS2812B_Show();
}
//N�����鷢����
void PWM_WS2812B_Blue(uint16_t num)
{
	WS2812B_Write_24Bits(num,0x0000ff);
	WS2812B_Show();
}



/*������*/
void set_ws2812_breathing(uint8_t index)
{
	 int i = 0;
	 switch (index) {
	 case 0: /* red */
			for (i = 0; i < 254; i+=2) {
				WS2812B_Write_24Bits(64, (uint32_t)(0x00<<16 | i<<8 | 0x00));WS2812B_Show();
				delay_ms(10);
			}
			for (i = 254; i > 0; i-=2) {
				WS2812B_Write_24Bits(64, (uint32_t)(0x00<<16 | i<<8 | 0x00));WS2812B_Show();
				delay_ms(10);
			}
			 break;
	 case 1: /* green */
			for (i = 0; i < 254; i+=2) {
				WS2812B_Write_24Bits(64, (uint32_t)(i<<16 | 0x00<<8 | 0x00));WS2812B_Show();
				delay_ms(10);
			}
			for (i = 254; i > 0; i-=2) {
				WS2812B_Write_24Bits(64, (uint32_t)(i<<16 | 0x00<<8 | 0x00));WS2812B_Show();
				delay_ms(10);
			}
			 break;
	 case 2: /* blue */
			for (i = 0; i < 254; i+=2) {
				WS2812B_Write_24Bits(64, (uint32_t)(0x00<<16 | 0x00<<8 | i));WS2812B_Show();
				delay_ms(10);
			}
			for (i = 254; i > 0; i-=2) {
				WS2812B_Write_24Bits(64, (uint32_t)(0x00<<16 | 0x00<<8 | i));WS2812B_Show();
				delay_ms(10);
			}
			break;
	 }
}





