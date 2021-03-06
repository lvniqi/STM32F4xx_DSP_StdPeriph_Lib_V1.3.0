#ifndef __GPIO_H
  #define __GPIO_H	
  #include "common.h"

  //位带操作,实现51类似的GPIO控制功能
  //具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
  /*系统定义*/
  /*警告！非用户使用*/
  #define _BIT_BAND(add, channel) ((add & 0xF0000000)+0x2000000+((add &0xFFFFF)<<5)+(channel<<2))
  /*警告！非用户使用*/
  #define _MEM_Add(add)  *((volatile unsigned long  *)(add)) 
  #define _MEM_Add_Add(add)  ((volatile unsigned long  *)(add)) 
  /*警告！非用户使用*/
  #define _BIT_Add(add, channel)   _MEM_Add(_BIT_BAND(add, channel)) 
  #define _BIT_Add_Add(add, channel)   _MEM_Add_Add(_BIT_BAND(add, channel)) 


  //IO口地址映射
  #define GPIOA_OUT_ADDRESS    (GPIOA_BASE+20) //0x40020014
  #define GPIOB_OUT_ADDRESS    (GPIOB_BASE+20) //0x40010C14
  #define GPIOC_OUT_ADDRESS    (GPIOC_BASE+20) //0x40011014 
  #define GPIOD_OUT_ADDRESS    (GPIOD_BASE+20) //0x40011414 
  #define GPIOE_OUT_ADDRESS    (GPIOE_BASE+20) //0x40011814 
  #define GPIOF_OUT_ADDRESS    (GPIOF_BASE+20) //0x40011A14    
  #define GPIOG_OUT_ADDRESS    (GPIOG_BASE+20) //0x40011E14    
  #define GPIOH_OUT_ADDRESS    (GPIOH_BASE+20) //0x40021C14
  #define GPIOI_OUT_ADDRESS    (GPIOI_BASE+20) //0x40022014

  #define GPIOA_IN_ADDRESS    (GPIOA_BASE+16) //0x40010810 
  #define GPIOB_IN_ADDRESS    (GPIOB_BASE+16) //0x40010C10 
  #define GPIOC_IN_ADDRESS    (GPIOC_BASE+16) //0x40011010 
  #define GPIOD_IN_ADDRESS    (GPIOD_BASE+16) //0x40011410 
  #define GPIOE_IN_ADDRESS    (GPIOE_BASE+16) //0x40011810 
  #define GPIOF_IN_ADDRESS    (GPIOF_BASE+16) //0x40011A10 
  #define GPIOG_IN_ADDRESS    (GPIOG_BASE+16) //0x40011E10 
  #define GPIOH_IDR_Addr      (GPIOF_BASE+16) //0x40021C10
  #define GPIOI_IDR_Addr      (GPIOG_BASE+16) //0x40022010

  //IO口操作,只对单一的IO口!
  //确保n的值小于16!
  #define PAout_ADDR(n)   _BIT_Add_Add(GPIOA_OUT_ADDRESS,n)  //输出 
  #define PAin_ADDR(n)    _BIT_Add_Add(GPIOA_IN_ADDRESS,n)  //输入 

  #define PBout_ADDR(n)   _BIT_Add_Add(GPIOB_OUT_ADDRESS,n)  //输出 
  #define PBin_ADDR(n)    _BIT_Add_Add(GPIOB_IN_ADDRESS,n)  //输入 

  #define PCout_ADDR(n)   _BIT_Add_Add(GPIOC_OUT_ADDRESS,n)  //输出 
  #define PCin_ADDR(n)    _BIT_Add_Add(GPIOC_IN_ADDRESS,n)  //输入 

  #define PDout_ADDR(n)   _BIT_Add_Add(GPIOD_OUT_ADDRESS,n)  //输出 
  #define PDin_ADDR(n)    _BIT_Add_Add(GPIOD_IN_ADDRESS,n)  //输入 

  #define PEout_ADDR(n)   _BIT_Add_Add(GPIOE_OUT_ADDRESS,n)  //输出 
  #define PEin_ADDR(n)    _BIT_Add_Add(GPIOE_IN_ADDRESS,n)  //输入

  #define PFout_ADDR(n)   _BIT_Add_Add(GPIOF_OUT_ADDRESS,n)  //输出 
  #define PFin_ADDR(n)    _BIT_Add_Add(GPIOF_IN_ADDRESS,n)  //输入

  #define PGout_ADDR(n)   _BIT_Add_Add(GPIOG_OUT_ADDRESS,n)  //输出 
  #define PGin_ADDR(n)    _BIT_Add_Add(GPIOG_IN_ADDRESS,n)  //输入

  #define PAout(n)   _BIT_Add(GPIOA_OUT_ADDRESS,n)  //输出 
  #define PAin(n)    _BIT_Add(GPIOA_IN_ADDRESS,n)  //输入 

  #define PBout(n)   _BIT_Add(GPIOB_OUT_ADDRESS,n)  //输出 
  #define PBin(n)    _BIT_Add(GPIOB_IN_ADDRESS,n)  //输入 

  #define PCout(n)   _BIT_Add(GPIOC_OUT_ADDRESS,n)  //输出 
  #define PCin(n)    _BIT_Add(GPIOC_IN_ADDRESS,n)  //输入 

  #define PDout(n)   _BIT_Add(GPIOD_OUT_ADDRESS,n)  //输出 
  #define PDin(n)    _BIT_Add(GPIOD_IN_ADDRESS,n)  //输入 

  #define PEout(n)   _BIT_Add(GPIOE_OUT_ADDRESS,n)  //输出 
  #define PEin(n)    _BIT_Add(GPIOE_IN_ADDRESS,n)  //输入

  #define PFout(n)   _BIT_Add(GPIOF_OUT_ADDRESS,n)  //输出 
  #define PFin(n)    _BIT_Add(GPIOF_IN_ADDRESS,n)  //输入

  #define PGout(n)   _BIT_Add(GPIOG_OUT_ADDRESS,n)  //输出 
  #define PGin(n)    _BIT_Add(GPIOG_IN_ADDRESS,n)  //输入

#endif
