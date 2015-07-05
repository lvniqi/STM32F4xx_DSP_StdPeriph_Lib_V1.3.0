#include "main.h"
#include "FPGA.h"
#include "AD.h"
#include "math.h"
Lcd_Curve Lcd_Curve1;
Lcd_Curve Lcd_Curve2;
Lcd_Curve Lcd_Curve3;
Lcd_Curve Lcd_Curve4;
NumBar FREQ;
int I_BASE;
int I_GET;
int Q_BASE;
int Q_GET;
int I[400];
int Q[400];
double V[400];
double V_MAX;
void UpdateFreq(int value){
  double t = 2*pow(10,-10)*pow(value,2)-pow(10,-5)*value+1.7988;
  double vp = 1*1.5/t;
  AD9854_SetSinc(value*1000,vp*4095);
}
u16 getAvr(u16 *p,int len){
  u32 sum=0;
  for(int i=0;i<len;i++){
    sum += p[i];
  }
  return sum/len;
}
void TEST_Init(){
  NumBar_SetValue(&FREQ,0);
  ADC1_Soft_Init(ADC1,ADC_Channel_7);
  ADC1_Soft_Init(ADC2,ADC_Channel_6);
  delay_ms(10);
  u16 t_I[10];
  u16 t_Q[10];
  for(int i=0;i<10;i++){
    ADC_SoftwareStartConv(ADC2);
    ADC_SoftwareStartConv(ADC1);
    delay_ms(1);
    t_I[i] = ADC_GetConversionValue(ADC2);
    t_Q[i] = ADC_GetConversionValue(ADC1);
  }
  I_BASE = getAvr(t_I,10);
  Q_BASE = getAvr(t_Q,10);
}
void TEST_1(){
  u16 t_I[10];
  u16 t_Q[10];
  //delay_ms((40000-FREQ.value)/5000);
  for(int i=0;i<10;i++){
    ADC_SoftwareStartConv(ADC2);
    ADC_SoftwareStartConv(ADC1);
    t_I[i] = ADC_GetConversionValue(ADC2);
    t_Q[i] = ADC_GetConversionValue(ADC1);
  }
  I_GET = getAvr(t_I,10)-I_BASE;
  if(I_GET<0){
    I_GET = 0;
  }
  Q_GET = getAvr(t_Q,10)-Q_BASE;
  //Lcd_Curve1.finish_flag = 0;
  //(Lcd_Curve1.Now_x) = 0;
  //LCD_Curve_Show(&Lcd_Curve1,j,1);
}

