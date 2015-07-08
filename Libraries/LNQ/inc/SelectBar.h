#ifndef SELSECTBAR_H_
  #define SELSECTBAR_H_
  #include "LCD.h"
  #include "stdbool.h"
  #include "SelectItem.h"
  #include "malloc.h"
  typedef struct _SelectBar{
    u8 end_x;
    u8 start_x;
    u8 y;
    u8 item_y;
    u8 len;
    short pos;
    LCD_STRING* title;
    SelectItem* selectNode_Start;
    SelectItem* selectNode_Now;
    SelectItem* selectNode_End;
    bool isShow;
    u16 title_color;
    u16 color;
    u16 back_color;
    void (*func)(struct _SelectBar* p,int value) ;
  }SelectBar;
  typedef  void (*SelectBarFunc)(SelectBar* p,int value);
  /*main*/
  extern void SelectBar_Init(SelectBar* p,u8 start_x,u8 end_x,u8 y);
  extern void SelectBar_SetFunc(SelectBar* p,SelectBarFunc func);
  /*status*/
  #define SelectBar_IsShow(p) ((p)->isShow)
  #define SelectBar_GetPos(p) ((p)->pos)
  #define SelectBar_IsDisableNumShow(p) ((p)->isDisableNumShow)
  /*action*/
  extern void SelectBar_ShiftDown(SelectBar* p);
  extern void SelectBar_ShiftUp(SelectBar* p);
  extern void SelectBar_Show(SelectBar* p);
  extern void SelectBar_ShowRect(SelectBar* p);
  extern void SelectBar_Hide(SelectBar* p);
  /*color*/
  extern void SelectBar_SetColor(SelectBar* p,u16 value);
  extern void SelectBar_SetBackColor(SelectBar* p,u16 value);
  extern u16 SelectBar_GetColor(SelectBar* p);
  extern u16 SelectBar_GetBackColor(SelectBar* p);
  /*title*/
  extern void SelectBar_SetTitle(SelectBar* p,LCD_STRING title);
  /*node*/
  extern void SelectBar_AddSelect(SelectBar* p,LCD_STRING string);
  extern SelectItem* SelectBar_GetSelect(SelectBar* p,int n);
  extern void SelectBar_ClearSelect(SelectBar* p);
#endif