#ifndef LCD_TOOLS_H_
  #define LCD_TOOLS_H_
  /*
   * LCD_TOOLS.H
   *
   *  Created on: 2015年1月13日
   *      Author: lvniqi
   *      Email: lvniqi@gmail.com
   */
  #include "LCD.h"
  /********************************************************************
   * 名称 : LCD_Show_Rect
   * 功能 : 画矩形
   * 输入 : x , y ， lenth，width， 颜色
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_Show_Rect(u16 x, u16 y, u16 lenth, u16 width, u16 point_color)
    ;
  /********************************************************************
   * 名称 : LCD_Clear_Rect
   * 功能 : 清除矩形
   * 输入 : x , y ， lenth，width， 颜色
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_Clear_Rect(u16 x, u16 y, u16 lenth, u16 width, u16
    point_color);
#endif
