#ifndef __EXTI_H
#define __EXIT_H
#include "sys.h"

#define KEY_RAM                (*((volatile unsigned short *) 0x60000006))     //�ⲿ��ַ�Ǵ�0x60000000��ʼ��

void EXTI_init(void);

#endif
