/*
 * Timer.c
 *
 *  Created on: 2014年7月30日
 *      Author: lvniqi
 */
#include "Timer.h"
void TIM2_Configuration(u32 div){
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_TimeBaseStructure.TIM_Period = div-1; //设置0.05ms一次TIM2比较的周期
  TIM_TimeBaseStructure.TIM_Prescaler = 11;
  //系统主频72M，这里分频72，相当于1M的定时器2时钟
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); //发生时间 产生DAC
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //输出PWM1说明
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 10; //设置了待装入捕获比较器的脉冲值
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //设置输出极性
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High; //设置互补输出极性
  //如果是PWM1要为Low，PWM2则为High
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; 
    //选择空闲状态下得非工作状态
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; 
    //选择互补空闲状态下得非工作状态
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  TIM_Cmd(TIM2, ENABLE);
  TIM_CtrlPWMOutputs(TIM2, ENABLE);

}
void TIM3_Configuration(void){
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  TIM_TimeBaseStructure.TIM_Period = 13; //设置1us一次TIM3比较的周期
  TIM_TimeBaseStructure.TIM_Prescaler = 14;
  //系统主频72M，这里分频72，相当于1M的定时器2时钟
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //清除溢出中断标志
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  //禁止ARR预装载缓冲器
  TIM_ARRPreloadConfig(TIM3, DISABLE);
  //开启TIM3的中断
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM3, ENABLE);
}
void TIM4_Configuration(void){
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  /* TIM1 channel 2 pin (PE.11) configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Connect TIM pins to AF2 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
  
  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  /* TIM4 Time base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 41;//为了配合48K发生器，分频系数为6.006
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  
  /* TIM4 Channel4 Input capture Mode configuration */
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
  /* TIM4 counter is captured at  rising  edges */
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* Enable capture interrupt */
  TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
  /* Enable the TIM4 counter */
  TIM_Cmd(TIM4, ENABLE);
  /* Reset the flags */
  TIM4->SR = 0;
}
u32 TIM3_SEND_DATA = 0;
#define TIM3_BASE_1 18
#define TIM3_DIFF 1
void TIM3_IRQHandler(){
  static int i = 0;
  static int l_count=0,count = 0;
  u32 t;
  count++;
  if(count == TIM3_BASE_1){
    GPIO_ToggleBits(GPIOE,GPIO_Pin_5);
  }else if(count == TIM3_BASE_1*2){
    GPIO_ToggleBits(GPIOE,GPIO_Pin_5);
    l_count = count;
  }else{
    if(TIM3_SEND_DATA&BIT(i/2)){
      t = 1;
    }else{
      t = 0;
    }
    if((t*TIM3_DIFF+TIM3_BASE_1+1) <= count-l_count&&l_count != 0){
      GPIO_ToggleBits(GPIOE,GPIO_Pin_5);
      i++;
      l_count = count;
      if(i == 64){
        i=0;
        l_count = 0;
        count = 0;
      }
    }
  }
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
}
const u32 DATA_CAPTURE[4] = {87,92,97,101};
int Capture;
u32 TIM4_GET_DATA = 0;
u32 TIM4_GET_COUNT = 0;
u8 captureEnable = 0;
void TIM4_IRQHandler(void){
  static u8 pos;
  static u32 data = 0;
  static u32 counter_last,counter_this;
  if (TIM_GetITStatus(TIM4, TIM_IT_CC3) == SET){
    counter_this = TIM_GetCapture3(TIM4);
    if (counter_this > counter_last){
      Capture = (counter_this - counter_last) - 1;
    }
    else{
      Capture = ((0xFFFF - counter_last) + counter_this) - 1;
    }
    counter_last = counter_this;
    if(Capture>=DATA_CAPTURE[0]&&Capture<DATA_CAPTURE[1]){
      captureEnable = 1;
      pos = 0;
      data = 0;
    }else if(captureEnable){
      int tempf = (Capture-DATA_CAPTURE[1]);
      data += (tempf/5)<<(pos);
      pos++;
      if(pos == 32){
        captureEnable = 0;
        pos = 0;
        if((data&0x0000ffff) == ((data&0xffff0000)>>16)){
          TIM4_GET_DATA = data&0x0000ffff;
          TIM4_GET_COUNT++;
        }
      }else if(Capture<DATA_CAPTURE[0]||Capture>DATA_CAPTURE[3]){
        captureEnable = 0;
        pos = 0;
        data = 0;
      }
    }
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
  }
}