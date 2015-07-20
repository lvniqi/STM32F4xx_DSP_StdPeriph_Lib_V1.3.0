/*
 * AD.h
 *
 *  Created on: 2014年6月9日
 *      Author: lvniqi
 */

#ifndef AD_H_
  #define AD_H_
  #include "common.h"
  #include "Sequeue.h"
  #include "pingpang.h"
  #define ADC1_DMA_STREAM DMA2_Stream4
  #define ADC1_DMA_CHANNEL DMA_Channel_0
  /********************************************************************
   * 名称 : ADC1_Soft_Init
   * 功能 : ADC 软触发 启动
   * 输入 : ADC_Channel ADC通道
   * 输出 : 无
   ***********************************************************************/
  extern u8 ADC1_Soft_Init(ADC_TypeDef* adcx,uint32_t ADC_Channelx);
  /********************************************************************
   * 名称 : ADC1_Init
   * 功能 : ADC1 初始化
   * 输入 : 无
   * 输出 : 无
   ***********************************************************************/
  extern void ADC1_Init(uint32_t ADC_Channelx,u32 ADC_ExternalTrigConvEdge);
  /********************************************************************
   * 名称 : Ad_Get_Service
   * 功能 : ADC 得到服务函数
   * 输入 : 无
   * 输出 : 无
   ***********************************************************************/
  extern void Ad_Get_Service();
  /*乒乓组 AD*/
  extern pingpang pingpang_ad;
  /*ADC完成标志*/
  extern u8 AdcTxFinishedFlag;
#endif /* AD_H_ */
