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
  extern int Capture;
  extern u16 data;
  extern u16 TIM3_SEND_DATA;
  extern u8 captureEnable;
#endif /* TIMER_H_ */
