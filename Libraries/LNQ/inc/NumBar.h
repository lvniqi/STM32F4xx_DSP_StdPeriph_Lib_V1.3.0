#ifndef NUMBAR_H_
  #define NUMBAR_H_
  #include "LCD.h"
  #include "stdbool.h"
  typedef  void (*NumBarFunc)(int value);
  typedef struct{
    u8 end_x;
    u8 start_x;
    u8 y;
    u8 pos;
    LCD_STRING ltag;
    LCD_STRING rtag;
    bool isActive;
    bool isShow;
    int value;
    int value_max;
    int value_min;
    u16 color;
    u16 back_color;
    u16 ltag_color;
    u16 rtag_color;
    NumBarFunc func;
  }NumBar;
  /*main*/
  extern void NumBar_Init(NumBar* p,u8 end_x,u8 y,int v_max,int v_min);
  extern void NumBar_SetValue(NumBar* p,int value);
  extern void NumBar_SetValue_GUI(NumBar* p);
  extern void NumBar_SetFunc(NumBar* p,NumBarFunc func);
  /*status*/
  #define NumBar_IsActive(p) ((p)->isActive)
  #define NumBar_IsShow(p) ((p)->isShow)
  #define NumBar_GetValue(p) ((p)->value)
  /*action*/
  extern void NumBar_SetActive(NumBar* p,bool isActive);
  extern void NumBar_ShiftLeft(NumBar* p);
  extern void NumBar_ShiftRight(NumBar* p);
  extern void NumBar_Add(NumBar* p);
  extern void NumBar_Cut(NumBar* p);
  extern void NumBar_Show(NumBar* p);
  extern void NumBar_ShowRect(NumBar* p);
  extern void NumBar_Hide(NumBar* p);
  /*color*/
  extern void NumBar_SetColor(NumBar* p,u16 value);
  extern void NumBar_SetBackColor(NumBar* p,u16 value);
  extern u16 NumBar_GetColor(NumBar* p);
  extern u16 NumBar_GetBackColor(NumBar* p);
  /*tag*/
  extern void NumBar_SetLTag(NumBar *p,LCD_STRING tag);
  extern void NumBar_SetRTag(NumBar *p,LCD_STRING tag);
#endif