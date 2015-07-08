#include "SelectBar.h"
/*main*/
void SelectBar_Init(SelectBar* p,u8 start_x,u8 end_x,u8 y){
  p->start_x = start_x;
  p->end_x = end_x;
  p->y = y;
  p->item_y = y-1;
  p->pos = -1;
  p->len = 0;
  p->title = NULL;
  p->selectNode_Start = NULL;
  p->selectNode_Now = NULL;
  p->selectNode_End = NULL;
  p->isShow = false;
  p->color = YELLOW;
  p->title_color = BLUE;
  p->back_color = RED;
}
void SelectBar_SetFunc(SelectBar* p,SelectBarFunc func){
  p->func = func;
}
/*action*/
void SelectBar_ShiftDown(SelectBar* p){
  if(p->isShow){
    if(p->selectNode_Now&&p->selectNode_Now->next){
      SelectItem_SetActive(p->selectNode_Now,false);
      p->selectNode_Now = p->selectNode_Now->next;
      p->pos++;
      SelectItem_SetActive(p->selectNode_Now,true);
    }
  }
}
void SelectBar_ShiftUp(SelectBar* p){
  if(p->isShow){
    SelectItem* p_i = p->selectNode_Start;
    while(p_i){
      if(p_i->next == p->selectNode_Now){
        break;
      }
      p_i = p_i->next;
    }
    if(p_i){
      SelectItem_SetActive(p->selectNode_Now,false);
      SelectItem_SetActive(p_i,true);
      p->selectNode_Now = p_i;
      p->pos--;
    }
  }
}
void SelectBar_Show(SelectBar* p){
  SelectItem* p_i = p->selectNode_Start;
  if(p->title){
    LCD_ShowStringBig_Union(p->start_x,p->y,LCD_STRING_LEFT,*(p->title),p->title_color);
  }
  while(p_i){
    SelectItem_Show(p_i);
    p_i = p_i->next;
  }
  if(p->selectNode_Now){
    SelectItem_SetActive(p->selectNode_Now,true);
  }
  p->isShow = true;
}

void SelectBar_ShowRect(SelectBar* p);

void SelectBar_Hide(SelectBar* p){
  if(p->isShow){
    SelectItem* p_i = p->selectNode_Start;
    while(p_i){
      SelectItem_Hide(p_i);
      p_i = p_i->next;
    }
    if(p->title){
    LCD_ShowStringBig_Union(p->start_x,p->y,LCD_STRING_LEFT,*(p->title),BACK_COLOR);
    }
    p->isShow = false;
  }
}
/*title*/
void SelectBar_SetTitle(SelectBar* p,LCD_STRING title){
  if(p->title){
    myfree(p->title);
  }
  p->title = mymalloc(sizeof(LCD_STRING));
  *(p->title) = title;
  if(p->isShow){
    LCD_ShowStringBig_Union(p->start_x,p->y,LCD_STRING_LEFT,*(p->title),p->title_color);
  }
}

/*node*/
void SelectBar_AddSelect(SelectBar* p,LCD_STRING string){
    SelectItem* new_one = mymalloc(sizeof(SelectItem));
    SelectItem_Init(new_one,p->start_x,p->item_y);
    SelectItem_SetString(new_one,string);
    p->item_y = p->item_y-1;
    if(p->isShow){
      if(p->isShow){
        SelectItem_Show(new_one);
      }
    }
    if(p->selectNode_Start == NULL){
      p->selectNode_Start = new_one;
      p->selectNode_Now = new_one;
      p->pos = 0;
      p->selectNode_End = new_one;
    }
    else{
      p->selectNode_End->next = new_one;
      p->selectNode_End = new_one;
    }
}
SelectItem* SelectBar_GetSelect(SelectBar* p,int n){
  SelectItem* p_i = p->selectNode_Start;
  for(int i=0;i<n;i++){
    if(p_i){
      p_i = p_i->next;
    }else{
      return NULL;
    }
  }
  return p_i;
}
void SelectBar_ClearSelect(SelectBar* p){
  SelectItem* p_i = p->selectNode_Start;
  while(p_i){
    SelectItem* p_l = p_i;
    p_i = p_i->next;
    SelectItem_Hide(p_l);
    myfree(p_l);
  }
  p->item_y = p->y-1;
  p->selectNode_Start = NULL;
  p->selectNode_Now = NULL;
  p->selectNode_End = NULL;
  p->pos = NULL;
}