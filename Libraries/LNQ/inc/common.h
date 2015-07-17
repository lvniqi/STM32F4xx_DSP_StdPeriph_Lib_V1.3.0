/*
 * Common.h
 *
 *  Created on: 2013-12-2
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */

#ifndef COMMON_H_
  #define COMMON_H_

  #include "stm32f4xx_it.h"
  #include "stm32f4xx_exti.h"
  #include "stm32f4xx_gpio.h"
  #include "stm32f4xx.h"
  #include "Sequeue.h"
  #include "pingpang.h"
  #include "USART.h"
  #include "GPIO.h"
  #include <stdio.h>
  #include "pt.h"
  #define pass asm("nop")
  #define True 1
  #define False 0
  #define not !
  #define or ||
  #define and &&
  #define elif else if
  #define BIT(x) (1<<(x))
  /*数据寄存器地址*/
  #define DRIVE_DR_BASE(DRIVE) ((uint32_t)&((DRIVE->DR)))
  //RCC时钟数据
  extern RCC_ClocksTypeDef RCC_Clocks;
  extern u32  SYS_TIME;
  extern PT *THREADS_HEAD;
  extern int fputc(int ch, FILE *f);
  extern void delay_ms(volatile u16 time);
  extern u16 Num_Len(long num);
  typedef volatile unsigned short* EXTERN_RAM;
  #define PT_DEBUG(level,...)   { printf("\033[3%d;1m",level);\
                                  printf("[%d.%d]",SYS_TIME/100,SYS_TIME%100);\
                                  printf(__VA_ARGS__);\
                                  printf("\033[0m");}
  extern void PT_SERVICE();
  extern void PT_ADD_THREAD(PT *in);
  extern void PT_REMOVE_THREAD(const char* FUNSTR);
  extern PT* PT_GET_THREAD(const char* FUNSTR);
  extern void Set_Key_Func(void(*func)(),char n);
  extern PT_THREAD(KEY_SERVICE(PT *pt));
#endif /* COMMON_H_ */
