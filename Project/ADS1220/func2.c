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
float RES_ZOOM_500(float t_r){
  if(t_r<50){
    t_r = 0.9965*t_r - 0.0909;
  }else{
    t_r = 0.9962*t_r - 0.0553;
  }
  return t_r;
}
float RES_ZOOM_5K(float t_r){
  if(t_r<100){
    t_r = 9.9657*t_r - 0.5881;
  }else{
    t_r = 9.966*t_r - 0.3323;
  }
  return t_r;
}
float RES_ZOOM_50K(float t_r){
  if(t_r<100){
    t_r = 104.705*t_r - 2;
  }
  else{
    t_r = 104.75*t_r - 11.168;
  }
  return t_r;
}

float CAP_ZOOM_500(double value){
  float t_r = value/2969*470;
  t_r = 0.00021*t_r*t_r + 0.8969*t_r + 2.116;
  return t_r;
}
float CAP_ZOOM_5(double value){
  float t_r = value/2555*14.7;
  t_r = -0.0313*t_r*t_r + 1.0012*t_r - 0.0951;
  return t_r;
}
float CAP_ZOOM_50(double value){
  float t_r = value/2868*46.2;
  t_r = -0.0065*t_r*t_r + 1.309*t_r - 0.8243;
  return t_r;
}


float Get_RES(double value){
  float t_r = value*RES_BASE_0/(4096.0-value);
  //500欧姆档
  if(SelectBar_GetPos(&sub_menu) == 0){
    t_r = RES_ZOOM_500(t_r);
    if(t_r>499.999||value>4090.0){
      t_r = 499.999;
    }
  }
  //5K欧姆档
  else if(SelectBar_GetPos(&sub_menu) == 1){
    t_r = RES_ZOOM_5K(t_r);
    if(t_r>4999.99||value>4090.0){
      t_r = 4999.99;
    }
  }
  //50K欧姆档
  else if(SelectBar_GetPos(&sub_menu) == 2){
    t_r = RES_ZOOM_50K(t_r);
    if(t_r>49999.9||value>4090.0){
      t_r = 49999.9;
    }
  }
  //自动档
  else if(SelectBar_GetPos(&sub_menu) == 3){
    t_r = Get_RES_AUTO(value);
  }
  return t_r;
}

float Get_RES_AUTO(double value){
  float t_r = value*RES_BASE_0/(4096.0-value);
  bool change_flag = false;
  //原来为500档
  if(PEin(5) == 0&&PEin(3) == 1){
    t_r = RES_ZOOM_500(t_r);
    if(t_r>500){
      change_flag = true;
    }
  }
  //原来为5K档
  else if(PEin(5) == 1&&PEin(3) == 0){
    t_r = RES_ZOOM_5K(t_r);
    if(t_r>6000|| t_r<400){
      change_flag = true;
    }
  }
  //原来为50K档
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
  //50nF档
  if(SelectBar_GetPos(&sub_menu) == 0){
    t_r = CAP_ZOOM_50(t_r);
    if(t_r>49.9999||value>4090.0){
      t_r = 49.9999;
    }
  }
  //500nF档
  else if(SelectBar_GetPos(&sub_menu) == 1){
    t_r = CAP_ZOOM_500(t_r);
    if(t_r>499.999||value>4090.0){
      t_r = 499.999;
    }
  }
  //自动档
  else if(SelectBar_GetPos(&sub_menu) == 2){
    t_r = Get_CAP_AUTO(value);
  }
  return t_r;
}

float Get_CAP_AUTO(double value){
  float t_r = 0;
  bool change_flag = false;
  //原来为50档
  if(PEin(1) == 1&&PBin(9) == 0){
    t_r = CAP_ZOOM_500(t_r);
    if(t_r>60){
      change_flag = true;
    }
  }
  //原来为50档
  else if(PEin(1) == 0&&PBin(9) == 1){
    t_r = CAP_ZOOM_50(t_r);
    if(t_r<40){
      change_flag = true;
    }
  }
  
  if(change_flag){
    if(t_r>60){
      CAP_LEVEL_3(NULL);
    }
    else if(t_r>5){
      CAP_LEVEL_2(NULL);
    }else{
      CAP_LEVEL_1(NULL);
    }
    CAP_CAP_Select(true); 
  }
  return t_r;
}