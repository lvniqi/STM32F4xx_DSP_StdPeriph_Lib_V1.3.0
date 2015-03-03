#ifndef __GPIO_H
  #define __GPIO_H	
  #include "common.h"

  //λ������,ʵ��51���Ƶ�GPIO���ƹ���
  //����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
  /*ϵͳ����*/
  /*���棡���û�ʹ��*/
  #define _BIT_BAND(add, channel) ((add & 0xF0000000)+0x2000000+((add &0xFFFFF)<<5)+(channel<<2))
  /*���棡���û�ʹ��*/
  #define _MEM_Add(add)  *((volatile unsigned long  *)(add)) 
  /*���棡���û�ʹ��*/
  #define _BIT_Add(add, channel)   _MEM_Add(_BIT_BAND(add, channel)) 


  //IO�ڵ�ַӳ��
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

  //IO�ڲ���,ֻ�Ե�һ��IO��!
  //ȷ��n��ֵС��16!
  #define PAout(n)   _BIT_Add(GPIOA_OUT_ADDRESS,n)  //��� 
  #define PAin(n)    _BIT_Add(GPIOA_IN_ADDRESS,n)  //���� 

  #define PBout(n)   _BIT_Add(GPIOB_OUT_ADDRESS,n)  //��� 
  #define PBin(n)    _BIT_Add(GPIOB_IN_ADDRESS,n)  //���� 

  #define PCout(n)   _BIT_Add(GPIOC_OUT_ADDRESS,n)  //��� 
  #define PCin(n)    _BIT_Add(GPIOC_IN_ADDRESS,n)  //���� 

  #define PDout(n)   _BIT_Add(GPIOD_OUT_ADDRESS,n)  //��� 
  #define PDin(n)    _BIT_Add(GPIOD_IN_ADDRESS,n)  //���� 

  #define PEout(n)   _BIT_Add(GPIOE_OUT_ADDRESS,n)  //��� 
  #define PEin(n)    _BIT_Add(GPIOE_IN_ADDRESS,n)  //����

  #define PFout(n)   _BIT_Add(GPIOF_OUT_ADDRESS,n)  //��� 
  #define PFin(n)    _BIT_Add(GPIOF_IN_ADDRESS,n)  //����

  #define PGout(n)   _BIT_Add(GPIOG_OUT_ADDRESS,n)  //��� 
  #define PGin(n)    _BIT_Add(GPIOG_IN_ADDRESS,n)  //����

#endif
