#ifndef SELECTBAR_L_H_
#define SELECTBAR_L_H_
  #include "String_L.h"
  extern "C"{
  #include "malloc.h"
  }
  class SelectItem_L;
  typedef void (*SelectItem_L_func)(SelectItem_L* p);
  class SelectItem_L:public String_L{
  public:
    //main
    SelectItem_L(String_L s);
    void setFunc_En(SelectItem_L_func func);
    void setFunc_Un(SelectItem_L_func func);
    void setBackColor(u16 color);
    void setActive(bool isActive);
    void addNext(SelectItem_L* p){
      next = p;
    }
    SelectItem_L* getNext(){
      return next;
    }
  private:
    bool isActive;
    u16 back_color;
    SelectItem_L* next;
    SelectItem_L_func func_en;
    SelectItem_L_func func_un;
  };
  
  class SelectBar_L;
  typedef void (*SelectBar_L_func)(SelectBar_L* p);
  class SelectBar_L{
  public:
    SelectBar_L(u16 x,u16 y,bool isV);
    void addSelect(String_L s);
    void show(){
      SelectItem_L* p_i = selectNode_Start;
      if(!title.isNull()){
        title.show();
      }
      while(p_i){
        p_i->show();
        p_i = p_i->getNext();
      }
      if(selectNode_Now){
        selectNode_Now->setActive(true);
      }
      isShow = true;
    }
  private:
    u8 start_x;
    u8 end_x;
    u8 start_y;
    u8 end_y;
    u8 item_y;
    u8 len;
    short pos;
    String_L title;
    SelectItem_L* selectNode_Start;
    SelectItem_L* selectNode_Now;
    SelectItem_L* selectNode_End;
    bool isShow;
    bool isV;
    u16 title_color;
    u16 color;
    u16 back_color;
    SelectBar_L_func func_en;
    SelectBar_L_func func_un;
  };
#endif