double TEST_1_5(){
  double max = 0;
  int min = 0;
  double x1,x2;
  for(int i=0;i<400;i++){
    V[i] = sqrt(pow(I[i],2)+pow(Q[i],2));
    if(V[i] > max){
      max = V[i];
    }
  }
  V_MAX = max;
  for(int i=0;i<400;i++){
    V[i] = 20*log(max/V[i]);
    LCD_Curve_Show(&Lcd_Curve4,256-V[i],1);
    if(V[i]<V[min]){
      min = i;
    }
  }
  for(int i=0;i<400;i++){
    if(V[i]<3){
      x1 = i;
      break;
    }
  }
  for(int i=399;i>0;i--){
    if(V[i]<3){
      x2 = i;
      break;
    }
  }
  //LCD_ShowNumBig(10,6,LCD_STRING_RIGHT,min*100,RED);
  //LCD_ShowNumBig(10,5,LCD_STRING_RIGHT,(x2-x1)*100,RED);
}
void TEST_2(){
  Lcd_Curve1.finish_flag = 0;
  (Lcd_Curve1.Now_x) = 0;
  Lcd_Curve2.finish_flag = 0;
  (Lcd_Curve2.Now_x) = 0;
  Lcd_Curve3.finish_flag = 0;
  (Lcd_Curve3.Now_x) = 0;
  Lcd_Curve4.finish_flag = 0;
  (Lcd_Curve4.Now_x) = 0;
  int min;
  for(int i=0;i<400;i++){
    NumBar_SetValue(&FREQ,100*i);
    delay_ms(1);
    TEST_1();
    I[i] = I_GET;
    Q[i] = Q_GET;
    LCD_Curve_Show(&Lcd_Curve1,(Q_GET>>3)+128,1);
    LCD_Curve_Show(&Lcd_Curve2,(I_GET>>3),1);
    double t = atan((-Q_GET)*1.0/I_GET);
    t = t/3.14*180;
    LCD_Curve_Show(&Lcd_Curve3,(t+128),1);
    if(t<2.5&&t>-2.5){
      min = i;
    }
  }
  //LCD_ShowNumBig(10,4,LCD_STRING_RIGHT,min*100,RED);
  TEST_1_5();
}
void UpdateFreqPoint(int value){
  double t = 2*pow(10,-10)*pow(value,2)-pow(10,-5)*value+1.7988;
  double vp = 1*1.5/t;
  AD9854_SetSinc(value*1000,vp*4095);
  delay_ms(200);
  TEST_1();
  double t2 = atan((-Q_GET)*1.0/I_GET)/3.14*180;
  LCD_ShowNumBig_L(10,20,4,t2,RED);
  double t_v = sqrt(pow(I_GET,2)+pow(Q_GET,2));
  t_v = 20*log(t_v/V_MAX);
  LCD_ShowNumBig_L(10,20,3,t_v,RED);
}
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  LCD_Init();
  LCD_Curve_Init(&Lcd_Curve1,LCD_WIDTH/2-LCD_CURVE_WIDTH/2,40,RED);
  LCD_Curve_Init(&Lcd_Curve2,LCD_WIDTH/2-LCD_CURVE_WIDTH/2,40,WHITE);
  LCD_Curve_Init(&Lcd_Curve3,LCD_WIDTH/2-LCD_CURVE_WIDTH/2,40,YELLOW);
  LCD_Curve_Init(&Lcd_Curve4,LCD_WIDTH/2-LCD_CURVE_WIDTH/2,40,BLUE);
  EXTI_init();
  
  u16 i = 1;
  AD9854_Init();
  NumBar_Init(&FREQ,10,0,40000,0);
  LCD_STRING t;
  t.type = _LCD_STRING_ASCII;
  t.string.ascii = "Freq";
  NumBar_SetLTag(&FREQ,t);
  t.string.ascii = "kHZ";
  NumBar_SetRTag(&FREQ,t);
  NumBar_SetFunc(&FREQ,UpdateFreq);
  NumBar_Show(&FREQ);
  NumBar_SetActive(&FREQ,false);
  NumBar_SetValue(&FREQ,100);
  delay_ms(100);
  TEST_Init();
  TEST_2();
  while(1){
    if(MAIN_KEY.keysign){
      MAIN_KEY.keysign = 0;
      if(MAIN_KEY.keycode == 0){
        NumBar_SetValue_GUI(&FREQ);
      }
      if(MAIN_KEY.keycode == 4){
        static bool isCurve = true;
        if(isCurve){
          LCD_Show_Rect_FULL(LCD_WIDTH/2-LCD_CURVE_WIDTH/2-1,39,LCD_CURVE_HEIGHT+3,LCD_CURVE_WIDTH+3,BACK_COLOR);
          isCurve = false;
          NumBar_SetActive(&FREQ,true);
          NumBar_SetFunc(&FREQ,UpdateFreqPoint);
        }else{
          LCD_Show_Rect_FULL(LCD_WIDTH/2-LCD_CURVE_WIDTH/2-1,39,LCD_CURVE_HEIGHT+3,LCD_CURVE_WIDTH+3,BACK_COLOR);
          NumBar_SetFunc(&FREQ,UpdateFreq);
          NumBar_SetActive(&FREQ,false);
          LCD_Curve_Init(&Lcd_Curve1,LCD_WIDTH/2-LCD_CURVE_WIDTH/2,40,RED);
          LCD_Curve_Init(&Lcd_Curve2,LCD_WIDTH/2-LCD_CURVE_WIDTH/2,40,WHITE);
          LCD_Curve_Init(&Lcd_Curve3,LCD_WIDTH/2-LCD_CURVE_WIDTH/2,40,YELLOW);
          LCD_Curve_Init(&Lcd_Curve4,LCD_WIDTH/2-LCD_CURVE_WIDTH/2,40,BLUE);
          TEST_Init();
          TEST_2();
          isCurve = true;
        }
      }
      if(MAIN_KEY.keycode == 10){
        NumBar_Add(&FREQ);
      }
      else if(MAIN_KEY.keycode == 14){
        NumBar_Cut(&FREQ);
      }
      else if(MAIN_KEY.keycode == 13){
        NumBar_ShiftLeft(&FREQ);
      }
      else if(MAIN_KEY.keycode == 15){
        NumBar_ShiftRight(&FREQ);
      }
    }
  }
}