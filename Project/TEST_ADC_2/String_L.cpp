#include "String_L.h"
void String_L::String_L_ASCII_Init(u16 x,u16 y,char* d,LCD_PLACE_TYPE type,u16 color){
  isShow = false;
  this->y = y;
  string.type = _LCD_STRING_ASCII;
  string.string.ascii = d;
  u16 t_len = strlen(d);
  this->color = color;
  switch(type){
    case LCD_STRING_LEFT:
      start_x = x;
      end_x = start_x + t_len;
      break;
    case LCD_STRING_MID:
      start_x = x-t_len/2;
      end_x = start_x + t_len;
      break;
    case LCD_STRING_RIGHT:
      start_x = x-t_len;
      end_x = x;
      break;
  }
}
void String_L::String_L_CHINESE_Init(u16 x,u16 y,u16 pos_1,u16 len,LCD_PLACE_TYPE type,u16 color){
  isShow = false;
  this->y = y;
  string.type = _LCD_STRING_CHINESE;
  string.string.chinese.start = pos_1;
  string.string.chinese.len = len;
  this->color = color;
  switch(type){
    case LCD_STRING_LEFT:
      start_x = x;
      end_x = start_x + len*2;
      break;
    case LCD_STRING_MID:
      start_x = x-len;
      end_x = start_x + len;
      break;
    case LCD_STRING_RIGHT:
      start_x = x-len*2;
      end_x = x;
      break;
  }
}
String_L::String_L(){
  String_L_ASCII_Init(0,0,"UNKNOWN",LCD_STRING_LEFT,YELLOW);
  string.type = _LCD_STRING_NULL;
}
String_L::String_L(u16 x,u16 y,char* d){
  String_L_ASCII_Init(x,y,d,LCD_STRING_LEFT,YELLOW);
}
String_L::String_L(u16 x,u16 y,char* d,LCD_PLACE_TYPE type){
  String_L_ASCII_Init(x,y,d,type,YELLOW);
}
String_L::String_L(u16 x,u16 y,char* d,u16 color){
  String_L_ASCII_Init(x,y,d,LCD_STRING_LEFT,color);
}
String_L::String_L(u16 x,u16 y,char* d,LCD_PLACE_TYPE type,u16 color){
  String_L_ASCII_Init(x,y,d,type,color);
}
String_L::String_L(u16 x,u16 y,u16 pos_1,u16 len){
  String_L_CHINESE_Init(x,y,pos_1,len,LCD_STRING_LEFT,YELLOW);
}
String_L::String_L(u16 x,u16 y,u16 pos_1,u16 len,u16 color){
  String_L_CHINESE_Init(x,y,pos_1,len,LCD_STRING_LEFT,color);
}
String_L::String_L(u16 x,u16 y,u16 pos_1,u16 len,LCD_PLACE_TYPE type){
  String_L_CHINESE_Init(x,y,pos_1,len,type,YELLOW);
}
String_L::String_L(u16 x,u16 y,u16 pos_1,u16 len,LCD_PLACE_TYPE type,u16 color){
  String_L_CHINESE_Init(x,y,pos_1,len,type,color);
}
bool String_L::isNull(){
  if(string.type == _LCD_STRING_NULL){
    return true;
  }else{
    return false;
  }
}
void String_L::show(){
  isShow = true;
  LCD_ShowStringBig_Union(start_x,y,LCD_STRING_LEFT,string,color);
}
void String_L::hide(){
  isShow = false;
  LCD_ShowStringBig_Union(start_x,y,LCD_STRING_LEFT,string,BACK_COLOR);
}
void String_L::setColor(u16 color){
  this->color = color;
  if(isShow){
    show();
  }
}
void String_L::moveTo(u16 x,u16 y){
  this->y = y;
  u16 len = end_x-start_x;
  this->start_x = x;
  this->end_x = len+x;
}
u16 String_L::getColor(){
  return color;
}
u16 String_L::getLen(){
  return end_x-start_x;
}