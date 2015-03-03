#ifndef __EXTI_H
  #define __EXTI_H
  #include "common.h"
  //#include 
  //#define KEY_RAM                (*((volatile unsigned short *) 0x60000006))     //�ⲿ��ַ�Ǵ�0x60000000��ʼ��
  #define KEY_RAM                (*((volatile unsigned short *) 0x6000000c))   
    //�ⲿ��ַ�Ǵ�0x60000000��ʼ��

  typedef struct _key_type{
    u16 keysign;
    u16 count;
    u8 keycode;
  } key_type;
  extern volatile key_type MAIN_KEY;
  extern void EXTI_init(void);
  extern void EXTI1_IRQHandler(void);
  #define Wait_For_Any_Key() while (not MAIN_KEY.keysign);MAIN_KEY.keysign = 0
#endif
