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
  EXTI_ClearITPendingBit(EXTI_Line0);
  {
    //主时基 等精度测频
    static u8 counter_1 = 0;
    static u8 counter_2 = 0;
    static u8 counter_3 = 0;
    u8 reg_1 = FREQ_READ_REGISTER(1);
    u8 reg_2 = FREQ_READ_REGISTER(2);
    u8 reg_3 = FREQ_READ_REGISTER(3);
    //首先更新高频通道
    if(reg_1 != counter_1){
        Freq_Input(0,0,FREQ_CHINNAL_1_RECV());
    }
    //然后更新次高频通道
    if(reg_2 != counter_2){
        Freq_Input(1,0,FREQ_CHINNAL_2_RECV());
    }
    //然后更新低频通道
    if(reg_3 != counter_3){
        Freq_Input(1,1,FREQ_CHINNAL_3_RECV());
    }
    counter_1 = reg_1;
    counter_2 = reg_2;
    counter_3 = reg_3;
  }
  {
    //主时基 等精度测频
    static u8 counter_1 = 0;
    static u8 counter_2 = 0;
    static u8 counter_3 = 0;
    u8 reg_1 = IMPLUSE_READ_REGISTER(0);
    u8 reg_3 = IMPLUSE_READ_REGISTER(3);
    //首先更新高频通道
    if(reg_1 != counter_1){
        Impluse_Input(0,0,IMPLUSE_CHINNAL_1_RECV());
    }
    //然后更新低频通道
    if(reg_3 != counter_3){
        Impluse_Input(1,1,IMPLUSE_CHINNAL_3_RECV());
    }
    counter_1 = reg_1;
    counter_3 = reg_3;
  }
  //主时基 等精度测频
  Span_Input(SPAN_RECV());
}