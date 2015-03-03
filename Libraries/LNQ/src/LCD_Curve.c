#include "LCD_Curve.h"
u8 _LCD_Curve_Buffer[LCD_CURVE_BUFFER_LEN];
/********************************************************************
 * ���� : _LCD_Set_Curve_Pixel
 * ���� : �����������ص� (�������û�ʹ��)
 * ���� : ���� x,y,color
 * ��� : �� 
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
 * ���� : _LCD_Show_Curve_Compensation
 * ���� : ��ֵ���� (�������û�ʹ��)
 * ���� : ���� x1,y1,x2,y2,color
 * ��� : �� 
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
 * ���� : LCD_Curve_Show
 * ���� : ������ʾ
 * ���� : ����  y�� �����С
 * ��� : �� 
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
      /*�����������*/
      _LCD_Set_Curve_Pixel(Lcd_Curve1, (Lcd_Curve1->Now_x + step + Lcd_Curve1
                           ->Start_x), y3 + Lcd_Curve1->Start_y, BACK_COLOR);
      _LCD_Show_Curve_Compensation(Lcd_Curve1, (Lcd_Curve1->Now_x + Lcd_Curve1
                                   ->Start_x), y2_before + Lcd_Curve1->Start_y,
                                   (Lcd_Curve1->Now_x) + step + Lcd_Curve1
                                   ->Start_x, y3 + Lcd_Curve1->Start_y,
                                   BACK_COLOR);
      /*���������*/
      (Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)] = y2;
      _LCD_Set_Curve_Pixel(Lcd_Curve1, (Lcd_Curve1->Now_x) + Lcd_Curve1
                           ->Start_x, y2 + Lcd_Curve1->Start_y, color);
      /*д����������*/
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
 * ���� : LCD_Curve_Init
 * ���� : ���߳�ʼ��
 * ���� : ����  ��ʼλ��x ��ʼλ��y ��ɫ
 * ��� : �� 
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
 * ���� : LCD_Clear_Curve
 * ���� : �������
 * ���� : ����  �����С
 * ��� : �� 
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
 * ���� : LCD_Curve_Grid
 * ���� : ���߻���־��
 * ���� : ����
 * ��� : �� 
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
 * ���� : _LCD_Set_Curve_Pixel
 * ���� : �����������ص� (�������û�ʹ��)
 * ���� : ���� x,y,color
 * ��� : �� 
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
      /*�����������*/
      _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, (Lcd_Curve1->Now_x + step +
                               Lcd_Curve1->Start_x), y3 + Lcd_Curve1->Start_y,
                               BACK_COLOR);
      _LCD_Show_Curve_Compensation_DMA(Lcd_Curve1, (Lcd_Curve1->Now_x +
                                       Lcd_Curve1->Start_x), y2_before +
                                       Lcd_Curve1->Start_y, (Lcd_Curve1->Now_x)
                                       + step + Lcd_Curve1->Start_x, y3 +
                                       Lcd_Curve1->Start_y, BACK_COLOR);
      /*���������*/
      (Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)] = y2;
      _LCD_Set_Curve_Pixel_DMA(Lcd_Curve1, (Lcd_Curve1->Now_x) + Lcd_Curve1
                               ->Start_x, y2 + Lcd_Curve1->Start_y, color);
      /*д����������*/
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
