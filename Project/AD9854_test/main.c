#include "main.h"
#include "FPGA.h"
Lcd_Curve Lcd_Curve1;
NumBar FREQ;
NumBar VPP;
void UpdateFreq(int value){
  AD9854_SetSinc(value*1000,VPP.value/1000.0*4095);
  *((EXTERN_RAM)0x600a0014) = 0;
}
void UpdateVpp(int value){
  AD9854_SetSinc(FREQ.value*1000,value/1000.0*4095);
  *((EXTERN_RAM)0x600a0014) = 0;
}
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  LCD_Init();
  LCD_Curve_Init(&Lcd_Curve1,LCD_WIDTH/2-LCD_CURVE_WIDTH/2,40,RED);
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
  NumBar_setFunc(&FREQ,UpdateFreq);
  NumBar_Show(&FREQ);
  NumBar_SetActive(&FREQ,true);
  NumBar_SetValue(&FREQ,100);
  NumBar_Init(&VPP,LCD_STRING_WIDTH-2,0,1000,0);
  t.type = _LCD_STRING_ASCII;
  t.string.ascii = "Vpp";
  NumBar_SetLTag(&VPP,t);
  t.string.ascii = "mV";
  NumBar_SetRTag(&VPP,t);
  NumBar_Show(&VPP);
  NumBar_setFunc(&VPP,UpdateVpp);
  NumBar_SetValue(&VPP,500);
  //LCD_ShowNumBig(0,20,LCD_STRING_LEFT,50000,RED);
  //LCD_ShowChineseStringBig(0,0,LCD_STRING_LEFT,0,3,RED);
  //LCD_ShowStringBig(LCD_STRING_WIDTH,0,LCD_STRING_RIGHT,"123ABCDE",RED);
  //LCD_ShowStringBig(LCD_STRING_WIDTH,9,LCD_STRING_RIGHT,"123abcde",RED);
  
  while(1){
    if(MAIN_KEY.keysign){
      MAIN_KEY.keysign = 0;
      if(MAIN_KEY.keycode == 0){
        NumBar_SetValue_GUI(&FREQ);
      }
      if(MAIN_KEY.keycode == 4){
        if(NumBar_IsActive(&FREQ)){
          NumBar_SetActive(&FREQ,false);
          NumBar_SetActive(&VPP,true);
        }else{
          NumBar_SetActive(&VPP,false);
          NumBar_SetActive(&FREQ,true);
        }
      }
      if(MAIN_KEY.keycode == 10){
        NumBar_Add(&FREQ);
        NumBar_Add(&VPP);
      }
      else if(MAIN_KEY.keycode == 14){
        NumBar_Cut(&FREQ);
        NumBar_Cut(&VPP);
      }
      else if(MAIN_KEY.keycode == 13){
        NumBar_ShiftLeft(&FREQ);
        NumBar_ShiftLeft(&VPP);
      }
      else if(MAIN_KEY.keycode == 15){
        NumBar_ShiftRight(&FREQ);
        NumBar_ShiftRight(&VPP);
      }
    }
  }
}