#include "NumBar_CPP.h"
/*other*/
//得到最大数字首位
u32 NumBar_CPP::getFirst(){
  u32 temp=10;
  for(int i=0;i<pos;i++){
      temp*=10;
  }
  if(value >0){
    return (value%temp)/(temp/10);
  }else{
    return ((-value)%temp)/(temp/10);
  }
}
//修正pos
void NumBar_CPP::fixPos(u16 len){
  if(pos >= len){
    if(len > 0){
      pos = len-1;
    }
    else{
      pos = 0;
    }
  }
}
/*main*/
NumBar_CPP::NumBar_CPP(u8 end_x,u8 y,int v_max,int v_min){
  this->start_x = end_x-Num_Len(v_max)-1;
  this->end_x = end_x;
  this->y = y;
  this->value_max = v_max;
  this->value_min = v_min;
  this->value = v_min;
  this->pos = 0;
  this->func = NULL;
  this->color = YELLOW;
  this->back_color = RED;
  this->isActive = false;
  /*LCD_STRING t;
  t.type = _LCD_STRING_NULL;
  this->ltag = t;
  this->rtag = t;
  this->ltag_color = WHITE;
  this->rtag_color = WHITE;*/
  this->isShow = false;
}

void NumBar_CPP::setValue(int value){
  if(value_max>= value && value >=value_min){
    this->value = value;
    if(func){
      func(this,value);
    }
  }
  if(isShow){
    showValue();
    setActive(isActive);
   
  }
}

void NumBar_CPP::showValue(){
  LCD_ShowNumBig_L(start_x,end_x,y,value,color);
}

void NumBar_CPP::setValue_GUI(){
  if(isShow){
    int data = 0;
    int x = end_x - 1;
    int len = end_x-start_x;
    u16 temp_color = BACK_COLOR;
    BACK_COLOR = back_color;
    LCD_ShowNumBig_L(start_x,end_x,y,value,color);
    int i=0;
    for(;i<len;i++){
      while(!MAIN_KEY.keysign);
      MAIN_KEY.keysign = 0;
      if(MAIN_KEY.keycode >10){
        break;
      }
      else if(MAIN_KEY.keycode == 10){
        data = -data;
        LCD_ShowNumBig_L(start_x,end_x,y,data,color);
      }
      else{
        data*=10;
        data += MAIN_KEY.keycode;
        LCD_ShowNumBig_L(start_x,end_x,y,data,color);
      }
    }
    BACK_COLOR = temp_color;
    if(data!=0 && i != 0){
      setValue(data);
    }else{
      setValue(value);
    }
  }
}
void NumBar_CPP::setFunc(NumBar_CPP_func func){
  this->func = func;
}
/*action*/
//设置关注
void NumBar_CPP::setActive(bool isActive){
  if(isActive && isShow){
    u16 len = Num_Len(value);
    fixPos(len);
    u16 temp_color = BACK_COLOR;
    BACK_COLOR = back_color;
    LCD_ShowNumBig(end_x-pos,y,LCD_STRING_RIGHT,getFirst(),color);
    BACK_COLOR = temp_color;
    this->isActive = true;
  }
  else{
    LCD_ShowNumBig(end_x-pos,y,LCD_STRING_RIGHT,getFirst(),color);
    this->isActive = false;
  }
}
//左移
void NumBar_CPP::shiftLeft(){
  if(isActive&&isShow){
    u16 len = Num_Len(value);
    fixPos(len);
    u16 temp_color = BACK_COLOR;
    LCD_ShowNumBig(end_x-pos,y,LCD_STRING_RIGHT,getFirst(),color);
    BACK_COLOR = back_color;
    if(pos+1 < len){
      pos += 1;
    }
    LCD_ShowNumBig(end_x-pos,y,LCD_STRING_RIGHT,getFirst(),color);
    BACK_COLOR = temp_color;
  }
}
//右移
void NumBar_CPP::shiftRight(){
  if(isActive&&isShow){
    u16 len = Num_Len(value);
    fixPos(len);
    u16 temp_color = BACK_COLOR;
    LCD_ShowNumBig(end_x-pos,y,LCD_STRING_RIGHT,getFirst(),color);
    BACK_COLOR = back_color;
    if(pos > 0)
    {
      pos -= 1;
    }
    LCD_ShowNumBig(end_x-pos,y,LCD_STRING_RIGHT,getFirst(),color);
    BACK_COLOR = temp_color;
  }
}
//加数
void NumBar_CPP::add(){
  if(isActive&&isShow){
    u16 len = Num_Len(value);
    fixPos(len);
    int temp = 1;
    for(int i=0;i<pos;i++){
      temp*=10;
    }
    if(temp + value < value_max){
      value += temp;
    }else{
      value = value_max;
    }
    setValue(value);
  }
}
//减数
void NumBar_CPP::cut(){
  if(isActive&&isShow){
    u16 len = Num_Len(value);
    fixPos(len);
    int temp = 1;
    for(int i=0;i<pos;i++){
      temp*=10;
    }
    if(value_min <= value-temp){
      value-= temp;
    }else{
      value = value_min;
    }
    setValue(value);
  }
}
void NumBar_CPP::show(){
  isShow = true;
  if(!ltag.isNull()){
    ltag.show();
  }
  setValue(value);
  if(!rtag.isNull()){
    rtag.show();
  }
  //showRect();
}
void NumBar_CPP::hide(){
  if(isShow){
    if(!ltag.isNull()){
      ltag.hide();
    }
    LCD_ShowNumBig_L(start_x,end_x,y,value,BACK_COLOR);
    if(!rtag.isNull()){
      rtag.hide();
    }
    isShow = false;
  }
}
void NumBar_CPP::showRect(){
  int x1 = start_x;
  int x2 = end_x;
  if(isShow){
    x1 -=ltag.getLen();
    x2 +=rtag.getLen();
  }
  LCD_Show_Rect(x1*16-1,y*32,(x2-x1)*16,32,color);
}
void NumBar_CPP::setLtag(const String_L& tag){
  ltag = tag;
  ltag.moveTo(start_x-ltag.getLen(),y);
}
void NumBar_CPP::setRtag(const String_L& tag){
  rtag = tag;
  rtag.moveTo(end_x,y);
}
