/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/

  #include "stm32f4xx.h"
  #include "stm32f4xx_fsmc.h"
  #include "LCD.h"
  #include "exti.h"
  #include "USART.h"
  #include "AD.h"
  #include "DA.h"
  #include "Timer.h"
  #include "Pingpang.h"
  #include "stm32f4xx_dac.h"
  #include <stdbool.h>
  #include "malloc.h"
  #include "math.h"
  //滑动滤波长度
  #define SHIFT_FITHER_LEN 20
  //通道数 4个
  #define CHANNEL_LEN 2
  //模式数 3个
  #define  MODE_LEN 3
  extern const u32 MODE_REG[MODE_LEN];
  extern double FREQ_RESULT;
  extern u8 FREQ_USED_POS;
  extern void FPGA_EXIT_INIT(void);
  extern double getFreq();
  extern double_sequeue FREQ_SEQ[CHANNEL_LEN][2];
  extern double _FREQ_SEQ_BASE[CHANNEL_LEN*2][SHIFT_FITHER_LEN+1];
  extern void Freq_Init_ALL();
  extern void Freq_Init(int channel,int x);
  extern double Freq_Get();
  void Freq_Input(int channel ,int x,double freq);
  
  extern double CHINNAL_1_RECV();
  extern double CHINNAL_3_RECV();
  
  extern PT_THREAD(FREQ_GET_SERVICE(PT *pt));
  
#endif /* __MAIN_H */
