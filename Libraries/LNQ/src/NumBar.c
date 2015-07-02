#include "NumBar.h"
void NumBar_Init(NumBar* p,u8 start_x,u8 end_x,u8 y,int v_max,int v_min){
  p->start_x = start_x;
  p->end_x = end_x;
  p->y = y;
  p->value_max = v_max;
  p->value_min = v_min;
  p->value = 0;
  p->pos = 0;
  p->color = YELLOW;
  p->back_color = RED;
  p->func = NULL;
}
void NumBar_SetValue(NumBar* p,int value){
  p->value = value;
  LCD_ShowNumBig_L(p->start_x,p->end_x,p->y,p->value,p->color);
  if(p->func){
    p->func(value);
  }
}
//得到最大数字首位
u32 NumBar_GetFirst(NumBar* p){
  u32 temp=10;
  for(int i=0;i<p->pos;i++){
      temp*=10;
  }
  if(p->value >0){
    return (p->value%temp)/(temp/10);
  }else{
    return ((-p->value)%temp)/(temp/10);
  }
}
//修正pos
void NumBar_FixPos(NumBar* p,u16 len){
  if(p->pos >= len){
    if(len > 0){
      p->pos = len-1;
    }
    else{
      p->pos = 0;
    }
  }
}
//添加关注
void NumBar_Enable(NumBar* p){
  u16 len = Num_Len(p->value);
  NumBar_FixPos(p,len);
  u16 temp_color = BACK_COLOR;
  BACK_COLOR = p->back_color;
  LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
  BACK_COLOR = temp_color;
}
//清除关注
void NumBar_Disable(NumBar* p){
  LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
}
//左移
void NumBar_ShiftLeft(NumBar* p){
  u16 len = Num_Len(p->value);
  NumBar_FixPos(p,len);
  u16 temp_color = BACK_COLOR;
  LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
  BACK_COLOR = p->color;
  if(p->pos+1 < len){
    p->pos += 1;
  }
  LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
  BACK_COLOR = temp_color;
}
//右移
void NumBar_ShiftRight(NumBar* p){
  u16 len = Num_Len(p->value);
  NumBar_FixPos(p,len);
  u16 temp_color = BACK_COLOR;
  LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
  BACK_COLOR = p->color;
  if(p->pos > 0)
  {
    p->pos -= 1;
  }
  LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
  BACK_COLOR = temp_color;
}
//加数
void NumBar_Add(NumBar* p){
  u16 len = Num_Len(p->value);
  NumBar_FixPos(p,len);
  int temp = 1;
  for(int i=0;i<p->pos;i++){
    temp*=10;
  }
  if(temp + p->value < p->value_max){
    p->value += temp;
  }else{
    p->value = p->value_max;
  }
  LCD_ShowNumBig_L(p->start_x,p->end_x,p->y,p->value,p->color); 
  NumBar_Enable(p);
  if(p->func){
    p->func(p->value);
  }
}
//减数
void NumBar_Cut(NumBar* p){
  u16 len = Num_Len(p->value);
  NumBar_FixPos(p,len);
  int temp = 1;
  for(int i=0;i<p->pos;i++){
    temp*=10;
  }
  if(p->value_min <= p->value-temp){
    p->value-= temp;
  }else{
    p->value = p->value_min;
  }
  LCD_ShowNumBig_L(p->start_x,p->end_x,p->y,p->value,p->color); 
  NumBar_Enable(p);
  if(p->func){
    p->func(p->value);
  }
}