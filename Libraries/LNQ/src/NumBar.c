#include "NumBar.h"
/*other*/
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
/*main*/
void NumBar_Init(NumBar* p,u8 end_x,u8 y,int v_max,int v_min){
  p->start_x = end_x-Num_Len(v_max)-1;
  p->end_x = end_x;
  p->y = y;
  p->value_max = v_max;
  p->value_min = v_min;
  p->value = v_min;
  p->pos = 0;
  p->color = YELLOW;
  p->back_color = RED;
  p->func = NULL;
  p->isActive = false;
  LCD_STRING t;
  t.type = _LCD_STRING_NULL;
  p->ltag = t;
  p->rtag = t;
  p->ltag_color = WHITE;
  p->rtag_color = WHITE;
  p->isShow = false;
  p->print_func = NumBar_ShowValue;
}
void NumBar_SetValue(NumBar* p,int value){
  if(p->value_max>= value && value >=p->value_min){
    p->value = value;
  }
  if(p->isShow){
    if(p->func){
      p->func(p,value);
    }
    if(p->print_func){
      p->print_func(p);
      if(p->print_func == NumBar_ShowValue){
        NumBar_SetActive(p,p->isActive);
      }
    }
    
  }
}

void NumBar_SetValue_GUI(NumBar* p){
  if(p->isShow){
    int data = 0;
    int x = p->end_x - 1;
    int len = p->end_x-p->start_x;
    u16 temp_color = BACK_COLOR;
    BACK_COLOR = p->back_color;
    LCD_ShowNumBig_L(p->start_x,p->end_x,p->y,p->value,p->color);
    int i=0;
    for(;i<len;i++){
      while(!MAIN_KEY.keysign);
      MAIN_KEY.keysign = 0;
      if(MAIN_KEY.keycode >10){
        break;
      }
      else if(MAIN_KEY.keycode == 10){
        data = -data;
        LCD_ShowNumBig_L(p->start_x,p->end_x,p->y,data,p->color);
      }
      else{
        data*=10;
        data += MAIN_KEY.keycode;
        LCD_ShowNumBig_L(p->start_x,p->end_x,p->y,data,p->color);
      }
    }
    BACK_COLOR = temp_color;
    if(data!=0 && i != 0){
      NumBar_SetValue(p,data);
    }else{
      NumBar_SetValue(p,p->value);
    }
  }
}
void NumBar_SetPrintFunc(NumBar* p,NumBarPrintFunc func){
  p->print_func = func;
}
void NumBar_ClearPrintFunc(NumBar* p){
  p->print_func = NULL;
}
void NumBar_SetFunc(NumBar* p,NumBarFunc func){
  p->func =func;
}
/*action*/
//设置关注
void NumBar_SetActive(NumBar* p,bool isActive){
  if(isActive && p->isShow){
    u16 len = Num_Len(p->value);
    NumBar_FixPos(p,len);
    u16 temp_color = BACK_COLOR;
    BACK_COLOR = p->back_color;
    LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
    BACK_COLOR = temp_color;
    p->isActive = true;
  }
  else{
    LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
    p->isActive = false;
  }
}
//左移
void NumBar_ShiftLeft(NumBar* p){
  if(p->isActive&&p->isShow){
    u16 len = Num_Len(p->value);
    NumBar_FixPos(p,len);
    u16 temp_color = BACK_COLOR;
    LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
    BACK_COLOR = p->back_color;
    if(p->pos+1 < len){
      p->pos += 1;
    }
    LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
    BACK_COLOR = temp_color;
  }
}
//右移
void NumBar_ShiftRight(NumBar* p){
  if(p->isActive&&p->isShow){
    u16 len = Num_Len(p->value);
    NumBar_FixPos(p,len);
    u16 temp_color = BACK_COLOR;
    LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
    BACK_COLOR = p->back_color;
    if(p->pos > 0)
    {
      p->pos -= 1;
    }
    LCD_ShowNumBig(p->end_x-p->pos,p->y,LCD_STRING_RIGHT,NumBar_GetFirst(p),p->color);
    BACK_COLOR = temp_color;
  }
}
//加数
void NumBar_Add(NumBar* p){
  if(p->isActive&&p->isShow){
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
    if(p->func){
      p->func(p,p->value);
    }
    if(p->print_func){
      p->print_func(p);
      if(p->print_func == NumBar_ShowValue){
        NumBar_SetActive(p,true);
      }
    }
  }
}
//减数
void NumBar_Cut(NumBar* p){
  if(p->isActive&&p->isShow){
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
    if(p->func){
      p->func(p,p->value);
    }
    if(p->print_func){
      p->print_func(p);
      if(p->print_func == NumBar_ShowValue){
        NumBar_SetActive(p,true);
      }
    }
  }
}
void NumBar_Show(NumBar* p){
  p->isShow = true;
  if(p->ltag.type != _LCD_STRING_NULL){
    LCD_ShowStringBig_Union(p->start_x,p->y,LCD_STRING_RIGHT,p->ltag,p->ltag_color);
  }
  if(p->print_func){
      p->print_func(p);
      if(p->print_func == NumBar_ShowValue){
        NumBar_SetActive(p,p->isActive);
      }
  }
  if(p->func){
    p->func(p,p->value);
  }
  if(p->rtag.type != _LCD_STRING_NULL){
    LCD_ShowStringBig_Union(p->end_x,p->y,LCD_STRING_LEFT,p->rtag,p->rtag_color);
  }
  //NumBar_ShowRect(p);
}
void NumBar_ShowValue(NumBar* p){
  LCD_ShowNumBig_L(p->start_x,p->end_x,p->y,p->value,p->color);
  //NumBar_SetActive(p,true);
}
void NumBar_Hide(NumBar* p){
  p->isShow = false;
  if(p->ltag.type != _LCD_STRING_NULL){
    LCD_ShowStringBig_Union(p->start_x,p->y,LCD_STRING_RIGHT,p->ltag,BACK_COLOR);
  }
  LCD_ShowNumBig_L(p->start_x,p->end_x,p->y,p->value,BACK_COLOR);
  if(p->rtag.type != _LCD_STRING_NULL){
    LCD_ShowStringBig_Union(p->end_x,p->y,LCD_STRING_LEFT,p->rtag,BACK_COLOR);
  }
}
void NumBar_ShowRect(NumBar* p){
  int x1 = p->start_x;
  int x2 = p->end_x;
  if(p->isShow){
    if(p->ltag.type == _LCD_STRING_CHINESE){
      x1 -= p->ltag.string.chinese.len*2;
      x2 += p->rtag.string.chinese.len*2;
    }
    else{
      x1 -=strlen(p->ltag.string.ascii);
      x2 +=strlen(p->rtag.string.ascii);
    }
    LCD_Show_Rect(x1*16-1,p->y*32,(x2-x1)*16,32,p->color);
  }
}
void NumBar_SetLTag(NumBar *p,LCD_STRING tag){
  p->ltag = tag;
  LCD_ShowStringBig_Union(p->start_x,p->y,LCD_STRING_RIGHT,p->ltag,p->ltag_color);
}
void NumBar_SetRTag(NumBar *p,LCD_STRING tag){
  p->rtag = tag;
  LCD_ShowStringBig_Union(p->end_x,p->y,LCD_STRING_LEFT,p->rtag,p->rtag_color);
}
void NumBar_SetLtagColor(NumBar* p,u16 value){
  p->ltag_color = value;
}
void NumBar_SetRtagColor(NumBar* p,u16 value){
  p->rtag_color = value;
}
