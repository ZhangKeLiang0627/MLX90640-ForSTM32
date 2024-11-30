#include "Encoder.h"

void Encoder_Init(void)
{  
    /*初始化结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
	
	/*开启rcc时钟*/
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

     /*初始化PB4&PB5*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*将PB4复用为TIM3_CH1&将PB5复用为TIM3_CH2*/
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);

	/*TIM3初始化*/
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65535;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	/*输入捕获初始化*/
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;//输入捕获滤波器（0~15）
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	/*配置编码器函数*/
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	/*清除中断标志位*/
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	/*开启定时器中断*/
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/*初始化计数值*/
    TIM_SetCounter(TIM3, 32768);
	
	/*配置优先级*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);
	
	/*开启定时器TIM3*/
    TIM_Cmd(TIM3, ENABLE);
}


/**
  * @brief TIM3的中断函数
  * @param  无
  * @retval 无
  */
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

/**
  * @brief 读取CE11的值（1or-1）
  * @param  无
  * @retval 返回（1or-1）
  */
int Read_value(void)
{
	int value_1 = 0;
	
	value_1 = (int16_t)(TIM_GetCounter(TIM3) - 32768);TIM_SetCounter(TIM3, 32768);	

	return value_1;
}

/**
  * @brief 应用层函数（在主函数中调用）
  * @param  无
  * @retval 正转返回1/反转返回2
  */
uint8_t Encoder_GetNum(void)
{
    int Encoder_Num = 0;
    static uint8_t Encoder_ReturnNum1, Encoder_ReturnNum2;
    Encoder_Num = Read_value();
    switch(Encoder_Num)
    {
        case 1: Encoder_ReturnNum1 ++; break;
        case -1: Encoder_ReturnNum2 ++; break;
        default: break;
    }
	if(Encoder_ReturnNum1 >= 2)
	{
		Encoder_ReturnNum1 = 0;
		Encoder_ReturnNum2 = 0;
		return 1;
	}
	else if(Encoder_ReturnNum2 >= 2 || (Encoder_ReturnNum1 == 1 && Encoder_ReturnNum2 == 1))
	{
		Encoder_ReturnNum1 = 0;
		Encoder_ReturnNum2 = 0;
		return 2;
	}	
	else return 0;
}











