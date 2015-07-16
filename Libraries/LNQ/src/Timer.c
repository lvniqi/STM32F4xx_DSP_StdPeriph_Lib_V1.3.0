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
void TIM4_Configuration(void){
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  TIM_TimeBaseStructure.TIM_Period = 6; //设置1us一次TIM4比较的周期
  TIM_TimeBaseStructure.TIM_Prescaler = 2;
  //系统主频72M，这里分频72，相当于1M的定时器2时钟
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //清除溢出中断标志
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  //禁止ARR预装载缓冲器
  TIM_ARRPreloadConfig(TIM4, DISABLE);
  //开启TIM4的中断
  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM4, ENABLE);
}
int dat = 0xF5;
#define TIM4_BASE_1 90
#define TIM4_DIFF 5
void TIM4_IRQHandler(){
  static int i = 0;
  static int l_count=0,count = 0;
  u32 t;
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  count++;
  if(count == 90){
    PEout(5) = !PEin(5);
  }else if(count == 180){
    PEout(5) = !PEin(5);
    l_count = count;
  }else{
    if(dat&BIT(i/2)){
      t = 1;
    }else{
      t = 0;
    }
    if((t*TIM4_DIFF+TIM4_BASE_1) == count-l_count&&l_count != 0){
      PEout(5) = !PEin(5);
      i++;
      l_count = count;
      if(i == 16){
        i=0;
        l_count = 0;
        count = 0;
      }
    }
  }
}
