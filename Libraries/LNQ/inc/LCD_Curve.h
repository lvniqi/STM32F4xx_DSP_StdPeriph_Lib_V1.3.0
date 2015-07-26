#ifndef LCD_CURVE_H_
  #define LCD_CURVE_H_
  /*
   * LCD_CURVE.H
   *
   *  Created on: 2015��1��13��
   *      Author: lvniqi
   *      Email: lvniqi@gmail.com
   */
  #include "LCD.h"
  #define LCD_CURVE_BUFFER_LEN 400/*Һ�����峤��*/
  typedef struct _Lcd_Curve{
    int Now_x; //����λ��
    int Now_y; //����λ��
    int Start_x; //��ʼx
    int Start_y; //��ʼy
    int End_x; //����x
    int End_y; //����y
    u8 Curve_last_y[480]; //�ϴε�y
    u8 finish_flag; //������ʾ��ɱ�־
    u16 point_color; //������ɫ
    u8_sequeue LCD_Curve_Buffer; //���߻���
  } Lcd_Curve;
  #define LCD_Curve_Go_0(Lcd_Curve1)\
  {       \
    if(!((Lcd_Curve1)->finish_flag)){\
      LCD_Curve_Grid((Lcd_Curve1));\
    }\
  ((Lcd_Curve1)->finish_flag) = 0;        \
  ((Lcd_Curve1)->Now_x) = 0;\
  }
  //���߿��
  #define LCD_CURVE_WIDTH 400
  //��ȷָ���
  #define WIDTH_DIV 5
  //һ��������
  #define WIDTH_PART_SIZE (LCD_CURVE_WIDTH/WIDTH_DIV)
  //��ȱ�ǿ��
  #define WIDTH_EDGE 4
  //���߸߶�
  #define LCD_CURVE_HEIGHT 256
  //��־�߷ָ���
  #define HEIGHT_DIV 4
  //һ��������
  #define HEIGHT_PART_SIZE (LCD_CURVE_HEIGHT/HEIGHT_DIV)
  //�߶ȱ�ǿ��
  #define HEIGHT_EDGE 4
  /*���� ��������Դ*/
  extern u8 _LCD_Curve_Buffer[LCD_CURVE_BUFFER_LEN];

  /********************************************************************
   * ���� : LCD_Curve_Show
   * ���� : ������ʾ
   * ���� : ����  y�� �����С
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_Curve_Show(Lcd_Curve *Lcd_Curve1, u16 y, u8 setp);
  /********************************************************************
   * ���� : LCD_Clear_Curve
   * ���� : �������
   * ���� : ����  �����С
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_Clear_Curve(Lcd_Curve *Lcd_Curve1, u8 setp);
  /********************************************************************
   * ���� : LCD_Curve_Init
   * ���� : ���߳�ʼ��
   * ���� : ����  ��ʼλ��x ��ʼλ��y ��ɫ
   * ��� : �� 
   ***********************************************************************/
  extern void LCD_Curve_Init(Lcd_Curve *Lcd_Curve1, u16 start_x1, u16 start_y1,
    u16 color);
  extern void LCD_Curve_Grid(Lcd_Curve *Lcd_Curve1);
  extern void LCD_Curve_Show_DMA(Lcd_Curve *Lcd_Curve1, u16 y, u8 setp);
#endif
