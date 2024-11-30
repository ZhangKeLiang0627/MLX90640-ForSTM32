#ifndef __MLX90640_LCD_DISPLAY_H
#define __MLX90640_LCD_DISPLAY_H

/* includes */
#include <stdio.h>
#include <math.h>


/* define */
#define constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt))) //���ƺ��� //���������¶�2��ɫ������ֵ

#define min(a,b)  ((a)<(b)?(a):(b)) //����Сֵ

#define max(a,b)  ((a)>(b)?(a):(b)) //�����ֵ

#define abs(x)    ((x)>0?(x):(-x)) //�����ֵ


/* Func */
void mlx90640_display_init(void);
void mlx90640_display_process(void);


#endif /* __MLX90640_LCD_DISPLAY_H */

