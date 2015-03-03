#include "LCD_Curve.h"
u8 _LCD_Curve_Buffer[LCD_CURVE_BUFFER_LEN];
/********************************************************************
 * 名称 : _LCD_Set_Curve_Pixel
 * 功能 : 设置曲线像素点 (不建议用户使用)
 * 输入 : 曲线 x,y,color
 * 输出 : 无 
 ***********************************************************************/
void inline _LCD_Set_Curve_Pixel(Lcd_Curve* Lcd_Curvel1, u16 x, u16 y, u16
                                 color){
  u16 x1 = x - Lcd_Curvel1->Start_x;
  u16 y1 = y - Lcd_Curvel1->Start_y;
  if ((y1 % HEIGHT_PART_SIZE == 0 and(x1 < WIDTH_EDGE or x1 > LCD_CURVE_WIDTH -
      WIDTH_EDGE or x1 % WIDTH_PART_SIZE == 0))or(x1 % WIDTH_PART_SIZE == 0 and
      (y1 < HEIGHT_EDGE or y1 > LCD_CURVE_HEIGHT - HEIGHT_EDGE or y1 %
      HEIGHT_PART_SIZE == 0))){

  }
  else{
    _LCD_Set_Pixel(x, y, color);
  }
}

/********************************************************************
 * 名称 : _LCD_Show_Curve_Compensation
 * 功能 : 插值补偿 (不建议用户使用)
 * 输入 : 曲线 x1,y1,x2,y2,color
 * 输出 : 无 
 ***********************************************************************/
inline void _LCD_Show_Curve_Compensation(Lcd_Curve* Lcd_Curve1, u16 x1, u16 y1,
  u16 x2, u16 y2_before, u16 color){
  int i, j = 0;
  int x_different = (int)x2 - x1;
  int y_different = (int)y2_before - y1;
  int y_now = y1;
  int count = y_different / x_different;
  for (i = 0; i < x_different; i++){
    if (count > 0){
      for (j = 0; j < (count + 1) / 2; j++){
        y_now = y_different * i / x_different + y1 + j;
        _LCD_Set_Curve_Pixel(Lcd_Curve1, x1, y_now, color);
      }
      x1++;
      for (j = count / 2; j <= count; j++){
        y_now = y_different * i / x_different + y1 + j;
        _LCD_Set_Curve_Pixel(Lcd_Curve1, x1, y_now, color);
      }
    }
    else if (count < 0){
      for (j = 0; j > (count - 1) / 2; j--){
        y_now = y_different * i / x_different + y1 + j;
        _LCD_Set_Curve_Pixel(Lcd_Curve1, x1, y_now, color);
      }
      x1++;
      for (j = count / 2; j >= count; j--){
        y_now = y_different * i / x_different + y1 + j;
        _LCD_Set_Curve_Pixel(Lcd_Curve1, x1, y_now, color);
      }
    }
    else{
      y_now = y_different * i / x_different + y1;
      _LCD_Set_Curve_Pixel(Lcd_Curve1, x1, y_now, color);
      x1++;
    }
  }

  if (y_different > 0){
    for (i = y_now; i < y2_before; i++){
      _LCD_Set_Curve_Pixel(Lcd_Curve1, x1, i, color);
    }
  }
  else if (y_different < 0){
    for (i = y_now; i > y2_before; i--){
      _LCD_Set_Curve_Pixel(Lcd_Curve1, x1, i, color);
    }
  }

}

/********************************************************************
 * 名称 : LCD_Curve_Show
 * 功能 : 曲线显示
 * 输入 : 曲线  y轴 间隔大小
 * 输出 : 无 
 ***********************************************************************/
