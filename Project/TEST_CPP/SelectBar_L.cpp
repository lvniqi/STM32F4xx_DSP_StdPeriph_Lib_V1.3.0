#include "SelectBar_L.h"
SelectItem_L::SelectItem_L(String_L s){
  string = s.getString();
  color = s.getColor();
  y = s.getY();
  start_x = s.getStart_x();
  end_x = start_x + s.getLen();
  back_color = RED;
  isShow = false;
  isActive = false;
  func_en = NULL;
  func_un = NULL;
  next = NULL;
}
void SelectItem_L::setFunc_En(SelectItem_L_func func){
  func_en = func;
}
void SelectItem_L::setFunc_Un(SelectItem_L_func func){
  func_un = func;
}
void SelectItem_L::setBackColor(u16 color){
  this->back_color = color;
  if(isActive){
    setActive(true);
  }
}
/*action*/
void SelectItem_L::setActive(bool isActive){
  if(isActive && getShow()){
    u16 temp_color = BACK_COLOR;
    BACK_COLOR = back_color;
    show();
    BACK_COLOR = temp_color;
    this->isActive = true;
    if(func_en){
      func_en(this);
    }
  }
  else{
    show();
    this->isActive = false;
    if(func_un){
      func_un(this);
    }
  }
}

SelectBar_L::SelectBar_L(u16 x,u16 y,bool isV){
  start_x = x;
  start_y = y;
  item_y = y-1;
  pos = -1;
  len = 0;
  selectNode_Start = NULL;
  selectNode_Now = NULL;
  selectNode_End = NULL;
  isShow = false;
  title_color = BLUE;
  back_color = RED;
  isV = true;
}
void SelectBar_L::addSelect(String_L s){
  SelectItem_L* new_one = (SelectItem_L*)mymalloc(sizeof(SelectItem_L));
  if(isV){
    *new_one = SelectItem_L(s);
    new_one->moveTo(start_x,item_y);
    item_y = item_y-1;
    if(isShow){
      new_one->show();
    }
    if(selectNode_Start == NULL){
      selectNode_Start = new_one;
      selectNode_Now = new_one;
      pos = 0;
      selectNode_End = new_one;
    }
    else{
      selectNode_End->addNext(new_one);
      selectNode_End = new_one;
    }
  }
}