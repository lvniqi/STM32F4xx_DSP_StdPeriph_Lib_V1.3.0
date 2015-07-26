#ifndef LCD_CURVE_H_
  #define LCD_CURVE_H_
  /*
   * LCD_CURVE.H
   *
   *  Created on: 2015年1月13日
   *      Author: lvniqi
   *      Email: lvniqi@gmail.com
   */
  #include "LCD.h"
  #define LCD_CURVE_BUFFER_LEN 400/*液晶缓冲长度*/
  typedef struct _Lcd_Curve{
    int Now_x; //现在位置
    int Now_y; //现在位置
    int Start_x; //起始x
    int Start_y; //起始y
    int End_x; //结束x
    int End_y; //结束y
    u8 Curve_last_y[480]; //上次的y
    u8 finish_flag; //单次显示完成标志
    u16 point_color; //画笔颜色
    u8_sequeue LCD_Curve_Buffer; //曲线缓冲
  } Lcd_Curve;
  #define LCD_Curve_Go_0(Lcd_Curve1)\
  {       \
    if(!((Lcd_Curve1)->finish_flag)){\
      LCD_Curve_Grid((Lcd_Curve1));\
    }\
  ((Lcd_Curve1)->finish_flag) = 0;        \
  ((Lcd_Curve1)->Now_x) = 0;\
  }
  //曲线宽度
  #define LCD_CURVE_WIDTH 400
  //宽度分割数
  #define WIDTH_DIV 5
  //一个间隔宽度
  #define WIDTH_PART_SIZE (LCD_CURVE_WIDTH/WIDTH_DIV)
  //宽度标记宽度
  #define WIDTH_EDGE 4
  //曲线高度
  #define LCD_CURVE_HEIGHT 256
  //标志线分割数
  #define HEIGHT_DIV 4
  //一个间隔宽度
  #define HEIGHT_PART_SIZE (LCD_CURVE_HEIGHT/HEIGHT_DIV)
  //高度标记宽度
  #define HEIGHT_EDGE 4
  /*变量 缓冲数组源*/
  extern u8 _LCD_Curve_Buffer[LCD_CURVE_BUFFER_LEN];

  /********************************************************************
   * 名称 : LCD_Curve_Show
   * 功能 : 曲线显示
   * 输入 : 曲线  y轴 间隔大小
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_Curve_Show(Lcd_Curve *Lcd_Curve1, u16 y, u8 setp);
  /********************************************************************
   * 名称 : LCD_Clear_Curve
   * 功能 : 清除曲线
   * 输入 : 曲线  间隔大小
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_Clear_Curve(Lcd_Curve *Lcd_Curve1, u8 setp);
  /********************************************************************
   * 名称 : LCD_Curve_Init
   * 功能 : 曲线初始化
   * 输入 : 曲线  开始位置x 开始位置y 颜色
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_Curve_Init(Lcd_Curve *Lcd_Curve1, u16 start_x1, u16 start_y1,
    u16 color);
  extern void LCD_Curve_Grid(Lcd_Curve *Lcd_Curve1);
  extern void LCD_Curve_Show_DMA(Lcd_Curve *Lcd_Curve1, u16 y, u8 setp);
#endif
