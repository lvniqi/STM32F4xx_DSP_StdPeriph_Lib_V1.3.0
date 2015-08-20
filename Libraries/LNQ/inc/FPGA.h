/*
 * FPGA.h
 *
 *  Created on: 2014Äê8ÔÂ1ÈÕ
 *      Author: lvniqi
 */

#ifndef FPGA_H_
  #define FPGA_H_
  #include "common.h"
  #define FPGA_FREQ_L              (*((volatile unsigned short *) 0x60030028))
    //0x0002
  #define FPGA_FREQ_H              (*((volatile unsigned short *) 0x6003004A))
    //0x0002
  #define FPGA_AD_READ()              (*(((__IO uint16_t*) 0x6003006C))) 
    //0x0002
  #define FPGA_AD_START()          {(*((volatile unsigned short *) 0x6003008E))= 1;} //0x0002
  #define FPGA_AD_END()             {(*((volatile unsigned short *) 0x6003008E))= 0;} //0x0002
  extern void FPGA_EXTI4_init(void);
#endif /* FPGA_H_ */