void LCD_Curve_Show(Lcd_Curve* Lcd_Curve1, u16 y2, u8 step){
  if (not y2){
    y2 += 1;
  }u16 len = Lcd_Curve1->End_x - Lcd_Curve1->Start_x;
  u16 y3 = (Lcd_Curve1->Curve_last_y)[((Lcd_Curve1->Now_x) + step + len) % len];
  u16 x1 = ((Lcd_Curve1->Now_x) - step + len) % len;
  u16 y1_after = (Lcd_Curve1->Curve_last_y)[x1];
  u16 y2_before = (Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)];
  u16 color = Lcd_Curve1->point_color;
  static u16 y1_before;
  if (y2 >= 256){
    y2 = 255;
  }
  if (Lcd_Curve1->finish_flag != 1){
    if ((Lcd_Curve1->Now_x) == 0){
      _LCD_Set_Curve_Pixel(Lcd_Curve1, (Lcd_Curve1->Now_x) + Lcd_Curve1
                           ->Start_x, (Lcd_Curve1->Curve_last_y)[(Lcd_Curve1
                           ->Now_x)] + Lcd_Curve1->Start_y, BACK_COLOR);
      _LCD_Show_Curve_Compensation(Lcd_Curve1, (Lcd_Curve1->Now_x) + Lcd_Curve1
                                   ->Start_x, y2_before + Lcd_Curve1->Start_y, 
                                   (Lcd_Curve1->Now_x) + step + Lcd_Curve1
                                   ->Start_x, y3 + Lcd_Curve1->Start_y,
                                   BACK_COLOR);
      (Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)] = y2;
      _LCD_Set_Curve_Pixel(Lcd_Curve1, (Lcd_Curve1->Now_x) + Lcd_Curve1
                           ->Start_x, y2 + Lcd_Curve1->Start_y, color);
    }
    else{
      /*首先清除数据*/
      _LCD_Set_Curve_Pixel(Lcd_Curve1, (Lcd_Curve1->Now_x + step + Lcd_Curve1
                           ->Start_x), y3 + Lcd_Curve1->Start_y, BACK_COLOR);
      _LCD_Show_Curve_Compensation(Lcd_Curve1, (Lcd_Curve1->Now_x + Lcd_Curve1
                                   ->Start_x), y2_before + Lcd_Curve1->Start_y,
                                   (Lcd_Curve1->Now_x) + step + Lcd_Curve1
                                   ->Start_x, y3 + Lcd_Curve1->Start_y,
                                   BACK_COLOR);
      /*再添加数据*/
      (Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)] = y2;
      _LCD_Set_Curve_Pixel(Lcd_Curve1, (Lcd_Curve1->Now_x) + Lcd_Curve1
                           ->Start_x, y2 + Lcd_Curve1->Start_y, color);
      /*写入现有数据*/
      _LCD_Show_Curve_Compensation(Lcd_Curve1, (Lcd_Curve1->Now_x) - step +
                                   Lcd_Curve1->Start_x, y1_after + Lcd_Curve1
                                   ->Start_y, (Lcd_Curve1->Now_x) + Lcd_Curve1
                                   ->Start_x, y2 + Lcd_Curve1->Start_y, color);
    }
    y1_before = y2_before;
    (Lcd_Curve1->Now_x) += step;
    if (Lcd_Curve1->Now_x >= len){
      Lcd_Curve1->finish_flag = 1;
    }
  }
}

/********************************************************************
 * 名称 : LCD_Curve_Init
 * 功能 : 曲线初始化
 * 输入 : 曲线  开始位置x 开始位置y 颜色
 * 输出 : 无 
 ***********************************************************************/
void LCD_Curve_Init(Lcd_Curve* Lcd_Curve1, u16 start_x1, u16 start_y1, u16
                    color){
  Lcd_Curve1->Start_x = start_x1;
  Lcd_Curve1->Start_y = start_y1;
  Lcd_Curve1->End_x = start_x1 + LCD_CURVE_WIDTH;
  Lcd_Curve1->End_y = start_y1 + LCD_CURVE_HEIGHT;
  Lcd_Curve1->point_color = color;
  Sequeue_Init(&(Lcd_Curve1->LCD_Curve_Buffer), _LCD_Curve_Buffer,
               LCD_CURVE_BUFFER_LEN);
  u16 i;
  for (i = Lcd_Curve1->Start_y; i <= Lcd_Curve1->End_y; i++){
    _LCD_Set_Pixel(Lcd_Curve1->Start_x - 1, i, WHITE);
    _LCD_Set_Pixel(Lcd_Curve1->End_x + 1, i, WHITE);
  }
  for (i = Lcd_Curve1->Start_x; i <= Lcd_Curve1->End_x; i++){
    _LCD_Set_Pixel(i, Lcd_Curve1->Start_y - 1, WHITE);
    _LCD_Set_Pixel(i, Lcd_Curve1->End_y + 1, WHITE);
  }
  LCD_Curve_Grid(Lcd_Curve1);
}

