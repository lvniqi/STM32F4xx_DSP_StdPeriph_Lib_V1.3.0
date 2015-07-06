/*
 * Timer.c
 *
 *  Created on: 2014��7��30��
 *      Author: lvniqi
 */
#include "Timer.h"
void TIM2_Configuration(u32 div){
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_TimeBaseStructure.TIM_Period = div-1; //����0.05msһ��TIM2�Ƚϵ�����
  TIM_TimeBaseStructure.TIM_Prescaler = 11;
  //ϵͳ��Ƶ72M�������Ƶ72���൱��1M�Ķ�ʱ��2ʱ��
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); //����ʱ�� ����DAC
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //���PWM1˵��
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 10; //�����˴�װ�벶��Ƚ���������ֵ
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�����������
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High; //���û����������
  //�����PWM1ҪΪLow��PWM2��ΪHigh
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; 
    //ѡ�����״̬�µ÷ǹ���״̬
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; 
    //ѡ�񻥲�����״̬�µ÷ǹ���״̬
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  TIM_Cmd(TIM2, ENABLE);
  TIM_CtrlPWMOutputs(TIM2, ENABLE);

}
