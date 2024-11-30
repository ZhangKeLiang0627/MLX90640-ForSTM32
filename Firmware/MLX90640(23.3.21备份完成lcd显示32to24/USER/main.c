/* includes */
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"   
#include "malloc.h" 
#include "usmart.h"  
#include "mmc_sd.h"	   
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"    
#include "fattester.h"
#include "fontupd.h"
#include "text.h"
#include "WS2812B.h"
#include "Encoder.h"
#include "ST7789_Init.h"
#include "oled.h"
#include <MLX90640_I2C_Driver.h>
#include <MLX90640_API.h>
#include "mlx90640_lcd_display.h"


// /*定义MLX90640的变量参数*/
// #define  FPS2HZ   0x02
// #define  FPS4HZ   0x03
// #define  FPS8HZ   0x04
// #define  FPS16HZ  0x05
// #define  FPS32HZ  0x06

// #define  MLX90640_ADDR 0x33
// #define	 RefreshRate FPS4HZ 
// #define  TA_SHIFT 8 //Default shift for MLX90640 in open air

// static uint16_t eeMLX90640[832];  
// static float mlx90640To[768];
// uint16_t frame[834];
// float emissivity=0.95;
// int status;


/* 定义一些变量 */
uint16_t numdata;

int8_t Encoder_Num, Key_Num = 0;

/* 主函数 */
int main(void)
{   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(100);  					//初始化延时函数
	uart_init(115200);					//初始化串口波特率为115200
	LED_Init();							//初始化LED 
 	KEY_Init();							//按键初始化 
	
	ST7789_Init();
	OLED_Init();
	
	MLX90640_I2CInit();
 	printf("I2CinitOK!\r\n");

	mlx90640_display_init();
	printf("MLX90640initOK!\r\n");
// 	MLX90640_SetRefreshRate(MLX90640_ADDR, RefreshRate);
// 	MLX90640_SetChessMode(MLX90640_ADDR);
// 	paramsMLX90640 mlx90640;

// 	status = MLX90640_DumpEE(MLX90640_ADDR, eeMLX90640);
//   if (status != 0) printf("\r\nload system parameters error with code:%d\r\n",status);
//   status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
//   if (status != 0) printf("\r\nParameter extraction failed with error code:%d\r\n",status);
// 	numdata = MLX90640_GetRefreshRate(MLX90640_ADDR);
// 	printf("numdata = %d", numdata);
	
	LCD_Color_Fill(0,0,240,240,BLUE);

//	uint16_t i = 0;
	while(1)
	{	
		mlx90640_display_process(); //执行mlx90640_lcd_display处理函数
		
// 		int status = MLX90640_GetFrameData(MLX90640_ADDR, frame);
// 		if (status < 0)
// 		{
// 			//printf("GetFrame Error: %d\r\n",status);
// 		}
// 		float vdd = MLX90640_GetVdd(frame, &mlx90640);
// 		float Ta = MLX90640_GetTa(frame, &mlx90640);

// 		float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
// 		//printf("vdd:  %f Tr: %f\r\n",vdd,tr);
// 		MLX90640_CalculateTo(frame, &mlx90640, emissivity , tr, mlx90640To);
		
// 		printf("\r\n==========================IAMLIUBO MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
		
// //		sprintf(numdata, "%d", mlx90640To[i]);
		
// //		i++;
// 		for(uint16_t i = 0; i < 768; i++){
// //			
// 		if(i%32 == 0 && i != 0){
// 			printf("\r\n");
// 		}	
		
// 		printf("%2.2f ",mlx90640To[i]);
// //		OLED_Clear();
// //		OLED_ShowString(0,0,"float:", 16, 1);
// //		OLED_ShowNum(0,16,mlx90640To[i],2, 16, 1);
// //		OLED_Refresh();
// 		delay_ms(10);
			
			
// 		}
// 		printf("\r\n==========================IAMLIUB0 MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");



	} 
}





