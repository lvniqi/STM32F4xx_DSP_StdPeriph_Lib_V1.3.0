/* Includes ------------------------------------------------------------------*/
#include "main.h"
#define SHIFT_FITHER_LEN 13
#define GATE_ZOOM 1
const u32 ERROR_ADD[12] = {68574,136148,255200,593100};
double Dds_SetFreq(u32 div){
  TIM2_Configuration(div);
  return  14000000.0/(64*div*2);
}
void Sequeue_Copy(int* r,int_sequeue* s){
  int len = Sequeue_Getlen(s);
  for(int i=0;i<len;i++){
    r[i] = Sequeue_Get_One(s,i);
  }
}
int getMid(int_sequeue* s){
  int temp_data[SHIFT_FITHER_LEN];
  Sequeue_Copy(temp_data,s);
  for(int i=0;i<SHIFT_FITHER_LEN;i++){
    for(int j=SHIFT_FITHER_LEN-1;j>i;j--){
      if(temp_data[j]>temp_data[j-1]){
        int t = temp_data[j];
        temp_data[j] = temp_data[j-1];
        temp_data[j-1] = t;
      }
    }
  }
  return (temp_data[SHIFT_FITHER_LEN/2-2]
    +temp_data[SHIFT_FITHER_LEN/2-1]
    +temp_data[SHIFT_FITHER_LEN/2]
    +temp_data[SHIFT_FITHER_LEN/2+1]
    +temp_data[SHIFT_FITHER_LEN/2+2])/5;
}

int t_data[SHIFT_FITHER_LEN];
int main(void){
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  Usart1_Dma_Open(115200);
  LCD_Init();
  ADS1220 t;
  ADS1220_Init(&t);
  printf("Freq:%d\r\n",(int)Dds_SetFreq(100));
  Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO,true,DMA_Mode_Circular);
  DAC_Cmd(DAC_Channel_1,ENABLE);
  DMA_Cmd(DAC1_DMA_STREAM, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  
  ADS1220_Set_Rate(&t,20);
  //ADS1220_PGA_ON(&t);
  //1 +68074*i
  //ADS1220_Set_Gain(&t,GATE_ZOOM);
  ADS1220_PGA_OFF(&t);
  //ADS1220_Continuous_Mode_ON(&t);
  int_sequeue s_t;
  int b_t[SHIFT_FITHER_LEN+1];
  Sequeue_Init(&s_t,b_t,SHIFT_FITHER_LEN+1);
  while(!Sequeue_Full(&s_t)){
    if(ADS1220_DRDY_PIN == 0){
      Sequeue_In_Queue(&s_t,ADS1220_Read_Data());
    }
    delay_ms(1);
  }
  int t_before;
  while(1){
    if(ADS1220_DRDY_PIN == 0){
      int d = ADS1220_Read_Data();
      Sequeue_Out_Queue(&s_t);
      Sequeue_In_Queue(&s_t,d);
      int t2 = getMid(&s_t);
      char c[40];
      sprintf(c,"0x%x        ",d);
      LCD_ShowNumBig_L(0,20,2,t2,RED); 
      LCD_ShowStringBig(50,1,LCD_STRING_LEFT,c,RED); 
      LCD_ShowNumBig_L(0,20,0,t2*204800.0/0x7fffff/GATE_ZOOM,RED); 
    }
    Usart1_Buffer_Tx_Service();
    Usart1_Buffer_Rx_Service();
  }
}