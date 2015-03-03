#include "LCD_Tools.h"
/********************************************************************
 * ���� : LCD_Show_Rect
 * ���� : ������
 * ���� : x , y �� lenth��width�� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_Show_Rect(u16 x, u16 y, u16 lenth, u16 width, u16 point_color){
  u16 i, j;
  for (i = 0; i < lenth; i++){
    _LCD_Set_Pixel(x + i, y + width, point_color);
    _LCD_Set_Pixel(x + i, y, point_color);
  }
  for (j = 0; j < width; j++){
    _LCD_Set_Pixel(x + lenth, y + j, point_color);
    _LCD_Set_Pixel(x, y + j, point_color);
  }
}

/********************************************************************
 * ���� : LCD_Clear_Rect
 * ���� : �������
 * ���� : x , y �� lenth��width�� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_Clear_Rect(u16 x, u16 y, u16 lenth, u16 width, u16 point_color){
  u16 i, j;
  for (i = 0; i < lenth; i++){
    for (j = 0; j < width; j++){
      _LCD_Set_Pixel(x + i, y + j, point_color);
    }
  }
}
