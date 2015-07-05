#ifndef LCD_STRING_H_
  #define LCD_STRING_H_
  /*
   * LCD_STRING.H
   *
   *  Created on: 2015年1月13日
   *      Author: lvniqi
   *      Email: lvniqi@gmail.com
   */
  #include "LCD.h"
  #include "FONT.h"
  #include "string.h"
  typedef enum {
    LCD_STRING_LEFT,
    LCD_STRING_MID,
    LCD_STRING_RIGHT,
  }LCD_PLACE_TYPE;
  typedef enum{
    _LCD_STRING_ASCII,
    _LCD_STRING_CHINESE,
    _LCD_STRING_NULL,
  }LCD_STRING_TYPE;
  typedef struct{
    u8 start;
    u8 len;
  }LCD_STRING_CHINESE;
  typedef union{
    LCD_STRING_CHINESE chinese;
    char* ascii;
  }LCD_STRING_UNION;
  typedef struct{
    LCD_STRING_TYPE type;
    LCD_STRING_UNION string;
  }LCD_STRING;
  #define LCD_STRING_WIDTH (LCD_WIDTH/16)
  #define LCD_STRING_HIGHT (LCD_HIGHT/32)
  extern char LCD_BUFFER[100];

  /********************************************************************
   * 名称 : LCD_ShowChar
   * 功能 : 显示1608字符 
   * 输入 :  x y 字符 大小 颜色
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_ShowChar(u16 x, u16 y2, u8 num, u8 size, u16 color);
  /********************************************************************
   * 名称 : LCD_GetChar
   * 功能 : 输入数字
   * 输入 : x , y 大小
   * 输出 : 无 
   ***********************************************************************/
  extern u32 LCD_GetChar(u16 x, u16 y2, u8 size);
  /********************************************************************
   * 名称 : LCD_ShowString
   * 功能 : 显示1608字符串
   * 输入 :  x y  大小 字符串 颜色
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_ShowString(u16 x, u16 y, u8 size, char *p, u16 color);
  /********************************************************************
   * 名称 : LCD_ShowChinese
   * 功能 : 显示1608中文字符
   * 输入 :  x y  大小 中文位于asc4_1608数组中的位置 颜色
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_ShowChinese(u16 x, u16 y, u8 size, u8 num, u16 color);
  /********************************************************************
  * 名称 : LCD_ShowChineseBig
  * 功能 : 显示3216字符
  * 输入 :  x y  大小 字符位于asc_3216数组中的位置 颜色
  * 输出 : 无 
  ***********************************************************************/
  extern void LCD_ShowChineseBig(u16 x,u16 y,u8 num,u16 color);
  /********************************************************************
  * 名称 : LCD_ShowChineseStringBig
  * 功能 : 显示3216字符
  * 输入 :  x y  大小 字符位于asc_3216数组中的位置 颜色
  * 输出 : 无 
  ***********************************************************************/
  extern void LCD_ShowChineseStringBig(u16 x,u16 y,LCD_PLACE_TYPE type,u8 num,u8 len,u16 color);
  /********************************************************************
  * 名称 : LCD_ShowStringBig_Union
  * 功能 : 显示3216字符
  * 输入 :  x y  大小 字符位于asc_3216数组中的位置 颜色
  * 输出 : 无 
  ***********************************************************************/
  extern void LCD_ShowStringBig_Union(u16 x, u16 y,LCD_PLACE_TYPE type, LCD_STRING p, u16 color);
  /********************************************************************
   * 名称 : LCD_ShowCharBig
   * 功能 : 显示3216字符
   * 输入 :  x y  大小 字符位于asc_3216数组中的位置 颜色
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_ShowCharBig(u16 x, u16 y, u8 num, u16 color);
  /********************************************************************
   * 名称 : LCD_ShowNumBig
   * 功能 : 显示3216数字
   * 输入 :  x y  大小 数字 颜色
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_ShowStringBig(u16 x, u16 y,LCD_PLACE_TYPE type, char *p, u16 color);
  /********************************************************************
   * 名称 : LCD_ShowChinese
   * 功能 : 显示1608中文字符
   * 输入 :  x y  大小 中文位于asc4_1608数组中的位置 颜色
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_ShowNumBig(u16 x, u16 y,LCD_PLACE_TYPE type, long num, u16 color);
  extern void LCD_ShowNumBig_L(u16 x, u16 x_end, u16 y,long num, u16 color);
  /********************************************************************
   * 名称 : LCD_ShowCharBig_DMA
   * 功能 : 显示3216字符
   * 输入 :  x y  大小 字符位于asc_3216数组中的位置 颜色
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_ShowCharBig_DMA(u16 x, u16 y, u8 num, u16 color);
  extern void LCD_ShowStringBig_DMA(u16 x, u16 y, char *p, u16 color);
  extern void LCD_ShowNumBig_DMA(u16 x, u16 y, u32 num, u16 color);
  extern void LCD_ShowNumBig_L_DMA(u16 x, u16 x_end, u16 y, u32 num, u16 color);
  /*
  printf
  显示例子：
  sprintf(LCD_BUFFER,"asdsad:%d",i);
  LCD_ShowString(140,220,18,LCD_BUFFER);
   */
#endif
