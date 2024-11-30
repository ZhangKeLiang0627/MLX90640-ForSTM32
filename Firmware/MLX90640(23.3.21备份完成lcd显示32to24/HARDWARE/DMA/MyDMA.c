#include "MyDMA.h"

u16 color_buff[1];//DMA2��memory��ַ//��lcd.c����extern

void DMA2_Init(u16 cndtr)
{
	/*��ʼ���ṹ��*/
	DMA_InitTypeDef DMA_InitStructure;

	/*����rccʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	/*��λDMA2_Stream3*/
	DMA_DeInit(DMA2_Stream3);
	
	/*��ʼ���ṹ�������λDMA2_Stream3ӳ��SPI1_TX*/
	DMA_InitStructure.DMA_BufferSize = cndtr;
	DMA_InitStructure.DMA_Channel = DMA_Channel_3;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;

	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)color_buff;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;

	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI1->DR;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;

	DMA_Init(DMA2_Stream3, &DMA_InitStructure);
	
	/*�ر�DMA2*/
	DMA_Cmd(DMA2_Stream3, DISABLE);
}
















