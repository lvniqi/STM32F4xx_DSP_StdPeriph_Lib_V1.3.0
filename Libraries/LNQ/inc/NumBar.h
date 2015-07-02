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
    int value;
    int value_max;
    int value_min;
    u16 color;
    u16 back_color;
    NumBarFunc func;
  }NumBar;
  /*main*/
  extern void NumBar_Init(NumBar* p,u8 start_x,u8 end_x,u8 y,int v_max,int v_min);
  extern void NumBar_SetValue(NumBar* p,int value);
  extern void NumBar_setFunc(NumBar* p,NumBarFunc func);
  /*status*/
  extern bool NumBar_IsEnable(NumBar* p);
  extern int  NumBar_GetValue(NumBar* p);
  /*action*/
  extern void NumBar_Enable(NumBar* p);
  extern void NumBar_Disable(NumBar* p);
  extern void NumBar_ShiftLeft(NumBar* p);
  extern void NumBar_ShiftRight(NumBar* p);
  extern void NumBar_Add(NumBar* p);
  extern void NumBar_Cut(NumBar* p);
  /*color*/
  extern void NumBar_SetColor(NumBar* p,u16 value);
  extern void NumBar_SetBackColor(NumBar* p,u16 value);
  extern u16 NumBar_GetColor(NumBar* p);
  extern u16 NumBar_GetBackColor(NumBar* p);
#endif