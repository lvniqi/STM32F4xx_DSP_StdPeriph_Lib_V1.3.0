#include "main.h"
NumBar RES;
NumBar CAP;
NumBar RES_MAX,RES_MIN;
NumBar CAP_MAX,CAP_MIN;
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
  NumBar_DisableNumShow(&RES);
  NumBar_Show(&RES);
  //NumBar_ShowRect(&RES);
  NumBar_Init(&RES_MAX,14,3,50000,0);
  NumBar_Init(&RES_MIN,14,2,50000,0);
  NumBar_SetRTag(&RES_MAX,t);
  NumBar_SetRTag(&RES_MIN,t);
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese.start =4;
  t.string.chinese.len =4;
  NumBar_SetLTag(&RES_MAX,t);
  NumBar_Hide(&RES_MAX);
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese.start =8;
  t.string.chinese.len =4;
  NumBar_SetLTag(&RES_MIN,t);
  NumBar_Hide(&RES_MIN);
  NumBar_SetLtagColor(&RES_MIN,BLUE);
  NumBar_SetRtagColor(&RES_MIN,BLUE);
  NumBar_SetLtagColor(&RES_MAX,BLUE);
  NumBar_SetRtagColor(&RES_MAX,BLUE);
  
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
  NumBar_DisableNumShow(&CAP);
  NumBar_Show(&CAP);
  //NumBar_ShowRect(&CAP);
  NumBar_Init(&CAP_MAX,14,3,50000,0);
  NumBar_Init(&CAP_MIN,14,2,50000,0);
  NumBar_SetRTag(&CAP_MAX,t);
  NumBar_SetRTag(&CAP_MIN,t);
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese.start =12;
  t.string.chinese.len =4;
  NumBar_SetLTag(&CAP_MAX,t);
  NumBar_Hide(&CAP_MAX);
  t.type = _LCD_STRING_CHINESE;
  t.string.chinese.start =16;
  t.string.chinese.len =4;
  NumBar_SetLTag(&CAP_MIN,t);
  NumBar_Hide(&CAP_MIN);
  NumBar_SetLtagColor(&CAP_MIN,BLUE);
  NumBar_SetRtagColor(&CAP_MIN,BLUE);
  NumBar_SetLtagColor(&CAP_MAX,BLUE);
  NumBar_SetRtagColor(&CAP_MAX,BLUE);
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
    char c[40];
    sprintf(c,"0x%4x  ",d);
    //LCD_ShowNumBig_L(20,30,3,t2,RED); 
    LCD_ShowStringBig(15,2,LCD_STRING_LEFT,c,RED);
    //t2 = t2*4096.0/0x7fffff;
    //0~500
    //double t3 = (REG*t2/(4096-t2));
    //double t4 = (1.0014*t3 - 47.333);
    //LCD_ShowNumBig_L(20,30,1,t4,RED);
    //float t5 = t4;
    //NumBar_SetValue(&RES,*(int*)(&t5));
    //float t6 = t3;
    //NumBar_SetValue(&CAP,*(int*)(&t6));
    //LCD_ShowNumBig_L(20,30,6,SelectBar_GetPos(&main_menu),RED);
    //LCD_ShowNumBig_L(20,30,5,SelectBar_GetPos(&sub_menu),RED);
    t2 = t2*4096.0/0x7fffff;
    t2 = 1.0004*t2 + 0.8115;
    //float t_r = t2*RES_BASE_0/(4096.0-t2);
    //500欧姆档
    /*if(t_r<50){
      t_r = 0.9965*t_r - 0.0951;
    }else{
      t_r = 0.9962*t_r - 0.0583;
    }
    if(t_r<0){
      t_r = 0;
    }*/
    //5000欧姆档
    /*if(t_r<100){
      t_r = 9.9656*t_r - 0.7881;
    }else{
      t_r = 9.9632*t_r - 0.3323;
    }*/
    //50K欧姆档
    /*if(t_r<100){
      t_r = 104.66*t_r + 0.0775;
    }
    else{
      t_r = 104.74*t_r - 11.168;
    }*/
    //电容
    //500nF
    //float t_r = t2/2969*470;
    //t_r = 0.00021*t_r*t_r + 0.8969*t_r + 2.116;
    //50nF
    //float t_r = t2/2868*46.2;
    //t_r = -0.0065*t_r*t_r + 1.309*t_r - 0.8243;
    //5nF
    //float t_r = t2/2555*14.7;
    //t_r = -0.0313*t_r*t_r + 1.0012*t_r - 0.0951;
    //NumBar_SetValue(&CAP,*(int*)(&t_r));
    //float t_r = t2*RES_BASE_0/(4096.0-t2);
    if(SelectBar_GetPos(&main_menu) == 2){
      float t_r = Get_RES_AUTO(t2);
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