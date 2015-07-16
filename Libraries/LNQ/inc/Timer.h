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
  extern void TIM4_Delay_hus(int us);
#endif /* TIMER_H_ */
