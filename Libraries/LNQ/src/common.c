/*
 * COMMON.c
 *
 *  Created on: 2014年5月4日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#include "common.h"
//RCC时钟数据
RCC_ClocksTypeDef RCC_Clocks;
PT *THREADS_HEAD = NULL;
u32  SYS_TIME = 0;
/********************************************************************
 * 名称 : assert_failed
 * 功能 : 帮助提示
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
#ifdef USE_FULL_ASSERT

  /**
   * @brief  Reports the name of the source file and the source line number
   *         where the assert_param error has occurred.
   * @param  file: pointer to the source file name
   * @param  line: assert_param error line source number
   * @retval None
   */
  void assert_failed(uint8_t* file, uint32_t line){
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    /* Infinite loop */
    while (1){}
  }
#endif 
/********************************************************************
 * 名称 : PUTCHAR_PROTOTYPE
 * 功能 : 重置printf中的putchar函数（非用户使用）
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
#ifdef DMA_USED_
  int fputc(int ch, FILE* f){
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    #ifdef USART1_USED_
      Usart_Send(&USART1_BUFFER, (u8)ch);
    #endif 
    #ifdef USART3_USED_
      Usart_Send(&USART3_BUFFER, (u8)ch);
    #endif 
    return ch;
  }
#else 
  int fputc(int ch, FILE* f){
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    #ifdef USART1_USED_
      Usart1_Send(ch);
      //USART_SendData(USART1, (uint8_t)ch);
      //Loop until the end of transmission
      //while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
    #endif 
    return ch;
  }
#endif 
u16 Num_Len(long num){
  if(num<0){
    num = -num;
  }
  int i = 0;
  while (num){
    i++;
    num /= 10;
  }
  return i;
}

void delay_ms(volatile u16 time){
  volatile u32 i = 0;
  while (time--){
    i = 9000; //自己定义
    while (i--)
      ;
  }
}
void PT_SERVICE(){
  PT* p = THREADS_HEAD;
  while(p != NULL){
    p->fun(p);
    p = p->next;
  }
}
void PT_ADD_THREAD(PT *in){
  in->FUNSTR = NULL;
  in->next = THREADS_HEAD;
  THREADS_HEAD = in;
}
void PT_REMOVE_THREAD(const char* FUNSTR){
  if(THREADS_HEAD){
    if(THREADS_HEAD->FUNSTR && strcmp(FUNSTR,THREADS_HEAD->FUNSTR) == 0){
      THREADS_HEAD = THREADS_HEAD->next;
      return ;
    }
    else{
      PT* p = THREADS_HEAD;
      while(p->next != NULL){
        if(p->next->FUNSTR && strcmp(FUNSTR,p->next->FUNSTR) == 0){
          p->next = p->next->next;
          return ;
        }
      }
      p = p->next;
    }
  }
}
PT* PT_GET_THREAD(const char* FUNSTR){
  if(THREADS_HEAD){
    if(THREADS_HEAD->FUNSTR && strcmp(FUNSTR,THREADS_HEAD->FUNSTR) == 0){
      return THREADS_HEAD;
    }
    else{
      PT* p = THREADS_HEAD;
      while(p->next != NULL){
        if(p->next->FUNSTR && strcmp(FUNSTR,p->next->FUNSTR) == 0){
          return p;
        }
      }
      p = p->next;
    }
  }
  return NULL;
}
#include "exti.h"
static void(*KEY_FUNC[15])() = {0,0,};
void Set_Key_Func(void(*func)(),char n){
  if(n<16&&n>=0){
    KEY_FUNC[n] = func;
  }
}
PT_THREAD(KEY_SERVICE(PT *pt)){
  PT_BEGIN(pt);
  while(1){
    PT_WAIT_UNTIL(pt,pt->ready&&MAIN_KEY.keysign);
    MAIN_KEY.keysign = 0;
    pt->ready = 0;
    if(KEY_FUNC[MAIN_KEY.keycode]){
      KEY_FUNC[MAIN_KEY.keycode]();
    }
  }
  PT_END(pt);
}