/********************************************************************
 * 名称 : LCD_Clear_Curve
 * 功能 : 清除曲线
 * 输入 : 曲线  间隔大小
 * 输出 : 无 
 ***********************************************************************/
void LCD_Clear_Curve(Lcd_Curve* Lcd_Curve1, u8 step){
  u16 i = 0;
  u16 color = Lcd_Curve1->point_color;
  Lcd_Curve1->point_color = BACK_COLOR;
  Lcd_Curve1->finish_flag = 0;
  (Lcd_Curve1->Now_x) = 0;
  for (i = 0; i < Lcd_Curve1->End_x - Lcd_Curve1->Start_x; i += step){
    LCD_Curve_Show(Lcd_Curve1, 0, step);
  }
  LCD_Curve_Init(Lcd_Curve1, Lcd_Curve1->Start_x, Lcd_Curve1->Start_y, color);
}

/********************************************************************
 * 名称 : LCD_Curve_Grid
 * 功能 : 曲线画标志线
 * 输入 : 曲线
 * 输出 : 无 
 ***********************************************************************/
void LCD_Curve_Grid(Lcd_Curve* Lcd_Curve1){
  u16 i, j;
  int k;
  for (j = HEIGHT_PART_SIZE; j < LCD_CURVE_HEIGHT; j += HEIGHT_PART_SIZE){
    for (i = 0; i < WIDTH_EDGE; i++){

      _LCD_Set_Pixel(i + Lcd_Curve1->Start_x, Lcd_Curve1->Start_y + j, WHITE);
      _LCD_Set_Pixel(LCD_CURVE_WIDTH - i + Lcd_Curve1->Start_x, Lcd_Curve1
                     ->Start_y + j, WHITE);
    }
  }
  for (j = HEIGHT_PART_SIZE; j < LCD_CURVE_HEIGHT; j += HEIGHT_PART_SIZE){
    for (i = WIDTH_PART_SIZE; i < LCD_CURVE_WIDTH; i += WIDTH_PART_SIZE){

      _LCD_Set_Pixel(i + Lcd_Curve1->Start_x, Lcd_Curve1->Start_y + j, WHITE);
    }
  }
  for (i = WIDTH_PART_SIZE; i < LCD_CURVE_WIDTH; i += WIDTH_PART_SIZE){
    for (j = 0; j < HEIGHT_EDGE; j++){
      _LCD_Set_Pixel(i + Lcd_Curve1->Start_x, Lcd_Curve1->Start_y + j, WHITE);
      _LCD_Set_Pixel(i + Lcd_Curve1->Start_x, Lcd_Curve1->Start_y +
                     LCD_CURVE_HEIGHT - j, WHITE);
    }
  }
}

/********************************************************************
 * 名称 : _LCD_Set_Curve_Pixel
 * 功能 : 设置曲线像素点 (不建议用户使用)
 * 输入 : 曲线 x,y,color
 * 输出 : 无 
 ***********************************************************************/
void inline _LCD_Set_Curve_Pixel_DMA(Lcd_Curve* Lcd_Curvel1, u16 x, u16 y, u16
                                     color){
  u16 x1 = x - Lcd_Curvel1->Start_x;
  u16 y1 = y - Lcd_Curvel1->Start_y;
  if ((y1 % HEIGHT_PART_SIZE == 0 and(x1 < WIDTH_EDGE or x1 > LCD_CURVE_WIDTH -
      WIDTH_EDGE or x1 % WIDTH_PART_SIZE == 0))or(x1 % WIDTH_PART_SIZE == 0 and
      (y1 < HEIGHT_EDGE or y1 > LCD_CURVE_HEIGHT - HEIGHT_EDGE or y1 %
      HEIGHT_PART_SIZE == 0))){

  }
  else{
    _LCD_Set_Pixel_DMA(x, y, color);
  }
}



