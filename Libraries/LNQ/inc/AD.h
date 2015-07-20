/*
 * AD.h
 *
 *  Created on: 2014��6��9��
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
   * ���� : ADC1_Soft_Init
   * ���� : ADC ���� ����
   * ���� : ADC_Channel ADCͨ��
   * ��� : ��
   ***********************************************************************/
  extern u8 ADC1_Soft_Init(ADC_TypeDef* adcx,uint32_t ADC_Channelx);
  /********************************************************************
   * ���� : ADC1_Init
   * ���� : ADC1 ��ʼ��
   * ���� : ��
   * ��� : ��
   ***********************************************************************/
  extern void ADC1_Init(uint32_t ADC_Channelx,u32 ADC_ExternalTrigConvEdge);
  /********************************************************************
   * ���� : Ad_Get_Service
   * ���� : ADC �õ�������
   * ���� : ��
   * ��� : ��
   ***********************************************************************/
  extern void Ad_Get_Service();
  /*ƹ���� AD*/
  extern pingpang pingpang_ad;
  /*ADC��ɱ�־*/
  extern u8 AdcTxFinishedFlag;
#endif /* AD_H_ */
