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
#include "math.h"
#define SHIFT_FITHER_LEN 13
extern double Dds_SetFreq(u32 div);
extern int getMid(int_sequeue* s);
extern PT_THREAD(PRINT_TEST(PT *pt));
extern void Init(void);

extern void RES_EN(SelectItem* p);
extern void RES_UN(SelectItem* p);
extern void CAP_EN(SelectItem* p);
extern void CAP_UN(SelectItem* p);
extern void RES_FIND_EN(SelectItem* p);
extern void RES_FIND_UN(SelectItem* p);
extern void CAP_FIND_EN(SelectItem* p);
extern void CAP_FIND_UN(SelectItem* p);
extern void RES_CAP_Select(bool isRES);

extern void RES_LEVEL_1(SelectItem* p);
extern void RES_LEVEL_2(SelectItem* p);
extern void RES_LEVEL_3(SelectItem* p);

extern float RES_ZOOM_500(float t_r);
extern float RES_ZOOM_5K(float t_r);
extern float RES_ZOOM_50K(float t_r);

extern float Get_RES(double value);
extern float Get_RES_AUTO(double value);

extern void SetRes(NumBar* p,int value);

extern void CAP_LEVEL_1(SelectItem* p);
extern void CAP_LEVEL_2(SelectItem* p);
extern void CAP_LEVEL_3(SelectItem* p);

extern float CAP_ZOOM_500(double value);
extern float CAP_ZOOM_5(double value);
extern float CAP_ZOOM_50(double value);

extern void Select_CAP_SAMPLE(SelectItem* p);
extern void UnSelect_CAP_SAMPLE(SelectItem* p);
extern void Select_CAP_RANGE(SelectItem* p);
extern void UnSelect_CAP_RANGE(SelectItem* p);
extern void Select_RES_SAMPLE(SelectItem* p);
extern void UnSelect_RES_SAMPLE(SelectItem* p);
extern void Select_RES_RANGE(SelectItem* p);
extern void UnSelect_RES_RANGE(SelectItem* p);

extern float Get_CAP(double value);
extern float Get_CAP_AUTO(double value);

extern void SetCap(NumBar* p,int value);

extern void MenuInit(void);
//0~50
#define RES_BASE_0 507.90
#define RES_OFFSET_0 0.0672
//50~500
#define RES_BASE_1 507.04
#define RES_OFFSET_1 0.0672
extern NumBar RES;
extern NumBar CAP;
extern NumBar RES_SAMPLE,RES_RANGE;
extern NumBar CAP_SAMPLE,CAP_RANGE;
extern ADS1220 ADC_1;
extern SelectBar main_menu;
extern SelectBar sub_menu;
extern int_sequeue ADC_1_SEQ;
#endif /* __MAIN_H */
