#ifndef STRING_L_H_
#define STRING_L_H_
  extern "C"{
    #include "common.h"
    #include "LCD.h"
  }
  class String_L{
    public:
      String_L();
      String_L(u16 x,u16 y,u16 pos_1,u16 len);
      String_L(u16 x,u16 y,u16 pos_1,u16 len,u16 color);
      String_L(u16 x,u16 y,u16 pos_1,u16 len,LCD_PLACE_TYPE type);
      String_L(u16 x,u16 y,u16 pos_1,u16 len,LCD_PLACE_TYPE type,u16 color);
      String_L(u16 x,u16 y,char* d);
      String_L(u16 x,u16 y,char* d,u16 color);
      String_L(u16 x,u16 y,char* d,LCD_PLACE_TYPE type);
      String_L(u16 x,u16 y,char* d,LCD_PLACE_TYPE type,u16 color);
      bool isNull();
      void show();
      void showRect();
      void setColor(u16 color);
      u16 getColor();
      u16 getLen();
      u16 getStart_x();
      u16 getY();
      void hide();
      bool getShow(){
        return isShow;
      }
      LCD_STRING getString();
      void moveTo(u16 x,u16 y);
    protected:
      u16 start_x;
      u16 end_x;
      u16 y;
      u16 color;
      bool isShow;
      LCD_STRING string;
      void String_L_CHINESE_Init(u16 x,u16 y,u16 pos_1,u16 len,LCD_PLACE_TYPE type,u16 color);
      void String_L_ASCII_Init(u16 x,u16 y,char* d,LCD_PLACE_TYPE type,u16 color);
  };
#endif