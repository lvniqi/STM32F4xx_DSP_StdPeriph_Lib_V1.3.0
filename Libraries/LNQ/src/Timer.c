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
