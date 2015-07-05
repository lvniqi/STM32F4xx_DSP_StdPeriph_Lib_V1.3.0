#ifndef LCD_STRING_H_
  #define LCD_STRING_H_
  /*
   * LCD_STRING.H
   *
   *  Created on: 2015��1��13��
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
   * ���� : LCD_ShowChar
   * ���� : ��ʾ1608�ַ� 
   * ���� :  x y �ַ� ��С ��ɫ
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_ShowChar(u16 x, u16 y2, u8 num, u8 size, u16 color);
  /********************************************************************
   * ���� : LCD_GetChar
   * ���� : ��������
   * ���� : x , y ��С
   * ��� : �� 
   ***********************************************************************/
  extern u32 LCD_GetChar(u16 x, u16 y2, u8 size);
  /********************************************************************
   * ���� : LCD_ShowString
   * ���� : ��ʾ1608�ַ���
   * ���� :  x y  ��С �ַ��� ��ɫ
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_ShowString(u16 x, u16 y, u8 size, char *p, u16 color);
  /********************************************************************
   * ���� : LCD_ShowChinese
   * ���� : ��ʾ1608�����ַ�
   * ���� :  x y  ��С ����λ��asc4_1608�����е�λ�� ��ɫ
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_ShowChinese(u16 x, u16 y, u8 size, u8 num, u16 color);
  /********************************************************************
  * ���� : LCD_ShowChineseBig
  * ���� : ��ʾ3216�ַ�
  * ���� :  x y  ��С �ַ�λ��asc_3216�����е�λ�� ��ɫ
  * ��� : �� 
  ***********************************************************************/
  extern void LCD_ShowChineseBig(u16 x,u16 y,u8 num,u16 color);
  /********************************************************************
  * ���� : LCD_ShowChineseStringBig
  * ���� : ��ʾ3216�ַ�
  * ���� :  x y  ��С �ַ�λ��asc_3216�����е�λ�� ��ɫ
  * ��� : �� 
  ***********************************************************************/
  extern void LCD_ShowChineseStringBig(u16 x,u16 y,LCD_PLACE_TYPE type,u8 num,u8 len,u16 color);
  /********************************************************************
  * ���� : LCD_ShowStringBig_Union
  * ���� : ��ʾ3216�ַ�
  * ���� :  x y  ��С �ַ�λ��asc_3216�����е�λ�� ��ɫ
  * ��� : �� 
  ***********************************************************************/
  extern void LCD_ShowStringBig_Union(u16 x, u16 y,LCD_PLACE_TYPE type, LCD_STRING p, u16 color);
  /********************************************************************
   * ���� : LCD_ShowCharBig
   * ���� : ��ʾ3216�ַ�
   * ���� :  x y  ��С �ַ�λ��asc_3216�����е�λ�� ��ɫ
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_ShowCharBig(u16 x, u16 y, u8 num, u16 color);
  /********************************************************************
   * ���� : LCD_ShowNumBig
   * ���� : ��ʾ3216����
   * ���� :  x y  ��С ���� ��ɫ
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_ShowStringBig(u16 x, u16 y,LCD_PLACE_TYPE type, char *p, u16 color);
  /********************************************************************
   * ���� : LCD_ShowChinese
   * ���� : ��ʾ1608�����ַ�
   * ���� :  x y  ��С ����λ��asc4_1608�����е�λ�� ��ɫ
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_ShowNumBig(u16 x, u16 y,LCD_PLACE_TYPE type, long num, u16 color);
  extern void LCD_ShowNumBig_L(u16 x, u16 x_end, u16 y,long num, u16 color);
  /********************************************************************
   * ���� : LCD_ShowCharBig_DMA
   * ���� : ��ʾ3216�ַ�
   * ���� :  x y  ��С �ַ�λ��asc_3216�����е�λ�� ��ɫ
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_ShowCharBig_DMA(u16 x, u16 y, u8 num, u16 color);
  extern void LCD_ShowStringBig_DMA(u16 x, u16 y, char *p, u16 color);
  extern void LCD_ShowNumBig_DMA(u16 x, u16 y, u32 num, u16 color);
  extern void LCD_ShowNumBig_L_DMA(u16 x, u16 x_end, u16 y, u32 num, u16 color);
  /*
  printf
  ��ʾ���ӣ�
  sprintf(LCD_BUFFER,"asdsad:%d",i);
  LCD_ShowString(140,220,18,LCD_BUFFER);
   */
#endif
