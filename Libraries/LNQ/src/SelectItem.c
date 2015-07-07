#include "SelectItem.h"
/*main*/
void SelectItem_Init(SelectItem* p,u8 start_x,u8 y){
  p->start_x = start_x;
  p->end_x = start_x+4;
  p->y = y;
  p->next = NULL;
  LCD_STRING t;
  t.type = _LCD_STRING_ASCII;
  t.string.ascii = "NULL";
  p->string = t;
  p->isActive = false;
  p->isShow = false;
  p->color = YELLOW;
  p->back_color = RED;
}
void SelectItem_SetFunc(SelectItem* p,SelectItemFunc func){
  p->func = func;
}
/*action*/
void SelectItem_SetActive(SelectItem* p,bool isActive){
  if(isActive && p->isShow){
    u16 temp_color = BACK_COLOR;
    BACK_COLOR = p->back_color;
    LCD_ShowStringBig_Union(p->start_x,p->y,LCD_STRING_LEFT,p->string,p->color);
    BACK_COLOR = temp_color;
    p->isActive = true;
    if(p->func){
      p->func(p);
    }
  }
  else{
    LCD_ShowStringBig_Union(p->start_x,p->y,LCD_STRING_LEFT,p->string,p->color);
    p->isActive = false;
  }
}

void SelectItem_Show(SelectItem* p){
  p->isShow = true;
  LCD_ShowStringBig_Union(p->start_x,p->y,LCD_STRING_LEFT,p->string,p->color);
  SelectItem_SetActive(p,p->isActive);
}
void SelectItem_ShowRect(SelectItem* p){
  int x1 = p->start_x;
  int x2 = p->end_x;
  LCD_Show_Rect(x1*16-1,p->y*32,(x2-x1)*16,32,p->color);
}
void SelectItem_Hide(SelectItem* p){
  LCD_ShowStringBig_Union(p->start_x,p->y,LCD_STRING_LEFT,p->string,BACK_COLOR);
}
/*string*/
void SelectItem_SetString(SelectItem *p,LCD_STRING string){
  if(p->isShow){
    SelectItem_Hide(p);
  }
  p->string = string;
  if(string.type == _LCD_STRING_CHINESE){
    p->end_x = p->start_x + string.string.chinese.len*2;
  }else{
    p->end_x = p->start_x + strlen(string.string.ascii);
  }
  if(p->isShow){
    SelectItem_Show(p);
  }
}