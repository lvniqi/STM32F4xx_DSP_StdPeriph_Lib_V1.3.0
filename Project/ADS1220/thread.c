#include "main.h"
NumBar RES;
NumBar CAP;
ADS1220 DAC_1;
int_sequeue DAC_1_SEQ;
int _DAC_1_SEQ_BASE[SHIFT_FITHER_LEN+1];
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
void CS1_TEST(SelectItem* p){
  NumBar_Hide(&CAP);
  NumBar_Show(&RES);
}
void CS2_TEST(SelectItem* p){
  NumBar_Hide(&RES);
  NumBar_Show(&CAP);
}
int getMid(int_sequeue* s){
  int temp_data[SHIFT_FITHER_LEN];
  Sequeue_Copy(temp_data,s);
  for(int i=0;i<SHIFT_FITHER_LEN;i++){
    for(int j=SHIFT_FITHER_LEN-1;j>i;j--){
      if(temp_data[j]>temp_data[j-1]){
        int DAC_1 = temp_data[j];
        temp_data[j] = temp_data[j-1];
        temp_data[j-1] = DAC_1;
      }
    }
  }
  return (temp_data[SHIFT_FITHER_LEN/2-2]
    +temp_data[SHIFT_FITHER_LEN/2-1]
    +temp_data[SHIFT_FITHER_LEN/2]
    +temp_data[SHIFT_FITHER_LEN/2+1]
    +temp_data[SHIFT_FITHER_LEN/2+2])/5;
}
void SetRes(NumBar* p,int value){
  float t = *(float*)(&value);
  float last = *(float*)(&p->value);
  if(t<0){
    t = -t;
  }
  if(t>245){
    LCD_STRING t;
    t.type = _LCD_STRING_ASCII;
    t.string.ascii = "kOhm";
    NumBar_SetRTag(p,t);
  }else if(t<245){
    LCD_STRING t;
    t.type = _LCD_STRING_ASCII;
    t.string.ascii = "Ohm ";
    NumBar_SetRTag(p,t);
  }  
  LCD_ShowFloatBig_L(p->start_x,p->end_x,p->y,t,p->color);
  p->value = value;
}
void SetCap(NumBar* p,int value){
  float t = *(float*)(&value);
  if(t<0){
    t = -t;
  }
  LCD_ShowFloatBig_L(p->start_x,p->end_x,p->y,t,p->color);
  p->value = value;
}
void RES_Init(void){
  NumBar_Init(&RES,26,0,400000,0);
  LCD_STRING t;
  LCD_STRING_CHINESE chinese;
  chinese.len = 2;
  chinese.start = 0;
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese = chinese;
  NumBar_SetLTag(&RES,t);
  t.type = _LCD_STRING_ASCII;
  t.string.ascii = "Ohm ";
  NumBar_SetRTag(&RES,t);
  NumBar_SetFunc(&RES,SetRes);
  NumBar_DisableNumShow(&RES);
  NumBar_Show(&RES);
  //NumBar_ShowRect(&RES);
}
void CAP_Init(void){
  NumBar_Init(&CAP,11,0,400000,0);
  LCD_STRING t;
  LCD_STRING_CHINESE chinese;
  chinese.len = 2;
  chinese.start = 2;
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese = chinese;
  NumBar_SetLTag(&CAP,t);
  t.type = _LCD_STRING_ASCII;
  t.string.ascii = "nF";
  NumBar_SetRTag(&CAP,t);
  NumBar_SetFunc(&CAP,SetCap);
  NumBar_DisableNumShow(&CAP);
  NumBar_Show(&CAP);
  //NumBar_ShowRect(&CAP);
}
void Init(void){
  LCD_Init();
  EXTI_init();
  ADS1220_Init(&DAC_1);
  Dds_SetFreq(15);
  Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO,true,DMA_Mode_Circular);
  DAC_Cmd(DAC_Channel_1,ENABLE);
  DMA_Cmd(DAC1_DMA_STREAM, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  ADS1220_Set_Rate(&DAC_1,20);
  ADS1220_PGA_OFF(&DAC_1);
  //ADS1220_PGA_ON(&t);
  //1 +68074*i
  //ADS1220_Set_Gain(&t,GATE_ZOOM);
  //ADS1220_Continuous_Mode_ON(&t);
  Sequeue_Init(&DAC_1_SEQ,_DAC_1_SEQ_BASE,SHIFT_FITHER_LEN+1);
  while(!Sequeue_Full(&DAC_1_SEQ)){
    if(ADS1220_DRDY_PIN == 0){
      Sequeue_In_Queue(&DAC_1_SEQ,ADS1220_Read_Data());
    }
    delay_ms(1);
  }
  
  RES_Init();
  CAP_Init();
  
  
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Connect EXTI Line7 to PA7 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);
  /* Configure EXTI Line7 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_ClearFlag(EXTI_Line7);
  /* Enable and set EXTI Line1 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
PT_THREAD(PRINT_TEST(PT *pt)){
  PT_BEGIN(pt);
  while(1){
    PT_WAIT_UNTIL(pt,pt->ready);
    pt->ready = 0;
    __disable_irq();
    double t2 = getMid(&DAC_1_SEQ);
    u32 d = Sequeue_Get_Rear(&DAC_1_SEQ);
    __enable_irq();
    char c[40];
    sprintf(c,"0x%x        ",d);
    LCD_ShowNumBig_L(10,20,3,t2,RED); 
    LCD_ShowStringBig(10,2,LCD_STRING_LEFT,c,RED);
    t2 = t2*4096.0/0x7fffff;
    //0~500
    double t3 = (REG*t2/(4096-t2));
    double t4 = (1.0014*t3 - 47.333);
    LCD_ShowNumBig_L(10,20,1,t4,RED);
    float t5 = t4;
    NumBar_SetValue(&RES,*(int*)(&t5));
    float t6 = t3;
    NumBar_SetValue(&CAP,*(int*)(&t6));
  }
  PT_END(pt);
}