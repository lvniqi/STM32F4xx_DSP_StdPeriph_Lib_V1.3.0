extern "C"{
  #include "common.h"
  #include "LCD.h"
}
class NumBar_CPP;
typedef void (*NumBar_CPP_func)(NumBar_CPP* p,int value);
class NumBar_CPP{
public:
  /*main*/
  NumBar_CPP(u8 end_x,u8 y,int v_max,int v_min);
  void setValue(int value);
  void setFunc(NumBar_CPP_func func);
  void setValue_GUI();
  /*status*/
  bool getActive();
  bool getShow();
  int  getValue();
  u8 getPos();
  /*action*/
  void setActive(bool isActive);
  void shiftLeft();
  void shiftRight();
  void add();
  void cut();
  void show();
  void showValue();
  void showRect();
  void hide();
private:
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
  NumBar_CPP_func func;
  u32 getFirst();
  void fixPos(u16 len);
};