/*
 * main.cpp
 *
 *  Created on: 2014-4-28
 *      Author: lvniqi
 */
#include "USART.h"
#ifdef USART1_USED_
  /* Public Variable ------------------------------------------------------- */
  u8 _USART1_RX_BUFFER[USART1_RX_BUFFER_LEN];
  u8 _USART1_TX_BUFFER[USART1_TX_BUFFER_LEN];
  USART_BUFFER USART1_BUFFER;
  /* public functions ------------------------------------------------------- */
  /********************************************************************
   * 名称 : NVIC_Configuration_USART1
   * 功能 : 初始化串口1中断（不建议使用）
   * 输入 : 无
   * 输出 : 无
   ***********************************************************************/
  void NVIC_Configuration_USART1(void){
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //选择分组方式0

    /* 使能 USART1 中断 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  }
  u8 InitUsart(USART_TypeDef* USARTx, u32 BaudRate){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    USART_DeInit(USARTx);
    if (USART1 == USARTx){
      //设置时钟
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
      //引脚重用
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
      //设置引脚
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //A9 TX
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

      GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化 PA9
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //A10 RX
      GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化 PA10
      //清标志
      USART_ClearFlag(USART1, USART_FLAG_TC);
      USART_InitStructure.USART_BaudRate = BaudRate;
      USART_InitStructure.USART_WordLength = USART_WordLength_8b;
      USART_InitStructure.USART_StopBits = USART_StopBits_1;
      USART_InitStructure.USART_Parity = USART_Parity_No;
      USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
      USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
      USART_Init(USART1, &USART_InitStructure);
      return 1;
    }
    return 0;
  }
  /********************************************************************
   * 名称 : USART1_IRQHandler
   * 功能 : 串口1中断服务函数（不建议使用）
   * 输入 : 无
   * 输出 : 无
   ***********************************************************************/
  void USART1_IRQHandler(void){

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
      //如果寄存器中有数据
      char a;
      a = USART_ReceiveData(USART1);
      Sequeue_In_Queue(&(USART1_BUFFER.RxBuffer), a);
    }
  }

  /********************************************************************
   * 名称 : DMA_Configuration_Usart_1
   * 功能 : USART1的DMA设置(推荐使用)
   * 输入 : 无
   * 输出 : 无
   ***********************************************************************/
  void DMA_Configuration_Usart_1(void){
    DMA_InitTypeDef DMA_InitStructure;
    /* DMA clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    /*Buffer init*/
    USART_Buffer_Init(&USART1_BUFFER, _USART1_RX_BUFFER, _USART1_TX_BUFFER,
                      USART1_RX_BUFFER_LEN, USART1_TX_BUFFER_LEN);
    /*uart1 TX Config*/
    DMA_DeInit(USART1_DMA_STREAM_TX);
    DMA_InitStructure.DMA_Channel = USART1_DMA_CHANNEL_TX;
    DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(USART1); //源地址
    DMA_InitStructure.DMA_Memory0BaseAddr = SEQUEUE_DR_BASE(
      (USART1_BUFFER.TxBuffer)); //目标地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    //外设作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = 1; //数据长度
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //外设寄存器不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //外设数据宽度 1字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    //内存数据宽度 1字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //正常模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low; //高优先级

    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

    DMA_Init(USART1_DMA_STREAM_TX, &DMA_InitStructure);

    /*uart1 RX Config*/
    DMA_DeInit(USART1_DMA_STREAM_RX);
    DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(USART1);
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //环形循环模式
    DMA_InitStructure.DMA_Memory0BaseAddr = SEQUEUE_DR_BASE(
      (USART1_BUFFER.RxBuffer));
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = USART1_RX_BUFFER_LEN;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //中等优先级
    DMA_Init(USART1_DMA_STREAM_RX, &DMA_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_DMA_STREAM_TX_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

  }



  /********************************************************************
   * 名称 : Usart1_Dma_Open
   * 功能 : USART1 DMA 打开
   * 输入 : 波特率
   * 输出 : 无
   ***********************************************************************/
  void Usart1_Dma_Open(u32 BaudRate){
    InitUsart(USART1, BaudRate);
    DMA_Configuration_Usart_1(); //完成DMA设置
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    DMA_Cmd(USART1_DMA_STREAM_RX, ENABLE);

    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    DMA_ITConfig(USART1_DMA_STREAM_TX, DMA_IT_TC, ENABLE);
    //DMA_Cmd(USART1_DMA_STREAM_TX, ENABLE);
    USART_Cmd(USART1, ENABLE); //  启用USART1
    #ifdef USART1_WELLCOME_
      printf("\033[34;1m");
      printf("wellcome to our board!\r\n");
      printf("+-------------------+\r\n");
      printf("|*******************|\r\n");
      printf("|*******************|\r\n");
      printf("|*******************|\r\n");
      printf("|*******************|\r\n");
      printf("|*******************|\r\n");
      printf("|*******************|\r\n");
      printf("+-------------------+\r\n");
      printf("\033[0m");
    #endif
    PT_DEBUG(3,"USART1 Now is up!\r\n");
  }

  /********************************************************************
   * 名称 : Usart1_Buffer_Rx_Service
   * 功能 : USART1 缓冲区 读取服务函数
   * 输入 : 无
   * 输出 : 无
   ***********************************************************************/
  void Usart1_Buffer_Rx_Service(){
    u16 len;
    len = (USART1_BUFFER.RxBuffer).len_max - (u32)USART1_DMA_STREAM_RX->NDTR;
    if (len != 0 and len != (USART1_BUFFER.RxBuffer).len_max){
      (USART1_BUFFER.RxBuffer).rear = len;
      if ((USART1_BUFFER.RxBuffer).rear == (USART1_BUFFER.RxBuffer).front){
        if ((USART1_BUFFER.RxBuffer).len != 0){
          (USART1_BUFFER.RxBuffer).len = (USART1_BUFFER.RxBuffer).len_max;
        }
      }
      else{

        (USART1_BUFFER.RxBuffer).len = ((USART1_BUFFER.RxBuffer).rear + 
         (USART1_BUFFER.RxBuffer).len_max - (USART1_BUFFER.RxBuffer).front) % (
         (USART1_BUFFER.RxBuffer).len_max);
      }
    }
    else{
      if ((USART1_BUFFER.RxBuffer).rear != (len % (USART1_BUFFER.RxBuffer)
          .len_max)){
        (USART1_BUFFER.RxBuffer).len = (USART1_BUFFER.RxBuffer).len_max - 
         (USART1_BUFFER.RxBuffer).front;
        (USART1_BUFFER.RxBuffer).rear = 0;
      }
    }
  }

  /********************************************************************
   * 名称 : Usart1_Buffer_Tx_Service
   * 功能 : USART1 缓冲区 发送服务函数
   * 输入 : 无
   * 输出 : 无
   ***********************************************************************/
  void Usart1_Buffer_Tx_Service(){
    static u8 un_send_time = 0;
    if ((Sequeue_Getlen(&(USART1_BUFFER.TxBuffer)) < Sequeue_GetlenMax(&
        (USART1_BUFFER.TxBuffer)) / 3)and(un_send_time < 3)){
      un_send_time++;
      return ;
    }
    else{
      un_send_time = 0;
    }
    if ((USART1_BUFFER.TxFinishedFlag)){
      DMA_Cmd(USART1_DMA_STREAM_TX, DISABLE);
      DMA_ITConfig(USART1_DMA_STREAM_TX, DMA_IT_TC, DISABLE);
      (USART1_BUFFER.TxBuffer).len -= (USART1_BUFFER.TxBuffer).len_should_cut;
      (USART1_BUFFER.TxBuffer).len_should_cut = 0;
      if ((USART1_BUFFER.TxBuffer).len){
        if ((USART1_BUFFER.TxBuffer).rear >= (USART1_BUFFER.TxBuffer).front){
          USART1_DMA_STREAM_TX->NDTR = (USART1_BUFFER.TxBuffer).len;
          USART1_DMA_STREAM_TX->M0AR = (uint32_t)((USART1_BUFFER.TxBuffer)
                                        .dataspace + (USART1_BUFFER.TxBuffer)
                                        .front);
          (USART1_BUFFER.TxBuffer).len_should_cut = (USART1_BUFFER.TxBuffer)
           .len;
          (USART1_BUFFER.TxBuffer).front = (USART1_BUFFER.TxBuffer).rear;
        }
        else{
          USART1_DMA_STREAM_TX->NDTR = (USART1_BUFFER.TxBuffer).len_max - 
                                        (USART1_BUFFER.TxBuffer).front;
          USART1_DMA_STREAM_TX->M0AR = (uint32_t)((USART1_BUFFER.TxBuffer)
                                        .dataspace + (USART1_BUFFER.TxBuffer)
                                        .front);
          (USART1_BUFFER.TxBuffer).front = 0;
          (USART1_BUFFER.TxBuffer).len_should_cut = (USART1_BUFFER.TxBuffer)
           .len - (USART1_BUFFER.TxBuffer).rear;
        }
        (USART1_BUFFER.TxFinishedFlag) = False;
        DMA_ITConfig(USART1_DMA_STREAM_TX, DMA_IT_TC, ENABLE);
        DMA_Cmd(USART1_DMA_STREAM_TX, ENABLE);
      }
    }
  }
#endif 
/********************************************************************
 * 名称 : USART_Buffer_Init
 * 功能 : USART缓冲区初始化
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void USART_Buffer_Init(USART_BUFFER* p, u8* rx_buffer, u8* tx_buffer, u32
                       rx_len, u32 tx_len){
  u8_sequeue* temp = &(p->RxBuffer);
  temp->dataspace = rx_buffer;
  Sequeue_Set_Null(temp, rx_len);
  temp->len_should_cut = 0;
  temp = &(p->TxBuffer);
  temp->dataspace = tx_buffer;
  temp->len_should_cut = 0;
  Sequeue_Set_Null(temp, tx_len);
  p->TxFinishedFlag = 1;
}


/********************************************************************
 * 名称 : Usart_Send
 * 功能 : 发送串口
 * 输入 : USART_BUFFER data
 * 输出 : 成功为1 不成功为0
 ***********************************************************************/
u8 Usart_Send(USART_BUFFER* p, u8 data){
  if ((Sequeue_Getlen(&(p->TxBuffer)) >= Sequeue_GetlenMax(&(p->TxBuffer)) / 2)
      and(p->TxFinishedFlag)){
    #ifdef USART1_USED_
      Usart1_Buffer_Tx_Service();
    #endif 
  }
  if (not Sequeue_Full(&(p->TxBuffer))){
    Sequeue_In_Queue(&(p->TxBuffer), data);
    return 1;
  }
  return 0;
}

void DMA2_Stream7_IRQHandler(void){
  if (DMA_GetFlagStatus(DMA2_Stream7, DMA_IT_TCIF7) != RESET){
    DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
    (USART1_BUFFER.TxFinishedFlag) = True;
  }
}
/********************************************************************
 * 名称 : Usart_Flush
 * 功能 : 同步输出缓冲
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void Usart_Flush(void){
  while(USART1_BUFFER.TxBuffer.len != 0){
    Usart1_Buffer_Tx_Service();
  }
  delay_ns(120);
}
