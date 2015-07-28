/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "arm_math.h" 
extern "C"{
  #include "main.h"
}
#define TEST_LENGTH_SAMPLES  1024
#define NUM_STAGES 4 
const float SCALE_VALUE = 0.000163672332029248*0.000120705509278592*
  6.03958570287577e-05*1.78899347736049e-05*0.974679434480896;
Lcd_Curve Lcd_Curve1;
const float32_t IIRCoeffs32LP[NUM_STAGES*5] = {
  1,2,1,1.99664604259722,-0.997300731925341,
  1,2,1,1.99184910825142,-0.992331930288533,
  1,2,1,1.98830345996529,-0.988545043393400,
  1,2,1,1.98642970814294,-0.986501267882032    
};
float32_t testOutput[TEST_LENGTH_SAMPLES];
float32_t testInput[TEST_LENGTH_SAMPLES];
float32_t IIRStateF32[NUM_STAGES*4];
arm_biquad_casd_df1_inst_f32 S;
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  TIM2_Configuration(28);
  //¿ªÆôÆ¹ÅÒ»º³å
  PingPang_Data_Free_Init();
  ADC1_Init(ADC_Channel_3,ADC_ExternalTrigConvEdge_Rising);
  Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO,true,DMA_Mode_Normal);
  LCD_Curve_Init(&Lcd_Curve1,10,10,GREEN);
  arm_biquad_cascade_df1_init_f32(&S,NUM_STAGES,(float*)IIRCoeffs32LP,(float*)IIRStateF32); 
  while(!pingpang_ad.geted[3]){
    Ad_Get_Service();
  }
  u8 t_t_t;
  while(1){
    Ad_Get_Service();
    Dac_Send_Service();
    _pingpang_data *temp = PingPang_Out(&pingpang_ad);
    if(temp){
      if(pingpang_da.geted[PINGPANG_GETED_LEN-5]){
        LCD_Curve_Go_0(&Lcd_Curve1);
        for(int i=0;i<LCD_CURVE_WIDTH;i++){
          LCD_Curve_Show(&Lcd_Curve1,(temp->data)[i]/16,1);
        }
        Ad_Get_Service();
      }
      for(int i=0;i<TEST_LENGTH_SAMPLES;i++){
        testInput[i] = (temp->data)[i];
      }
      arm_biquad_cascade_df1_f32(&S, testInput, testOutput, TEST_LENGTH_SAMPLES); 
      for(int i=0;i<TEST_LENGTH_SAMPLES;i++){
        (temp->data)[i] = testOutput[i]*SCALE_VALUE;
      }
      if(!PingPang_In_By_PingPang(&pingpang_da,temp)){
        PingPang_Free(temp);
      }
    }
  }
}