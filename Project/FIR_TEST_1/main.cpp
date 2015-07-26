/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "arm_math.h" 
extern "C"{
  #include "main.h"
}
#define TEST_LENGTH_SAMPLES  320
#define SNR_THRESHOLD_F32    140.0f
#define BLOCK_SIZE            32
#define NUM_TAPS              29
Lcd_Curve Lcd_Curve1;
/* ----------------------------------------------------------------------
** FIR Coefficients buffer generated using fir1() MATLAB function.
** fir1(28, 6/24)
** ------------------------------------------------------------------- */
const float32_t firCoeffs32[NUM_TAPS] = {
  -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
  -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
  +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
  +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f
};
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  TIM2_Configuration(350);
  //¿ªÆôÆ¹ÅÒ»º³å
  PingPang_Data_Free_Init();
  ADC1_Init(ADC_Channel_3,ADC_ExternalTrigConvEdge_Rising);
  Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO,true,DMA_Mode_Normal);
  LCD_Curve_Init(&Lcd_Curve1,10,10,GREEN);
  while(!pingpang_ad.geted[3]){
    Ad_Get_Service();
  }
  while(1){
    Ad_Get_Service();
    Dac_Send_Service();
    _pingpang_data *temp = PingPang_Out(&pingpang_ad);
    if(temp){
      if(pingpang_da.geted[5]){
        LCD_Curve_Go_0(&Lcd_Curve1);
        for(int i=0;i<LCD_CURVE_WIDTH;i++){
          LCD_Curve_Show(&Lcd_Curve1,(temp->data)[i]/16,1);
        }
        while(pingpang_da.geted[0]){
          Dac_Send_Service();
        }
      }
      /*
      for(int i=0;i<TEST_LENGTH_SAMPLES/2;i++){
        (temp->data)[i] = testOutput[i];
      }*/
      PingPang_In_By_PingPang(&pingpang_da,temp);
    }
  }
}