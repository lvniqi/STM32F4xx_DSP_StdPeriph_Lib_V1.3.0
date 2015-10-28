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
   * ���� : NVIC_Configuration_USART1
   * ���� : ��ʼ������1�жϣ�������ʹ�ã�
   * ���� : ��
   * ��� : ��
   ***********************************************************************/
  void NVIC_Configuration_USART1(void){
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //ѡ����鷽ʽ0

    /* ʹ�� USART1 �ж� */
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
      //����ʱ��
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
      //��������
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
      //��������
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //A9 TX
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

      GPIO_Init(GPIOA, &GPIO_InitStructure); // ��ʼ�� PA9
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //A10 RX
      GPIO_Init(GPIOA, &GPIO_InitStructure); // ��ʼ�� PA10
      //���־
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
   * ���� : USART1_IRQHandler
   * ���� : ����1�жϷ�������������ʹ�ã�
   * ���� : ��
   * ��� : ��
   ***********************************************************************/
  void USART1_IRQHandler(void){

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
      //����Ĵ�����������
      char a;
      a = USART_ReceiveData(USART1);
      Sequeue_In_Queue(&(USART1_BUFFER.RxBuffer), a);
    }
  }

  /********************************************************************
   * ���� : DMA_Configuration_Usart_1
   * ���� : USART1��DMA����(�Ƽ�ʹ��)
   * ���� : ��
   * ��� : ��
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
    DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(USART1); //Դ��ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = SEQUEUE_DR_BASE(
      (USART1_BUFFER.TxBuffer)); //Ŀ���ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    //������Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize = 1; //���ݳ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //����Ĵ���������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //�������ݿ�� 1�ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    //�ڴ����ݿ�� 1�ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //����ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low; //�����ȼ�

    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

    DMA_Init(USART1_DMA_STREAM_TX, &DMA_InitStructure);

    /*uart1 RX Config*/
    DMA_DeInit(USART1_DMA_STREAM_RX);
    DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(USART1);
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //����ѭ��ģʽ
    DMA_InitStructure.DMA_Memory0BaseAddr = SEQUEUE_DR_BASE(
      (USART1_BUFFER.RxBuffer));
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize = USART1_RX_BUFFER_LEN;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //�е����ȼ�
    DMA_Init(USART1_DMA_STREAM_RX, &DMA_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_DMA_STREAM_TX_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

  }



  /********************************************************************
   * ���� : Usart1_Dma_Open
   * ���� : USART1 DMA ��
   * ���� : ������
   * ��� : ��
   ***********************************************************************/
  void Usart1_Dma_Open(u32 BaudRate){
    InitUsart(USART1, BaudRate);
    DMA_Configuration_Usart_1(); //���DMA����
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    DMA_Cmd(USART1_DMA_STREAM_RX, ENABLE);

    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    DMA_ITConfig(USART1_DMA_STREAM_TX, DMA_IT_TC, ENABLE);
    //DMA_Cmd(USART1_DMA_STREAM_TX, ENABLE);
    USART_Cmd(USART1, ENABLE); //  ����USART1
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
   * ���� : Usart1_Buffer_Rx_Service
   * ���� : USART1 ������ ��ȡ������
   * ���� : ��
   * ��� : ��
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
   * ���� : Usart1_Buffer_Tx_Service
   * ���� : USART1 ������ ���ͷ�����
   * ���� : ��
   * ��� : ��
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
 * ���� : USART_Buffer_Init
 * ���� : USART��������ʼ��
 * ���� : ��
 * ��� : ��
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
 * ���� : Usart_Send
 * ���� : ���ʹ���
 * ���� : USART_BUFFER data
 * ��� : �ɹ�Ϊ1 ���ɹ�Ϊ0
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
 * ���� : Usart_Flush
 * ���� : ͬ���������
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void Usart_Flush(void){
  while(USART1_BUFFER.TxBuffer.len != 0){
    Usart1_Buffer_Tx_Service();
  }
  delay_ns(120);
}
