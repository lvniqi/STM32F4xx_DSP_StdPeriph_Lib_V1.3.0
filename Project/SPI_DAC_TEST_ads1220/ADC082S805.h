/*
 * ADC082.h
 *
 *  Created on: 2014Äê5ÔÂ4ÈÕ
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#ifndef ADC082_H_
  #define ADC082_H_
  #include "common.h"
  #include "spi.h"
  extern void InitAdc082Spi();
  extern void Set_Adc082(int x,u16 data);
#endif /* ADC082_H_ */
