/*
 * AD.c
 *
 *  Created on: 2014��6��9��
 *      Author: lvniqi
 */
#include "AD.h"

/*ƹ���� AD*/
pingpang pingpang_ad;
/*ADC��ɱ�־*/
u8 AdcTxFinishedFlag = False;

/********************************************************************
 * ���� : ADC1_GPIO_Config
 * ���� : ADC1 GPIO����
���ò���ͨ���˿� ʹ��GPIOʱ��	  ����ADC����PAx�˿��ź�
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void ADC_GPIO_Config(uint32_t GPIO_Pin){
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/********************************************************************
 * ���� : ADC1_Dma_Config
 * ���� : ADC1 Dma����
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void ADC1_Dma_Config(){
  PingPang_Init(&pingpang_ad,PINGPANG_IN);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //ʹ��DMA2ʱ��
  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(ADC1_DMA_STREAM); //ָ��DMAͨ�� ����
  DMA_InitStructure.DMA_Channel = ADC1_DMA_CHANNEL;
  //����DMA�����ַ
  DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(ADC1);
  //����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)(pingpang_ad.busy->data);
  //����Ϊ����Ϊ���ݴ������Դ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  //DMA����������ΪPINGPANG_LEN��
  DMA_InitStructure.DMA_BufferSize = PINGPANG_LEN;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //����ģʽ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //����ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(ADC1_DMA_STREAM, &DMA_InitStructure);

  //DMA �ж�����
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  DMA_ITConfig(ADC1_DMA_STREAM, DMA_IT_TC, ENABLE);
  //Enable DMA channel1
  DMA_Cmd(ADC1_DMA_STREAM, ENABLE); //ʹ��DMAͨ��
}
/********************************************************************
 * ���� : ADC1_Soft_Init
 * ���� : ADC ���� ����
 * ���� : ADC_Channel ADCͨ��
 * ��� : ��
 ***********************************************************************/
u8 ADC1_Soft_Init(ADC_TypeDef* adcx,uint32_t ADC_Channelx){
  ADC_GPIO_Config(((u32)1)<<ADC_Channelx);
  ADC_InitTypeDef ADC_InitStructure;
  if(ADC1 == adcx){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
  }
  else if(ADC2 == adcx){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
  }
  else if(ADC3 == adcx){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
  }
  else{
    return 0;
  }
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //��ͨ�� ����ɨ��
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //����ת�� �ر�
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //�����ⲿ����,�������ת��
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 5;
  ADC_Init(adcx, &ADC_InitStructure);
  ADC_RegularChannelConfig(adcx, ADC_Channelx, 1, ADC_SampleTime_3Cycles);
  ADC_Cmd(adcx, ENABLE);
  ADC_SoftwareStartConv(adcx);	   //�������ת��
  return 1;
}
/********************************************************************
 * ���� : ADC1_Init
 * ���� : ADC1 ���� ����ADC1�Ĺ���ģʽΪDMAģʽ
 * ���� : ADC_Channel ADCͨ��
 * ��� : ��
 ***********************************************************************/
void ADC1_Init(uint32_t ADC_Channelx){
  //�趨GPIO
  ADC_GPIO_Config(((u32)1) << ADC_Channelx);
  //�趨DMA
  ADC1_Dma_Config();
  ADC_Cmd(ADC1, DISABLE);
  // ADC1 ����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; //12λ����
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //��ͨ�� ����ɨ��
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //����ת�� �ر�
  //ʹ���ⲿ����
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_RisingFalling;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //�Ҷ���
  ADC_InitStructure.ADC_NbrOfConversion = 1; //һ��ת��ͨ��
  ADC_Init(ADC1, &ADC_InitStructure);
  //ͨ��1��������3��ʱ������
  ADC_RegularChannelConfig(ADC1, ADC_Channelx, 1, ADC_SampleTime_3Cycles);
  //����TIM2_CC2Ϊ����Դ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
  ADC_Init(ADC1, &ADC_InitStructure);
  //������ʹ��DMA
  ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1
  ADC_DMACmd(ADC1, ENABLE);
}

/********************************************************************
 * ���� : Ad_Get_Service
 * ���� : AD ������ �õ�������
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void Ad_Get_Service(){
  if (True == AdcTxFinishedFlag){
    if (pingpang_ad.busy){
      AdcTxFinishedFlag = False;
      (pingpang_ad.busy)->status = PINGPANG_USED;
      (pingpang_ad.busy)->len = 0;
      ADC1_DMA_STREAM->M0AR = (u32)(pingpang_ad.busy->data);
      ADC1_DMA_STREAM->NDTR = PINGPANG_LEN;
      //������ʹ��DMA
      ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
      DMA_ITConfig(ADC1_DMA_STREAM, DMA_IT_TC, ENABLE);
      DMA_Cmd(ADC1_DMA_STREAM, ENABLE);
      ADC_DMACmd(ADC1, ENABLE);
      ADC_Cmd(ADC1, ENABLE);
    }
    else{
      pingpang_ad.busy = PingPang_GetFree();
      if(pingpang_ad.busy){
        AdcTxFinishedFlag = False;
        ADC1_DMA_STREAM->M0AR = (u32)(pingpang_ad.busy->data);
        ADC1_DMA_STREAM->NDTR = PINGPANG_LEN;
        //������ʹ��DMA
        ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
        DMA_ITConfig(ADC1_DMA_STREAM, DMA_IT_TC, ENABLE);
        DMA_Cmd(ADC1_DMA_STREAM, ENABLE);
        ADC_DMACmd(ADC1, ENABLE);
        ADC_Cmd(ADC1, ENABLE);
      }
    }
  }
}
/********************************************************************
 * ���� : DMA2_Channel4_IRQHandler
 * ���� : DMA 3 ͨ���ж�
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void DMA2_Stream4_IRQHandler(void){
  if (DMA_GetFlagStatus(ADC1_DMA_STREAM, DMA_IT_TCIF4) != RESET){
    ADC_Cmd(ADC1, DISABLE);
    ADC_DMACmd(ADC1, DISABLE);
    DMA_ClearITPendingBit(ADC1_DMA_STREAM, DMA_IT_TCIF4);
    DMA_Cmd(ADC1_DMA_STREAM, DISABLE);
    DMA_ITConfig(ADC1_DMA_STREAM, DMA_IT_TC, DISABLE);
    (pingpang_ad.busy)->status = PINGPANG_FULL;
    (pingpang_ad.busy)->len = PINGPANG_LEN;
    if (PingPang_ChangeBusy(&pingpang_ad)){
      ADC1_DMA_STREAM->NDTR = PINGPANG_LEN;
      ADC1_DMA_STREAM->M0AR = (u32)(pingpang_ad.busy->data);
      //������ʹ��DMA
      ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
      DMA_ITConfig(ADC1_DMA_STREAM, DMA_IT_TC, ENABLE);
      DMA_Cmd(ADC1_DMA_STREAM, ENABLE);
      ADC_DMACmd(ADC1, ENABLE);
      ADC_Cmd(ADC1, ENABLE);
    }
    else{
      AdcTxFinishedFlag = True;
    }
  }
}