/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "arm_math.h" 
#include "arm_const_structs.h"
extern "C"{
  #include "main.h"
} 
#define TEST_LENGTH_SAMPLES 2048 
//使用窗函数
float testInput_1_2048[TEST_LENGTH_SAMPLES]; 
//不使用窗函数
float testInput_2_2048[TEST_LENGTH_SAMPLES]; 
static float32_t testOutput[TEST_LENGTH_SAMPLES/2]; 
uint32_t fftSize = 1024; 
uint32_t doBitReverse = 1; 
uint32_t refIndex = 213, testIndex = 0; 
float32_t maxValue; 
Lcd_Curve Lcd_Curve1;
Lcd_Curve Lcd_Curve2;
Lcd_Curve Lcd_Curve3;
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  TIM2_Configuration(25);
  //开启乒乓缓冲
  PingPang_Data_Free_Init();
  ADC1_Init(ADC_Channel_3,ADC_ExternalTrigConvEdge_Rising);
  Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO,true,DMA_Mode_Normal);
  LCD_Curve_Init(&Lcd_Curve1,10,10,GREEN);
  LCD_Curve_Init(&Lcd_Curve2,10,10,BLUE);
  LCD_Curve_Init(&Lcd_Curve3,10,10,RED);
  while(!pingpang_ad.geted[3]){
    Ad_Get_Service();
  }
  while(1){
    Ad_Get_Service();
    Dac_Send_Service();
    _pingpang_data *temp = PingPang_Out(&pingpang_ad);
    if(temp){
      float t_f[TEST_LENGTH_SAMPLES/2];
      for(int i=0;i<TEST_LENGTH_SAMPLES/2;i++){
        t_f[i] = (temp->data)[i]*hann_1024[i];
      }
      for(int i=0;i<TEST_LENGTH_SAMPLES;i+=2){
        testInput_1_2048 [i] = t_f[i/2];
        testInput_2_2048 [i] = (temp->data)[i/2];
      }
      for(int i=1;i<TEST_LENGTH_SAMPLES;i+=2){
        testInput_1_2048 [i] = 0;
        testInput_2_2048 [i] = 0;
      }
      arm_cfft_f32(&arm_cfft_sR_f32_len1024, testInput_1_2048, false, doBitReverse);
      arm_cfft_f32(&arm_cfft_sR_f32_len1024, testInput_2_2048, false, doBitReverse);
      /*#define start_x 162
      for(int i=start_x;i<start_x+4;i++){
        testInput_2_2048[i] *= 10;
      }
      for(int i=TEST_LENGTH_SAMPLES-start_x-4;i<TEST_LENGTH_SAMPLES-start_x;i++){
        testInput_2_2048[i] *= 10;
      }*/
      //arm_cfft_f32(&arm_cfft_sR_f32_len1024, testInput_1_2048, true, doBitReverse);
      arm_cfft_f32(&arm_cfft_sR_f32_len1024, testInput_2_2048, true, doBitReverse);
      arm_cmplx_mag_f32(testInput_2_2048, testOutput, fftSize);
      arm_max_f32(testOutput, fftSize, &maxValue, &testIndex); 
      if(pingpang_da.geted[5]){
        LCD_Curve_Go_0(&Lcd_Curve1);
        LCD_Curve_Go_0(&Lcd_Curve2);
        LCD_Curve_Go_0(&Lcd_Curve3);
        float t_f[TEST_LENGTH_SAMPLES/2];
        arm_cmplx_mag_f32(testInput_1_2048, t_f, fftSize);
        for(int i=0;i<LCD_CURVE_WIDTH;i++){
          LCD_Curve_Show(&Lcd_Curve2,(temp->data)[i]/16,1);
          LCD_Curve_Show(&Lcd_Curve1,testOutput[i]/16,1);
          LCD_Curve_Show(&Lcd_Curve3,t_f[i]/5000,1);
        }
        while(pingpang_da.geted[0]){
          Dac_Send_Service();
        }
      }
      for(int i=0;i<TEST_LENGTH_SAMPLES/2;i++){
        (temp->data)[i] = testOutput[i];
      }
      PingPang_In_By_PingPang(&pingpang_da,temp);
    }
  }
}