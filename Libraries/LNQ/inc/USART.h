/*
 * USART.h
 *
 *  Created on: 2014年4月28日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#ifndef USART_H_
  #define USART_H_
  /* Includes ------------------------------------------------------------------*/
  #include "common.h"
  #include "Sequeue.h"

  /*char队列定义-------------------------------*/
  typedef struct _usart_buffer{
    u8_sequeue TxBuffer;
    u8_sequeue RxBuffer;
    u8 TxFinishedFlag;
  } USART_BUFFER;
  /* Public define ------------------------------------------------------------*/
  #ifdef USART1_USED_
    #define USART1_DMA_STREAM_TX DMA2_Stream7
    #define USART1_DMA_STREAM_TX_IRQ DMA2_Stream7_IRQn
    #define USART1_DMA_CHANNEL_TX DMA_Channel_4
    #define USART1_DMA_STREAM_RX DMA2_Stream2
    #define USART1_DMA_STREAM_RX_IRQ DMA2_Stream2_IRQn
    #define USART1_DMA_CHANNEL_RX DMA_Channel_4
    #define Usart1_Send(p)      Usart_Send(&USART1_BUFFER,(p))
    /*队列长度-------------------------------*/
    #pragma message("使用 USART1")
    #define USART1_RX_BUFFER_LEN (1000)
    #define USART1_TX_BUFFER_LEN (1000)
    extern USART_BUFFER USART1_BUFFER;
    /* Exported functions ------------------------------------------------------- */
    extern void NVIC_Configuration_USART1(void);
    extern void DMA_Configuration_Usart_1(void);
    extern void Usart1_Buffer_Tx_Service();
    extern void Usart1_Buffer_Rx_Service();
    extern void Usart1_Dma_Open(u32 BaudRate);
    extern u8 InitUsart(USART_TypeDef *USARTx, u32 BaudRate);
    extern u8 Usart_Send(USART_BUFFER *p, u8 data);
    extern void Usart_Flush(void);
    extern void USART_Buffer_Init(USART_BUFFER *p, u8 *rx_buffer, u8
      *tx_buffer, u32 rx_len, u32 tx_len);

    /* Interrupt functions ------------------------------------------------------- */
    void USART1_IRQHandler(void);
    void DMA2_Stream7_IRQHandler(void);

    /* Exported Variable ------------------------------------------------------- */
  #endif 
#endif /* USART_H_ */
