#include "NumBar_CPP.h"
#include "Freq.h"

const u32 MODE_REG[MODE_LEN] = {0x10,0x20};
//频率数据
double_sequeue FREQ_SEQ[CHANNEL_LEN][2];
double _FREQ_SEQ_BASE[CHANNEL_LEN*2][SHIFT_FITHER_LEN+1];
u8 FREQ_USED_POS;
double FREQ_RESULT;

//FPGA中断初始化
void FPGA_EXIT_INIT(void){
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  //Enable GPIOB clock 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  // Enable SYSCFG clock 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Configure PB0 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Connect EXTI Line0 to PB0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
  /* Configure EXTI Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_ClearFlag(EXTI_Line0);
  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
//通道1接收
double CHINNAL_1_RECV(){
  long t_b = *(EXTERN_RAM)(MODE_REG[0]+0x600a0000);
  t_b |= *(EXTERN_RAM)(MODE_REG[0]+0x600a0002)<<16;
  long t_t = *(EXTERN_RAM)(MODE_REG[0]+0x600a0004);
  t_t |= *(EXTERN_RAM)(MODE_REG[0]+0x600a0006)<<16;
  return 0.99999*((double)t_t)/t_b*25000000;
}
//通道3接收
double CHINNAL_3_RECV(){
  long t_b2 = *(EXTERN_RAM)(MODE_REG[0]+0x600a0008);
  t_b2 |= *(EXTERN_RAM)(MODE_REG[0]+0x600a000a)<<16;
  return 0.99999*((double)25000000)/t_b2;
}

//初始化全部缓冲区
void Freq_Init_ALL(){
  for(int i=0;i<CHANNEL_LEN;i++){
    for(int j=0;j<2;j++){
      Freq_Init(i,j);
    }
  }
}
//初始化单个缓冲区
void Freq_Init(int channel,int x){
  Sequeue_Init(&(FREQ_SEQ[channel][x]),_FREQ_SEQ_BASE[channel*2+x],SHIFT_FITHER_LEN+1);
}
//输入单个double型数据到通道
void Freq_Input(int channel ,int x,double freq){
  if(Sequeue_Getlen(&(FREQ_SEQ[channel][x]))>SHIFT_FITHER_LEN-1){
    Sequeue_Out_Queue(&(FREQ_SEQ[channel][x]));
  }
  Sequeue_In_Queue(&(FREQ_SEQ[channel][x]),freq);
}

//修正频率值单位
void Freq_Value_UnitFix(NumBar_CPP* p,int value){
  static u8 UnitLevel = 0;
  double *value_t = (double*)(void*)value;
  if(*value_t>1050000){
    p->setValueReal(*value_t/1000000);
    if(UnitLevel != 2){
      p->setRtag(String_L(0,0,"MHz",WHITE));
    }
    UnitLevel = 2;
  }
  else if(*value_t>1050){
    p->setValueReal(*value_t/1000);
    if(UnitLevel != 1){
      p->setRtag(String_L(0,0,"kHz",WHITE));
    }
    UnitLevel = 1;
  }
  else if(*value_t<999){
    if(UnitLevel != 0){
      p->setRtag(String_L(0,0,"Hz ",WHITE));
    }
    UnitLevel = 0;
  }
}
//得到数据 给用户预留
double Freq_Get(){
  if(FREQ_USED_POS == 0){
    return getMid(&(FREQ_SEQ[0][0]));
  }else if(FREQ_USED_POS == 2){
    return getMid(&(FREQ_SEQ[1][1]));
  }
}
//FREQ数据获得服务函数
PT_THREAD(FREQ_GET_SERVICE(PT *pt)){
  static u8 counter_1 = 0;
  static u8 counter_2 = 0;
  static u8 counter_3 = 0;
  PT_BEGIN(pt);
  while(1){
    PT_WAIT_UNTIL(pt,pt->ready);
    pt->ready = 0;
    u8 reg_1 = ((*(EXTERN_RAM)(MODE_REG[0]+0x600a000c))&0xff00)>>8;
    //reg_1已经读不到数了 或者值太小
    if(counter_1 == reg_1 || FREQ_RESULT<34){
      //启用通道3
      __disable_irq();
      Freq_Init(1,1);
      __enable_irq();
      FREQ_USED_POS = 2;
      pt->count = 5;
      while(1){
        FREQ_USED_POS = 2;
        PT_WAIT_UNTIL(pt,pt->ready);
        pt->ready = 0;
        FREQ_RESULT = CHINNAL_3_RECV();
        if(FREQ_RESULT>42){
          pt->count = 100;
          //重启自动
          __disable_irq();
          Freq_Init(0,0);
          __enable_irq();
          FREQ_USED_POS = 0;
          break;
        }
        Freq_Input(1,1,FREQ_RESULT);
      }
    }
    counter_1 = reg_1;
  }
  PT_END(pt);
}