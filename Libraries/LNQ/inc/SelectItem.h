#ifndef SELECTITEM_H_
  #define SELECTITEM_H_
  #include "LCD.h"
  #include "stdbool.h"
  typedef struct _SelectItem{
    u8 end_x;
    u8 start_x;
    u8 y;
    LCD_STRING string;
    bool isActive;
    bool isShow;
    u16 color;
    u16 back_color;
    void (*func)(struct _SelectItem* p);
    struct _SelectItem* next;
  }SelectItem;
  typedef  void (*SelectItemFunc)(SelectItem* p);
  /*main*/
  extern void SelectItem_Init(SelectItem* p,u8 start_x,u8 y);
  extern void SelectItem_SetFunc(SelectItem* p,SelectItemFunc func);
  /*status*/
  #define SelectItem_IsActive(p) ((p)->isActive)
  #define SelectItem_IsShow(p) ((p)->isShow)
  #define SelectItem_GetString(p) ((p)->string)
  /*action*/
  extern void SelectItem_SetActive(SelectItem* p,bool isActive);
  extern void SelectItem_Show(SelectItem* p);
  extern void SelectItem_ShowRect(SelectItem* p);
  extern void SelectItem_Hide(SelectItem* p);
  /*color*/
  extern void SelectItem_SetColor(SelectItem* p,u16 value);
  extern void SelectItem_SetBackColor(SelectItem* p,u16 value);
  extern u16 SelectItem_GetColor(SelectItem* p);
  extern u16 SelectItem_GetBackColor(SelectItem* p);
  /*string*/
  extern void SelectItem_SetString(SelectItem *p,LCD_STRING string);
#endif