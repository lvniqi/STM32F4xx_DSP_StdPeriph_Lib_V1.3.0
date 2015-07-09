#include "main.h"
void RES_LEVEL_1(SelectItem* p){
  PEout(5) = 0;
  PEout(3) = 1;
}
void RES_LEVEL_2(SelectItem* p){
  PEout(5) = 1;
  PEout(3) = 0;
}
void RES_LEVEL_3(SelectItem* p){
  PEout(5) = 0;
  PEout(3) = 0;
}
void CAP_LEVEL_1(SelectItem* p){
  PEout(1) = 0;
  PBout(9) = 1;
}
void CAP_LEVEL_2(SelectItem* p){
  PEout(1) = 1;
  PBout(9) = 0;
}
void CAP_LEVEL_3(SelectItem* p){
  PEout(1) = 0;
  PBout(9) = 0;
}
void Select_CAP_SAMPLE(SelectItem* p){
  NumBar_SetActive(&CAP_SAMPLE,true);
}
void UnSelect_CAP_SAMPLE(SelectItem* p){
  NumBar_SetActive(&CAP_SAMPLE,false);
}
void Select_CAP_RANGE(SelectItem* p){
  NumBar_SetActive(&CAP_RANGE,true);
}
void UnSelect_CAP_RANGE(SelectItem* p){
  NumBar_SetActive(&CAP_RANGE,false);
}
void Select_RES_SAMPLE(SelectItem* p){
  NumBar_SetActive(&RES_SAMPLE,true);
}
void UnSelect_RES_SAMPLE(SelectItem* p){
  NumBar_SetActive(&RES_SAMPLE,false);
}
void Select_RES_RANGE(SelectItem* p){
  NumBar_SetActive(&RES_RANGE,true);
}
void UnSelect_RES_RANGE(SelectItem* p){
  NumBar_SetActive(&RES_RANGE,false);
}

float RES_ZOOM_500(float t_r){
  if(t_r<50){
    t_r = 0.997*t_r - 0.0909;
  }else{
    t_r = 0.9962*t_r - 0.0553;
  }
  return t_r;
}
float RES_ZOOM_5K(float t_r){
  if(t_r<100){
    t_r = 9.9657*t_r - 0.5881;
  }else{
    t_r = 9.968*t_r - 0.3323;
  }
  return t_r;
}
float RES_ZOOM_50K(float t_r){
  
  if(t_r<100){
      t_r = 104.85*t_r - 4.0975;
    }
    else{
      t_r = 0.0008*t_r*t_r + 104.69*t_r - 7.6;
    }
  
  return t_r;
}

float CAP_ZOOM_500(double value){
  float t_r = value/2969*470;
  t_r = 0.0002*t_r*t_r + 0.8960*t_r + 3.216;
  return t_r;
}
float CAP_ZOOM_5(double value){
  float t_r = value/2555*14.7;
  t_r = -0.0313*t_r*t_r + 1.0012*t_r - 0.0951;
  return t_r;
}
float CAP_ZOOM_50(double value){
  float t_r;
  if(PEin(1) == 0&&PBin(9) == 1){
    t_r = CAP_ZOOM_5(value);
    if(t_r > 6){
      CAP_LEVEL_2(NULL);
      RES_CAP_Select(false);
    }
  }else{
    t_r = 0.0159*value - 0.1461;
    if(t_r >43 && t_r <48){
      t_r = t_r-((float)(int)(t_r*10))/10+46.2;
    }
    else if(t_r >11.5 && t_r <16){
      t_r = t_r-((float)(int)(t_r*10))/10+15;
    }
    if(t_r < 5){
      CAP_LEVEL_1(NULL);
      RES_CAP_Select(false);
    }
  }
  return t_r;
}


float Get_RES(double value){
  float t_r = value*RES_BASE_0/(4096.0-value);
  //500ŷķ��
  if(SelectBar_GetPos(&sub_menu) == 0){
    t_r = RES_ZOOM_500(t_r);
    if(t_r>499.999||value>4090.0){
      t_r = 499.999;
    }
  }
  //5Kŷķ��
  else if(SelectBar_GetPos(&sub_menu) == 1){
    t_r = RES_ZOOM_5K(t_r);
    if(t_r>4999.99||value>4090.0){
      t_r = 4999.99;
    }
  }
  //50Kŷķ��
  else if(SelectBar_GetPos(&sub_menu) == 2){
    t_r = RES_ZOOM_50K(t_r);
    if(t_r>49999.9||value>4090.0){
      t_r = 49999.9;
    }
  }
  //�Զ���
  else if(SelectBar_GetPos(&sub_menu) == 3){
    t_r = Get_RES_AUTO(value);
    if(t_r>49999.9||value>4090.0){
      t_r = 49999.9;
    }
  }
  return t_r;
}

float Get_RES_AUTO(double value){
  float t_r = value*RES_BASE_0/(4096.0-value);
  bool change_flag = false;
  //ԭ��Ϊ500��
  if(PEin(5) == 0&&PEin(3) == 1){
    t_r = RES_ZOOM_500(t_r);
    if(t_r>500){
      change_flag = true;
    }
  }
  //ԭ��Ϊ5K��
  else if(PEin(5) == 1&&PEin(3) == 0){
    t_r = RES_ZOOM_5K(t_r);
    if(t_r>6000|| t_r<400){
      change_flag = true;
    }
  }
  //ԭ��Ϊ50K��
  else{
    t_r = RES_ZOOM_50K(t_r);
    if(value>4090.0){
      t_r = 9999999;
    }
    if(t_r<4000){
      change_flag = true;
    }
  }
  if(change_flag){
    if(t_r>5000){
      RES_LEVEL_3(NULL);
    }
    else if(t_r>500){
      RES_LEVEL_2(NULL);
    }else{
      RES_LEVEL_1(NULL);
    }
    RES_CAP_Select(true); 
  }
  return t_r;
}

float Get_CAP(double value){
  float t_r;
  //50nF��
  if(SelectBar_GetPos(&sub_menu) == 0){
    t_r = CAP_ZOOM_50(value);
    if(t_r>49.9999||value>4090.0){
      t_r = 49.9999;
    }
  }
  //500nF��
  else if(SelectBar_GetPos(&sub_menu) == 1){
    t_r = CAP_ZOOM_500(value);
    if(t_r>499.999||value>4090.0){
      t_r = 499.999;
    }
  }
  //�Զ���
  else if(SelectBar_GetPos(&sub_menu) == 2){
    t_r = Get_CAP_AUTO(value);
    if(t_r>499.999||value>4090.0){
      t_r = 499.999;
    }
  }
  return t_r;
}

float Get_CAP_AUTO(double value){
  float t_r = 0;
  bool change_flag = false;
  //ԭ��Ϊ50��
  if(PEin(1) == 1||PBin(9) == 1){
    t_r = CAP_ZOOM_50(value);
    if(t_r>52){
      change_flag = true;
    }
  }
  //ԭ��Ϊ500��
  else if(PEin(1) == 0&&PBin(9) == 0){
    t_r = CAP_ZOOM_500(value);
    if(t_r<50){
      change_flag = true;
    }
  }
  if(change_flag){
    if(t_r>52){
      CAP_LEVEL_3(NULL);
    }
    else if(t_r>5){
      CAP_LEVEL_2(NULL);
    }else{
      CAP_LEVEL_1(NULL);
    }
    RES_CAP_Select(false); 
  }
  return t_r;
}