#ifndef __ULTRASONIC_H
  #define __ULTRASONIC_H
  #include "common.h"
  typedef struct _ur_data{
    u32 time_len;
    u32 pos_len;
  }ur_data;
  extern ur_data UR_DATA;
  extern void UR_GPIO_Configuration(void);
  extern void UR_NVIC_Configuration(void);
  extern void UR_TIM_Configuration(void);
#endif
