#include "main.h"
NumBar RES;
NumBar CAP;
NumBar RES_SAMPLE,RES_RANGE;
NumBar CAP_SAMPLE,CAP_RANGE;
SelectBar main_menu;
SelectBar sub_menu;
ADS1220 ADC_1;
int_sequeue ADC_1_SEQ;
int _ADC_1_SEQ_BASE[SHIFT_FITHER_LEN+1];
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
        int ADC_1 = temp_data[j];
        temp_data[j] = temp_data[j-1];
        temp_data[j-1] = ADC_1;
      }
    }
  }
  return (temp_data[SHIFT_FITHER_LEN/2-2]
    +temp_data[SHIFT_FITHER_LEN/2-1]
    +temp_data[SHIFT_FITHER_LEN/2]
    +temp_data[SHIFT_FITHER_LEN/2+1]
    +temp_data[SHIFT_FITHER_LEN/2+2])/5;
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
  NumBar_ClearPrintFunc(&RES);
  NumBar_Show(&RES);
  //NumBar_ShowRect(&RES);
  NumBar_Init(&RES_SAMPLE,12,3,50000,0);
  NumBar_Init(&RES_RANGE,10,2,100,1);
  NumBar_SetRTag(&RES_SAMPLE,t);
  t.string.ascii = "%";
  NumBar_SetRTag(&RES_RANGE,t);
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese.start =52;
  t.string.chinese.len =3;
  NumBar_SetLTag(&RES_SAMPLE,t);
  NumBar_Hide(&RES_SAMPLE);
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese.start =49;
  t.string.chinese.len =3;
  NumBar_SetLTag(&RES_RANGE,t);
  NumBar_Hide(&RES_RANGE);
  NumBar_SetLtagColor(&RES_RANGE,BLUE);
  NumBar_SetRtagColor(&RES_RANGE,BLUE);
  NumBar_SetLtagColor(&RES_SAMPLE,BLUE);
  NumBar_SetRtagColor(&RES_SAMPLE,BLUE);
  
  GPIO_InitTypeDef GPIO_InitStructure;
  //E5 E3 E1 B9
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void CAP_Init(void){
  NumBar_Init(&CAP,26,0,400000,0);
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
  NumBar_ClearPrintFunc(&CAP);
  NumBar_Show(&CAP);
  //NumBar_ShowRect(&CAP);
  NumBar_Init(&CAP_SAMPLE,10,3,500,0);
  NumBar_Init(&CAP_RANGE,10,2,100,1);
  NumBar_SetRTag(&CAP_SAMPLE,t);
  t.string.ascii = "%";
  NumBar_SetRTag(&CAP_RANGE,t);
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese.start =55;
  t.string.chinese.len =3;
  NumBar_SetLTag(&CAP_SAMPLE,t);
  NumBar_Hide(&CAP_SAMPLE);
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese.start =49;
  t.string.chinese.len =3;
  NumBar_SetLTag(&CAP_RANGE,t);
  NumBar_Hide(&CAP_RANGE);
  NumBar_SetLtagColor(&CAP_RANGE,BLUE);
  NumBar_SetRtagColor(&CAP_RANGE,BLUE);
  NumBar_SetLtagColor(&CAP_SAMPLE,BLUE);
  NumBar_SetRtagColor(&CAP_SAMPLE,BLUE);
}
void MenuInit(void){
  
  SelectBar_Init(&main_menu,0,5,9);
  LCD_STRING t;
  t.type = _LCD_STRING_CHINESE;
  //sub menu
  t.string.chinese.start =39;
  t.string.chinese.len =4;
  SelectBar_Init(&sub_menu,0,5,4);
  SelectBar_SetTitle(&sub_menu,t);
  SelectBar_Show(&sub_menu);
  
  //main menu
  t.string.chinese.start =20;
  t.string.chinese.len =4;
  SelectBar_AddSelect(&main_menu,t);

  t.string.chinese.start =24;
  t.string.chinese.len =4;
  SelectBar_AddSelect(&main_menu,t);
  
  t.string.chinese.start =32;
  t.string.chinese.len =4;
  SelectBar_AddSelect(&main_menu,t);
  
  t.string.chinese.start =28;
  t.string.chinese.len =4;
  SelectBar_AddSelect(&main_menu,t);
  
  t.string.chinese.start =36;
  t.string.chinese.len =3;
  SelectBar_SetTitle(&main_menu,t);
  SelectItem_SetFunc_En(SelectBar_GetSelect(&main_menu,0),RES_EN);
  SelectItem_SetFunc_Un(SelectBar_GetSelect(&main_menu,0),RES_UN);
  SelectItem_SetFunc_En(SelectBar_GetSelect(&main_menu,1),CAP_EN);
  SelectItem_SetFunc_Un(SelectBar_GetSelect(&main_menu,1),CAP_UN);
  SelectItem_SetFunc_En(SelectBar_GetSelect(&main_menu,2),RES_FIND_EN);
  SelectItem_SetFunc_Un(SelectBar_GetSelect(&main_menu,2),RES_FIND_UN);
  SelectItem_SetFunc_En(SelectBar_GetSelect(&main_menu,3),CAP_FIND_EN);
  SelectItem_SetFunc_Un(SelectBar_GetSelect(&main_menu,3),CAP_FIND_UN);
  SelectBar_Show(&main_menu);
}
void Init(void){
  LCD_Init();
  EXTI_init();
  ADS1220_Init(&ADC_1);
  Dds_SetFreq(60);
  Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO,true,DMA_Mode_Circular);
  DAC_Cmd(DAC_Channel_1,ENABLE);
  DMA_Cmd(DAC1_DMA_STREAM, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  ADS1220_Set_Rate(&ADC_1,20);
  ADS1220_PGA_OFF(&ADC_1);
  //ADS1220_PGA_ON(&t);
  //1 +68074*i
  //ADS1220_Set_Gain(&t,GATE_ZOOM);
  //ADS1220_Continuous_Mode_ON(&t);
  Sequeue_Init(&ADC_1_SEQ,_ADC_1_SEQ_BASE,SHIFT_FITHER_LEN+1);
  while(!Sequeue_Full(&ADC_1_SEQ)){
    if(ADS1220_DRDY_PIN == 0){
      Sequeue_In_Queue(&ADC_1_SEQ,ADS1220_Read_Data());
    }
    delay_ms(1);
  }
  
  RES_Init();
  CAP_Init();
  MenuInit();
  
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
    double t2 = getMid(&ADC_1_SEQ);
    u32 d = Sequeue_Get_Rear(&ADC_1_SEQ);
    __enable_irq();
    t2 = t2*4096.0/0x7fffff;
    t2 = 1.0004*t2 + 0.8115;
    if(SelectBar_GetPos(&main_menu) == 3){
      float t_r = Get_CAP_AUTO(t2);
      if(t_r>499.999||t2>4090.0){
        t_r = 499.999;
      }
      float range;
      if(t_r < NumBar_GetValue(&CAP_SAMPLE)){
        range = NumBar_GetValue(&CAP_SAMPLE)-t_r;
      }else{
        range = t_r-NumBar_GetValue(&CAP_SAMPLE);
      }
      if((range/NumBar_GetValue(&CAP_SAMPLE))*100<NumBar_GetValue(&CAP_RANGE)){
        LCD_STRING t;
        LCD_STRING_CHINESE chinese;
        chinese.len = 3;
        chinese.start = 59;
        t.type = _LCD_STRING_CHINESE;
        t.string.chinese = chinese;
        LCD_ShowStringBig_Union(0,0,LCD_STRING_LEFT,t,GREEN);
      }else{
        LCD_STRING t;
        LCD_STRING_CHINESE chinese;
        chinese.len = 3;
        chinese.start = 58;
        t.type = _LCD_STRING_CHINESE;
        t.string.chinese = chinese;
        LCD_ShowStringBig_Union(0,0,LCD_STRING_LEFT,t,RED);
      }
      NumBar_SetValue(&CAP,*(int*)(&t_r));
    }
    if(SelectBar_GetPos(&main_menu) == 2){
      float t_r = Get_RES_AUTO(t2);
      if(t_r>49999.9||t2>4090.0){
        t_r = 49999.9;
      }
      float range;
      if(t_r < NumBar_GetValue(&RES_SAMPLE)){
        range = NumBar_GetValue(&RES_SAMPLE)-t_r;
      }else{
        range = t_r-NumBar_GetValue(&RES_SAMPLE);
      }
      if((range/NumBar_GetValue(&RES_SAMPLE))*100<NumBar_GetValue(&RES_RANGE)){
        LCD_STRING t;
        LCD_STRING_CHINESE chinese;
        chinese.len = 3;
        chinese.start = 59;
        t.type = _LCD_STRING_CHINESE;
        t.string.chinese = chinese;
        LCD_ShowStringBig_Union(0,0,LCD_STRING_LEFT,t,GREEN);
      }else{
        LCD_STRING t;
        LCD_STRING_CHINESE chinese;
        chinese.len = 3;
        chinese.start = 58;
        t.type = _LCD_STRING_CHINESE;
        t.string.chinese = chinese;
        LCD_ShowStringBig_Union(0,0,LCD_STRING_LEFT,t,RED);
      }
      NumBar_SetValue(&RES,*(int*)(&t_r));
    }
    else if(SelectBar_GetPos(&main_menu) == 1){
      float t_r = Get_CAP(t2);
      NumBar_SetValue(&CAP,*(int*)(&t_r));
    }
    else if(SelectBar_GetPos(&main_menu) == 0){
      float t_r = Get_RES(t2);
      NumBar_SetValue(&RES,*(int*)(&t_r));
    }
  }
  PT_END(pt);
}