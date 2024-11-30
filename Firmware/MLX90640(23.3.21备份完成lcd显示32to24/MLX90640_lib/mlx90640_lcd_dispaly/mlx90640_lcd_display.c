#include "stm32f4xx.h"                  // Device header
#include "mlx90640_lcd_display.h"
/*官方驱动headers*/
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
/*lcd驱动函数headers*/
#include "ST7789_Init.h"
#include "lcd.h"
/*需要用到的系统库*/
#include <string.h>


/*MLX90640所需的变量参数*/
#define  FPS2HZ   0x02
#define  FPS4HZ   0x03
#define  FPS8HZ   0x04
#define  FPS16HZ  0x05
#define  FPS32HZ  0x06

#define  MLX90640_ADDR 0x33
#define	 RefreshRate FPS8HZ 
#define EMMISIVITY 0.95f
#define TA_SHIFT 8 //Default shift for MLX90640 in open air

paramsMLX90640 mlx90640;
static uint16_t eeMLX90640[832];  
int status;


/*温度转颜色的变量参数*/
// start with some initial colors
float minTemp = 20.0f;
float maxTemp = 40.0f;
float centerTemp;

char tempBuffer[6];
// variables for interpolated colors
uint8_t red, green, blue;

// variables for row/column interpolation
float intPoint, val, a, b, c, d, ii;
int x, y, i, j;

// array for the 32 x 24 measured tempValues
static float tempValues[32*24];


/*定义函数声明*/
static uint16_t TempToColor(float val);
static void setTempScale(void);
static void setAbcd(void);
static void drawLegend(void);
static void drawMeasurement(void);
static void drawPicture(void);
static void readTempValues(void);
uint16_t GetColor565(uint8_t red, uint8_t green, uint8_t blue);

/*温度数值转颜色*/
static uint16_t TempToColor(float val){
	  /*
    pass in value and figure out R G B
    several published ways to do this I basically graphed R G B and developed simple linear equations
    again a 5-6-5 color display will not need accurate temp to R G B color calculation

    equations based on
    http://web-tech.ga-usa.com/2012/05/creating-a-custom-hot-to-cold-temperature-color-gradient-for-use-with-rrdtool/index.html

  */

  red = constrain(255.0f / (c - b) * val - ((b * 255.0f) / (c - b)), 0, 255);

  if ((val > minTemp) & (val < a)) {
    green = constrain(255.0f / (a - minTemp) * val - (255.0f * minTemp) / (a - minTemp), 0, 255);
  }
  else if ((val >= a) & (val <= c)) {
    green = 255;
  }
  else if (val > c) {
    green = constrain(255.0f / (c - d) * val - (d * 255.0f) / (c - d), 0, 255);
  }
  else if ((val > d) | (val < a)) {
    green = 0;
  }

  if (val <= b) {
    blue = constrain(255.0f / (a - b) * val - (255.0f * b) / (a - b), 0, 255);
  }
  else if ((val > b) & (val <= d)) {
    blue = 0;
  }
  else if (val > d) {
    blue = constrain(240.0f / (maxTemp - d) * val - (d * 240.0f) / (maxTemp - d), 0, 240);
  }

  // use the displays color mapping function to get 5-6-5 color palet (R=5 bits, G=6 bits, B-5 bits)
  return GetColor565(red, green, blue);
}

uint16_t GetColor565(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue) >> 3);
}

/*设置温度的极限值*/
static void setTempScale(void)
{
  minTemp = 255;
  maxTemp = 0;

  for (i = 0; i < 768; i++) {
    minTemp = min(minTemp, tempValues[i]);
    maxTemp = max(maxTemp, tempValues[i]);
  }

  setAbcd();
  drawLegend();
}


// Function to get the cutoff points in the temp vs RGB graph.
static void setAbcd(void) {
  a = minTemp + (maxTemp - minTemp) * 0.2121f;
  b = minTemp + (maxTemp - minTemp) * 0.3182f;
  c = minTemp + (maxTemp - minTemp) * 0.4242f;
  d = minTemp + (maxTemp - minTemp) * 0.8182f;
}



