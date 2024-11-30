#ifndef __MLX90640_LCD_DISPLAY_H
#define __MLX90640_LCD_DISPLAY_H

/* includes */
#include <stdio.h>
#include <math.h>


/* define */
#define constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt))) //限制函数 //用于限制温度2颜色超出阈值

#define min(a,b)  ((a)<(b)?(a):(b)) //求最小值

#define max(a,b)  ((a)>(b)?(a):(b)) //求最大值

#define abs(x)    ((x)>0?(x):(-x)) //求绝对值


/* Func */
void mlx90640_display_init(void);
void mlx90640_display_process(void);


#endif /* __MLX90640_LCD_DISPLAY_H */

