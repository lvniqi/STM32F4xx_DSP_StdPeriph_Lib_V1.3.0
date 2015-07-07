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
#include "spi.h"
#include "ADS1220.h"
#include "NumBar.h"
#include "malloc.h"
#include "SelectItem.h"
#include "SelectBar.h"
#define SHIFT_FITHER_LEN 13
extern double Dds_SetFreq(u32 div);
extern int getMid(int_sequeue* s);
extern PT_THREAD(PRINT_TEST(PT *pt));
extern void Init(void);

extern void CS1_TEST(SelectItem* p);
extern void CS2_TEST(SelectItem* p);
#define REG 507.7
extern NumBar RES;
extern NumBar CAP;
extern ADS1220 DAC_1;
extern int_sequeue DAC_1_SEQ;
#endif /* __MAIN_H */
