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
void SelectItem_L::hide(){
  String_L::hide();
  if(isActive&&func_un){
    func_un(this);
  }
}

SelectBar_L::SelectBar_L(u16 x,u16 y,bool isV){
  pos = -1;
  len = 0;
  selectNode_Start = NULL;
  selectNode_Now = NULL;
  selectNode_End = NULL;
  title = NULL;
  sub = NULL;
  isShow = false;
  back_color = RED;
  this->isV = isV;
  
  start_x = x;
  start_y = y;
  if(isV){
    item_y = y-1;
  }else{
    item_y = y;
  }
  if(isV){
    item_x = x;
  }else{
    if(title){
      item_x = x+title->getLen();
    }else{
      item_x = x+8;
    }
  }
}
void SelectBar_L::addSelect(String_L s){
  SelectItem_L* new_one = (SelectItem_L*)mymalloc(sizeof(SelectItem_L));
  *new_one = SelectItem_L(s);
  new_one->moveTo(item_x,item_y);
  len++;
  if(isV){
    item_y -= 1;
  }
  else{
    item_x += s.getLen();
  }
  if(selectNode_Start == NULL){
    selectNode_Start = new_one;
    selectNode_Now = new_one;
    if(isShow){
      new_one->setActive(true);
    }
    pos = 0;
    selectNode_End = new_one;
  }
  if(isShow){
    new_one->show();
  }
  else{
    selectNode_End->addNext(new_one);
    selectNode_End = new_one;
  }
}
void SelectBar_L::show(){
  SelectItem_L* p_i = selectNode_Start;
  if(title){
    title->show();
  }
  while(p_i){
    p_i->show();
    p_i = p_i->getNext();
  }
  if(selectNode_Now){
    selectNode_Now->setActive(true);
  }
  if(sub){
    sub->show();
  }
  isShow = true;
}
void SelectBar_L::hide(){
  SelectItem_L* p_i = selectNode_Start;
  if(title){
    title->hide();
  }
  while(p_i){
    p_i->hide();
    p_i = p_i->getNext();
  }
  if(sub){
    sub->hide();
  }
  isShow = false;
}
void SelectBar_L::shiftLast(){
  if(isShow){
    SelectItem_L* p_i = selectNode_Start;
    while(p_i){
      if(p_i->getNext() == selectNode_Now){
        break;
      }
      p_i = p_i->getNext();
    }
    if(p_i){
      selectNode_Now->setActive(false);
      p_i->setActive(true);
      selectNode_Now = p_i;
      pos--;
    }
  }
}
void SelectBar_L::shiftNext(){
  if(isShow){
    if(selectNode_Now&&selectNode_Now->getNext()){
      selectNode_Now->setActive(false);
      selectNode_Now = selectNode_Now->getNext();
      pos++;
      selectNode_Now->setActive(true);
    }
  }
}
void SelectBar_L::setTitle(String_L s){
  if(title){
    myfree(title);
  }
  title = (String_L*)mymalloc(sizeof(String_L));
  *(title) = s;
  title->moveTo(start_x,start_y);
  if(isShow){
    title->show();
  }
  if(selectNode_Start == NULL&& !isV){
    item_x = start_x+title->getLen();
  }
}
SelectItem_L* SelectBar_L::getSelect(int n){
  SelectItem_L* p_i = selectNode_Start;
  for(int i=0;i<n;i++){
    if(p_i){
      p_i = p_i->getNext();
    }else{
      return NULL;
    }
  }
  return p_i;
}
void SelectBar_L::clearSelect(void){
  SelectItem_L* p_i = selectNode_Start;
  while(p_i){
    SelectItem_L* p_l = p_i;
    p_i = p_i->getNext();
    p_l->hide();
    myfree(p_l);
  }
  if(isV){
    item_y = start_y-1;
    item_x = start_x;
  }else{
    item_y = start_y;
    if(title){
      item_x = start_x+title->getLen();
    }else{
      item_x = start_x+8;
    }
  }
  selectNode_Start = NULL;
  selectNode_Now = NULL;
  selectNode_End = NULL;
  pos = -1;
  len = 0;
}
void SelectBar_L::toggleLayout(){
  hide();
  isV = !isV;
  if(isV){
    item_y = start_y-1;
    item_x = start_x;
  }else{
    item_y = start_y;
    if(title){
      item_x = start_x+title->getLen();
    }else{
      item_x = start_x+8;
    }
  }
  SelectItem_L* p_i = selectNode_Start;
  while(p_i){
    p_i->moveTo(item_x,item_y);
    if(isV){
      item_y -=1;
    }else{
      item_x += p_i->getLen();
    }
    p_i = p_i->getNext();
  }
  show();
}
void SelectBar_L::setSub(SelectBar_L& sub){
  if(this->sub){
    myfree(this->sub);
  }
  this->sub = (SelectBar_L*)mymalloc(sizeof(SelectBar_L));
  *(this->sub) = sub;
}
SelectBar_L* SelectBar_L::getSub(){
  return sub;
}