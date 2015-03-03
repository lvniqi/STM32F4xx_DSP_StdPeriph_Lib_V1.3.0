/*
 * DA.h
 *
 *  Created on: 2014年7月27日
 *      Author: lvniqi
 */

#ifndef DA_H_
  #define DA_H_
  #include "common.h"
  #include "Sequeue.h"
  #include "PINGPANG.h"
  #include "stm32f4xx_dac.h"
  /*DA地址*/
  /* DHR registers offsets */
  #define DHR12R1_OFFSET             ((uint32_t)0x00000008)
  #define DHR12R2_OFFSET             ((uint32_t)0x00000014)
  #define DHR12RD_OFFSET             ((uint32_t)0x00000020)
  #define DAC1_DR_BASE ((u32)DAC_BASE+DHR12R1_OFFSET+DAC_Align_12b_R)
  #define DAC2_DR_BASE ((u32)DAC_BASE+DHR12R2_OFFSET+DAC_Align_12b_R)
  #define DAC1_DMA_STREAM DMA1_Stream5
  #define DAC1_DMA_CHANNEL DMA_Channel_7
  /*DAC_BUFFER 定义*/
  typedef struct _dac_buffer{
    u16_sequeue TxBuffer;
    u8 TxFinishedFlag;
  } DAC_BUFFER;
  /*乒乓组 da*/
  extern pingpang pingpang_da;
  /*da 发送完成标志*/
  extern u8 DacTxFinishedFlag;
    /********************************************************************
   * 名称 : Dac_Soft_Init
   * 功能 : DAC软件触发 初始化
   * 输入 : DAC通道 触发方式
   * 输出 : 无 
   ***********************************************************************/
  extern u8 Dac_Soft_Init(uint32_t DAC_Channelx);
    /********************************************************************
   * 名称 : Dac_Init
   * 功能 : DAC初始化
   * 输入 : DAC通道 触发方式
   * 输出 : 无 
   ***********************************************************************/
  extern u8 Dac_Init(uint32_t DAC_Channel, uint32_t DAC_TRIGGER);
  /********************************************************************
 * 名称 :Dac_Send_Service
 * 功能 : DAC发送服务函数
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
  extern void Dac_Send_Service();
  /********************************************************************
 * 名称 : DAC_Soft_Set
 * 功能 : 设置12位DAC
 * 输入 : DAC通道 12位数
 * 输出 : 无 
 ***********************************************************************/
  extern void DAC_Soft_Set(uint32_t DAC_Channelx,u16 i);
#endif /* DA_H_ */
