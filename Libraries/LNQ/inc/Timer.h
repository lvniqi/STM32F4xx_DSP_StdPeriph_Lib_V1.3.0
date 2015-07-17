/*
 * Timer.h
 *
 *  Created on: 2014Äê7ÔÂ30ÈÕ
 *      Author: lvniqi
 */

#ifndef TIMER_H_
  #define TIMER_H_
  #include "common.h"
  extern void TIM2_Configuration(u32 div);
  extern void TIM4_Configuration();
  extern void TIM3_Configuration();
  extern u16 TIM4_GET_DATA;
  extern u16 TIM3_SEND_DATA;
  extern u32 TIM4_GET_COUNT;
#endif /* TIMER_H_ */
