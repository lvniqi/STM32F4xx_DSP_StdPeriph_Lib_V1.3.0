/*
 * DA.h
 *
 *  Created on: 2014��7��27��
 *      Author: lvniqi
 */

#ifndef DA_H_
  #define DA_H_
  #include "common.h"
  #include "Sequeue.h"
  #include "PINGPANG.h"
  #include "stm32f4xx_dac.h"
  /*DA��ַ*/
  /* DHR registers offsets */
  #define DHR12R1_OFFSET             ((uint32_t)0x00000008)
  #define DHR12R2_OFFSET             ((uint32_t)0x00000014)
  #define DHR12RD_OFFSET             ((uint32_t)0x00000020)
  #define DAC1_DR_BASE ((u32)DAC_BASE+DHR12R1_OFFSET+DAC_Align_12b_R)
  #define DAC2_DR_BASE ((u32)DAC_BASE+DHR12R2_OFFSET+DAC_Align_12b_R)
  #define DAC1_DMA_STREAM DMA1_Stream5
  #define DAC1_DMA_CHANNEL DMA_Channel_7
  /*DAC_BUFFER ����*/
  typedef struct _dac_buffer{
    u16_sequeue TxBuffer;
    u8 TxFinishedFlag;
  } DAC_BUFFER;
  /*ƹ���� da*/
  extern pingpang pingpang_da;
  /*da ������ɱ�־*/
  extern u8 DacTxFinishedFlag;
    /********************************************************************
   * ���� : Dac_Soft_Init
   * ���� : DAC������� ��ʼ��
   * ���� : DACͨ�� ������ʽ
   * ��� : �� 
   ***********************************************************************/
  extern u8 Dac_Soft_Init(uint32_t DAC_Channelx);
    /********************************************************************
   * ���� : Dac_Init
   * ���� : DAC��ʼ��
   * ���� : DACͨ�� ������ʽ
   * ��� : �� 
   ***********************************************************************/
  extern u8 Dac_Init(uint32_t DAC_Channel, uint32_t DAC_TRIGGER);
  /********************************************************************
 * ���� :Dac_Send_Service
 * ���� : DAC���ͷ�����
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
  extern void Dac_Send_Service();
  /********************************************************************
 * ���� : DAC_Soft_Set
 * ���� : ����12λDAC
 * ���� : DACͨ�� 12λ��
 * ��� : �� 
 ***********************************************************************/
  extern void DAC_Soft_Set(uint32_t DAC_Channelx,u16 i);
#endif /* DA_H_ */