inline void _LCD_Show_Curve_Compensation_DMA(Lcd_Curve* Lcd_Curve1, u16 x1, u16
  y1, u16 x2, u16 y2_before, u16 color){
  int i, j = 0;
  int x_different = (int)x2 - x1;
  int y_different = (int)y2_before - y1;
  int y_now = y1;
  int count = y_different / x_different;
  for (i = 0; i < x_different; i++){
    if (count > 0){
      for (j = 0; j < (count + 1) / 2; j++){
        y_now = y_different * i / x_different + y1 + j;
        _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, x1, y_now, color);
      }
      x1++;
      for (j = count / 2; j <= count; j++){
        y_now = y_different * i / x_different + y1 + j;
        _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, x1, y_now, color);
      }
    }
    else if (count < 0){
      for (j = 0; j > (count - 1) / 2; j--){
        y_now = y_different * i / x_different + y1 + j;
        _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, x1, y_now, color);
      }
      x1++;
      for (j = count / 2; j >= count; j--){
        y_now = y_different * i / x_different + y1 + j;
        _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, x1, y_now, color);
      }
    }
    else{
      y_now = y_different * i / x_different + y1;
      _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, x1, y_now, color);
      x1++;
    }
  }

  if (y_different > 0){
    for (i = y_now; i < y2_before; i++){
      _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, x1, i, color);
    }
  }
  else if (y_different < 0){
    for (i = y_now; i > y2_before; i--){
      _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, x1, i, color);
    }
  }

}


void LCD_Curve_Show_DMA(Lcd_Curve* Lcd_Curve1, u16 y2, u8 step){
  if (not y2){
    y2 += 1;
  }u16 len = Lcd_Curve1->End_x - Lcd_Curve1->Start_x;
  u16 y3 = (Lcd_Curve1->Curve_last_y)[((Lcd_Curve1->Now_x) + step + len) % len];
  u16 x1 = ((Lcd_Curve1->Now_x) - step + len) % len;
  u16 y1_after = (Lcd_Curve1->Curve_last_y)[x1];
  u16 y2_before = (Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)];
  u16 color = Lcd_Curve1->point_color;
  static u16 y1_before;
  if (y2 >= 256){
    y2 = 255;
  }
  if (Lcd_Curve1->finish_flag != 1){
    if ((Lcd_Curve1->Now_x) == 0){
      _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, (Lcd_Curve1->Now_x) + Lcd_Curve1
                               ->Start_x, (Lcd_Curve1->Curve_last_y)[
                               (Lcd_Curve1->Now_x)] + Lcd_Curve1->Start_y,
                               BACK_COLOR);
      _LCD_Show_Curve_Compensation_DMA(Lcd_Curve1, (Lcd_Curve1->Now_x) +
                                       Lcd_Curve1->Start_x, y2_before +
                                       Lcd_Curve1->Start_y, (Lcd_Curve1->Now_x)
                                       + step + Lcd_Curve1->Start_x, y3 +
                                       Lcd_Curve1->Start_y, BACK_COLOR);
      (Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)] = y2;
      _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, (Lcd_Curve1->Now_x) + Lcd_Curve1
                               ->Start_x, y2 + Lcd_Curve1->Start_y, color);
    }
    else{
      /*首先清除数据*/
      _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, (Lcd_Curve1->Now_x + step +
                               Lcd_Curve1->Start_x), y3 + Lcd_Curve1->Start_y,
                               BACK_COLOR);
      _LCD_Show_Curve_Compensation_DMA(Lcd_Curve1, (Lcd_Curve1->Now_x +
                                       Lcd_Curve1->Start_x), y2_before +
                                       Lcd_Curve1->Start_y, (Lcd_Curve1->Now_x)
                                       + step + Lcd_Curve1->Start_x, y3 +
                                       Lcd_Curve1->Start_y, BACK_COLOR);
      /*再添加数据*/
      (Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)] = y2;
      _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, (Lcd_Curve1->Now_x) + Lcd_Curve1
                               ->Start_x, y2 + Lcd_Curve1->Start_y, color);
      /*写入现有数据*/
      _LCD_Show_Curve_Compensation_DMA(Lcd_Curve1, (Lcd_Curve1->Now_x) - step +
                                       Lcd_Curve1->Start_x, y1_after +
                                       Lcd_Curve1->Start_y, (Lcd_Curve1->Now_x)
                                       + Lcd_Curve1->Start_x, y2 + Lcd_Curve1
                                       ->Start_y, color);
    }
    y1_before = y2_before;
    (Lcd_Curve1->Now_x) += step;
    if (Lcd_Curve1->Now_x >= len){
      Lcd_Curve1->finish_flag = 1;
    }
  }
}
