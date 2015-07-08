#include "main.h"
void SubTitleSelect(bool isMeasure){
  if(isMeasure){
    LCD_STRING t;
    t.type = _LCD_STRING_CHINESE;
    t.string.chinese.len  = 4;
    t.string.chinese.start  = 39;
    SelectBar_SetTitle(&sub_menu,t);
  }else{
    LCD_STRING t;
    t.type = _LCD_STRING_CHINESE;
    t.string.chinese.len  = 4;
    t.string.chinese.start  = 45;
    SelectBar_SetTitle(&sub_menu,t);
  }
}
void RES_CAP_Select(bool isRES){
  if(isRES){
    ADS1220_ChangeChannel(&ADC_1,0);
  }else{
    ADS1220_ChangeChannel(&ADC_1,2);
  }
  __disable_irq();
  while(ADS1220_DRDY_PIN == 1);
  while(ADS1220_DRDY_PIN == 1);
  ADS1220_Set_Rate(&ADC_1,600);
  extern int _ADC_1_SEQ_BASE[SHIFT_FITHER_LEN+1];
  Sequeue_Init(&ADC_1_SEQ,_ADC_1_SEQ_BASE,SHIFT_FITHER_LEN+1);
  while(!Sequeue_Full(&ADC_1_SEQ)){
    if(ADS1220_DRDY_PIN == 0){
      Sequeue_In_Queue(&ADC_1_SEQ,ADS1220_Read_Data());
    }
    delay_ms(1);
  }
  ADS1220_Set_Rate(&ADC_1,20);
  __enable_irq();
}
//电阻测量
void RES_EN(SelectItem* p){
  LCD_STRING t;
  t.type = _LCD_STRING_ASCII;
  t.string.ascii = "50O Ohm";
  SelectBar_AddSelect(&sub_menu,t);
  t.string.ascii = "5 kOhm";
  SelectBar_AddSelect(&sub_menu,t);
  t.string.ascii = "50 kOhm";
  SelectBar_AddSelect(&sub_menu,t);
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese.len  = 2;
  t.string.chinese.start  = 43;
  SelectBar_AddSelect(&sub_menu,t);
  SelectItem_SetFunc_En(SelectBar_GetSelect(&sub_menu,0),RES_LEVEL_1);
  SelectItem_SetFunc_En(SelectBar_GetSelect(&sub_menu,1),RES_LEVEL_2);
  SelectItem_SetFunc_En(SelectBar_GetSelect(&sub_menu,2),RES_LEVEL_3);
  NumBar_Show(&RES);
  SelectBar_Show(&sub_menu);
  SubTitleSelect(true);
  RES_CAP_Select(true);
}


void RES_UN(SelectItem* p){
  SelectBar_ClearSelect(&sub_menu);
  NumBar_Hide(&RES);
}

//电容测量
void CAP_EN(SelectItem* p){
  LCD_STRING t;
  t.type = _LCD_STRING_ASCII;
  t.string.ascii = "5nF";
  SelectBar_AddSelect(&sub_menu,t);
  t.string.ascii = "500nF";
  SelectBar_AddSelect(&sub_menu,t);
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese.len  = 2;
  t.string.chinese.start  = 43;
  SelectBar_AddSelect(&sub_menu,t);
  SelectItem_SetFunc_En(SelectBar_GetSelect(&sub_menu,0),CAP_LEVEL_1);
  SelectItem_SetFunc_En(SelectBar_GetSelect(&sub_menu,1),CAP_LEVEL_2);
  SelectItem_SetFunc_En(SelectBar_GetSelect(&sub_menu,2),CAP_LEVEL_3);
  NumBar_Show(&CAP);
  SelectBar_Show(&sub_menu);
  SubTitleSelect(true);
  RES_CAP_Select(false);
}


void CAP_UN(SelectItem* p){
  SelectBar_ClearSelect(&sub_menu);
  NumBar_Hide(&CAP);
}

//电阻查找
void RES_FIND_EN(SelectItem* p){
  RES_CAP_Select(true);
  SubTitleSelect(false);
  NumBar_Show(&RES_MAX);
  NumBar_Show(&RES_MIN);
  NumBar_Show(&RES);
}

void RES_FIND_UN(SelectItem* p){
  NumBar_Hide(&RES_MAX);
  NumBar_Hide(&RES_MIN);
  NumBar_Hide(&RES);
  SelectBar_ClearSelect(&sub_menu);
}

//电容查找
void CAP_FIND_EN(SelectItem* p){
  RES_CAP_Select(false);
  SubTitleSelect(false);
  NumBar_Show(&CAP_MAX);
  NumBar_Show(&CAP_MIN);
  NumBar_Show(&CAP);
  LCD_STRING t;
  t.type = _LCD_STRING_ASCII;
  t.string.ascii = "";
  SelectBar_AddSelect(&sub_menu,t);
  SelectBar_AddSelect(&sub_menu,t);
}

void CAP_FIND_UN(SelectItem* p){
  NumBar_Hide(&CAP_MAX);
  NumBar_Hide(&CAP_MIN);
  NumBar_Hide(&CAP);
  SelectBar_ClearSelect(&sub_menu);
}

//设置电阻显示
void SetRes(NumBar* p,int value){
  float t = *(float*)(&value);
  float last = *(float*)(&p->value);
  if(t<0){
    t = 0;
  }
  LCD_ShowFloatBig_L(p->start_x,p->end_x,p->y,t,p->color);
  p->value = value;
}

//设置电容显示
void SetCap(NumBar* p,int value){
  float t = *(float*)(&value);
  if(t<0){
    t = -t;
  }
  LCD_ShowFloatBig_L(p->start_x,p->end_x,p->y,t,p->color);
  p->value = value;
}