// Draw a legend.
/*lcd写当前的最高温和最低温*/
static void drawLegend(void) {
  float inc = (maxTemp - minTemp) / 224.0f;
  j = 0;
  for (ii = minTemp; ii < maxTemp; ii += inc) {
    // BSP_LCD_DrawVLine(8+ + j++, 292, 20, TempToColor(ii));
    LCD_DrawLine(8, 230, 8 + (maxTemp - minTemp), 230, TempToColor(ii));//不知道改得对不对，这个还得调
  }
	
	// BSP_LCD_SetFont(&Font16);
    /*memset:以字节为单位为任意变量赋值，可以理解成用某个数为变量进行初始化*/
	memset(tempBuffer,0,sizeof(tempBuffer));
	sprintf(tempBuffer,(char *)"%2.1f",minTemp);
	// BSP_LCD_DisplayStringAt(8,290,(uint8_t *)tempBuffer,LEFT_MODE,LCD_COLOR_BLUE);
    LCD_ShowString(8, 214, (uint8_t *)tempBuffer, /*fontcolor*/0x001f, /*blackcolor*/0x0000, 16, 0);//写当前最低温

	memset(tempBuffer,0,sizeof(tempBuffer));
	sprintf(tempBuffer,(char *)"%2.1f",maxTemp);
	// BSP_LCD_DisplayStringAt(190,290,(uint8_t *)tempBuffer,LEFT_MODE,LCD_COLOR_RED);
    LCD_ShowString(190, 214, (uint8_t *)tempBuffer, /*fontcolor*/0xf800, /*blackcolor*/0x0000, 16, 0);//写当前最高温

}


// Draw a circle + measured value.
/*画中心圈（框）+测量屏幕中心温度*/ // 没怎么懂，这个只是显示了32*24的像素
static void drawMeasurement(void ) {

  // Mark center measurement
//   BSP_LCD_DrawCircle(120, 8+84, 3, LCD_COLOR_WHITE);
    LCD_DrawRectangle(110, 110, 120, 120, WHITE); //画一个中心框

  // Measure and print center temperature
  centerTemp = (tempValues[383 - 16] + tempValues[383 - 15] + tempValues[384 + 15] + tempValues[384 + 16]) / 4;
	
	// BSP_LCD_SetFont(&Font20);
	memset(tempBuffer,0,sizeof(tempBuffer));
	sprintf(tempBuffer,(char *)"%2.2f ",centerTemp);
	// BSP_LCD_DisplayStringAt(88,220,(uint8_t *)tempBuffer,LEFT_MODE,LCD_COLOR_RED);
    LCD_ShowString(123, 110, (uint8_t *)tempBuffer, /*pencolor*/RED, /*bgcolor*/BLACK, 16, 1);  //在中心框旁边标注中心框内的温度
}

/*画出热成像图*/
static void drawPicture(void)
{ 
	for (y=0; y<24; y++) {
		for (x=0; x<32; x++) {
			// BSP_LCD_FillRect(8 + x*7, 8 + y*7, 7, 7, TempToColor(tempValues[(31-x) + (y*32)]));
            LCD_Color_Fill(8 + x*7, 36 + y*7, 8 + 7 + x*7, 36 + 7 + y*7, TempToColor(tempValues[(31-x) + (y*32)])); //不知道写的对不对哈
		}
	}
}


// Read pixel data from MLX90640.
/*从MLX90640中读取温度数值*/
static void readTempValues(void) {

  for (uint8_t x = 0 ; x < 2 ; x++) // Read both subpages // 读两次，读全整页（一次读半帧）
  {
    uint16_t mlx90640Frame[834];
    status = MLX90640_GetFrameData(MLX90640_ADDR, mlx90640Frame);
    if (status < 0)
    {
    //   printf("GetFrame Error: %d\r\n",status); // 读的时候必报错，但是数据没错，问题不大
    }

    float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
    float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature // 基于传感器环境温度的反射温度


    MLX90640_CalculateTo(mlx90640Frame, &mlx90640, EMMISIVITY, tr, tempValues); // 将读取的数值计算成温度值
  }
}

/*mlx90640_lcd_display处理函数*/ //放在主循环或者中断
void mlx90640_display_process(void)
{
	readTempValues();   //读温度数值
    setTempScale();     //设置温度操作的极限值
    drawPicture();      //画出热力图
	drawMeasurement();  //画中心框+显示中心框的温度
}


void mlx90640_display_init(void)
{
	MLX90640_SetRefreshRate(MLX90640_ADDR, RefreshRate);
	MLX90640_SetChessMode(MLX90640_ADDR);
	
    status = MLX90640_DumpEE(MLX90640_ADDR, eeMLX90640);
    if (status != 0) printf("\r\nload system parameters error with code:%d\r\n",status);
    status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
    if (status != 0) printf("\r\nParameter extraction failed with error code:%d\r\n",status);
}






