#include "stm32f4xx_it.h"
#include "main.h"

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
  SYS_TIME++;
  PT *p = THREADS_HEAD;
  while(p){
    if(p->count >0){
      p->count -=1;
    }
    else{
      p->ready = 1;
      p->count = p->load-1;
    }
    p = p->next;
  }
}
void EXTI0_IRQHandler(void){
  //主时基 等精度测频
  static u8 counter_1 = 0;
  static u8 counter_2 = 0;
  static u8 counter_3 = 0;
  u8 reg_1 = ((*(EXTERN_RAM)(MODE_REG[0]+0x600a000c))&0xff00)>>8;
  u8 reg_3 = ((*(EXTERN_RAM)(MODE_REG[0]+0x600a000c))&0x00ff);
  EXTI_ClearITPendingBit(EXTI_Line0);
  //首先使用高频通道
  if(reg_1 != counter_1){
    FREQ_RESULT = CHINNAL_1_RECV();
    Freq_Input(0,0,FREQ_RESULT);
    FREQ_USED_POS = 0;
  }
  //然后使用低频通道
  else if(reg_3 != counter_3){
    u32 t_b2 = *(EXTERN_RAM)(MODE_REG[0]+0x600a0008);
    t_b2 |= *(EXTERN_RAM)(MODE_REG[0]+0x600a000a)<<16;
    double freq = 0.99999*((double)25000000)/t_b2;
    Freq_Input(1,1,freq);
    FREQ_USED_POS =2;
  }
  counter_1 = reg_1;
  counter_3 = reg_3;
}