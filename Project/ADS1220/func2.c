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

float CAP_ZOOM_500(float t_r){
  if(t_r<50){
    t_r = 0.9965*t_r - 0.0909;
  }else{
    t_r = 0.9962*t_r - 0.0553;
  }
  return t_r;
}
float CAP_ZOOM_10(float t_r){
  if(t_r<100){
    t_r = 104.805*t_r - 2;
  }
  else{
    t_r = 104.81*t_r - 11.168;
  }
  return t_r;
}
float CAP_ZOOM_50(float t_r){
  if(t_r<100){
    t_r = 9.9657*t_r - 0.5881;
  }else{
    t_r = 9.966*t_r - 0.3323;
  }
  return t_r;
}


void Print_RES(void){
  
}