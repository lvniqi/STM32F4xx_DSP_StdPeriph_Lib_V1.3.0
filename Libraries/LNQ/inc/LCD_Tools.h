#ifndef LCD_TOOLS_H_
  #define LCD_TOOLS_H_
  /*
   * LCD_TOOLS.H
   *
   *  Created on: 2015��1��13��
   *      Author: lvniqi
   *      Email: lvniqi@gmail.com
   */
  #include "LCD.h"
  /********************************************************************
   * ���� : LCD_Show_Rect
   * ���� : ������
   * ���� : x , y �� lenth��width�� ��ɫ
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_Show_Rect(u16 x, u16 y, u16 lenth, u16 width, u16 point_color)
    ;
  /********************************************************************
   * ���� : LCD_Clear_Rect
   * ���� : �������
   * ���� : x , y �� lenth��width�� ��ɫ
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_Clear_Rect(u16 x, u16 y, u16 lenth, u16 width, u16
    point_color);
#endif
