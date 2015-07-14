/*
 * AD.c
 *
 *  Created on: 2014年6月9日
 *      Author: lvniqi
 */
#include "AD.h"

/*乒乓组 AD*/
pingpang pingpang_ad;
/*ADC完成标志*/
u8 AdcTxFinishedFlag = False;

/********************************************************************
 * 名称 : ADC1_GPIO_Config
 * 功能 : ADC1 GPIO设置
配置采样通道端口 使能GPIO时钟	  设置ADC采样PAx端口信号
 * 输入 : 无
 * 输出 : 无
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
 * 名称 : ADC1_Dma_Config
 * 功能 : ADC1 Dma设置
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void ADC1_Dma_Config(){
  PingPang_Init(&pingpang_ad,PINGPANG_IN);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //使能DMA2时钟
  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(ADC1_DMA_STREAM); //指定DMA通道 禁用
  DMA_InitStructure.DMA_Channel = ADC1_DMA_CHANNEL;
  //设置DMA外设地址
  DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(ADC1);
  //设置DMA内存地址，ADC转换结果直接放入该地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)(pingpang_ad.busy->data);
  //外设为设置为数据传输的来源
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  //DMA缓冲区设置为PINGPANG_LEN；
  DMA_InitStructure.DMA_BufferSize = PINGPANG_LEN;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //环形模式
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //正常模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(ADC1_DMA_STREAM, &DMA_InitStructure);

  //DMA 中断设置
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  DMA_ITConfig(ADC1_DMA_STREAM, DMA_IT_TC, ENABLE);
  //Enable DMA channel1
  DMA_Cmd(ADC1_DMA_STREAM, ENABLE); //使能DMA通道
}
/********************************************************************
 * 名称 : ADC1_Soft_Init
 * 功能 : ADC 软触发 启动
 * 输入 : ADC_Channel ADC通道
 * 输出 : 无
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
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //单通道 无需扫描
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //连续转换 关闭
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不用外部触发,软件触发转换
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 5;
  ADC_Init(adcx, &ADC_InitStructure);
  ADC_RegularChannelConfig(adcx, ADC_Channelx, 1, ADC_SampleTime_3Cycles);
  ADC_Cmd(adcx, ENABLE);
  ADC_SoftwareStartConv(adcx);	   //软件启动转换
  return 1;
}
/********************************************************************
 * 名称 : ADC1_Init
 * 功能 : ADC1 设置 配置ADC1的工作模式为DMA模式
 * 输入 : ADC_Channel ADC通道
 * 输出 : 无
 ***********************************************************************/
void ADC1_Init(uint32_t ADC_Channelx){
  //设定GPIO
  ADC_GPIO_Config(((u32)1) << ADC_Channelx);
  //设定DMA
  ADC1_Dma_Config();
  ADC_Cmd(ADC1, DISABLE);
  // ADC1 设置
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; //12位数据
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //单通道 无需扫描
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //连续转换 关闭
  //使用外部触发
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_RisingFalling;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //右对齐
  ADC_InitStructure.ADC_NbrOfConversion = 1; //一个转换通道
  ADC_Init(ADC1, &ADC_InitStructure);
  //通道1采样周期3个时钟周期
  ADC_RegularChannelConfig(ADC1, ADC_Channelx, 1, ADC_SampleTime_3Cycles);
  //配置TIM2_CC2为触发源
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
  ADC_Init(ADC1, &ADC_InitStructure);
  //结束后使能DMA
  ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE); //使能ADC1
  ADC_DMACmd(ADC1, ENABLE);
}

/********************************************************************
 * 名称 : Ad_Get_Service
 * 功能 : AD 缓冲区 得到服务函数
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void Ad_Get_Service(){
  if (True == AdcTxFinishedFlag){
    if (pingpang_ad.busy){
      AdcTxFinishedFlag = False;
      (pingpang_ad.busy)->status = PINGPANG_USED;
      (pingpang_ad.busy)->len = 0;
      ADC1_DMA_STREAM->M0AR = (u32)(pingpang_ad.busy->data);
      ADC1_DMA_STREAM->NDTR = PINGPANG_LEN;
      //结束后使能DMA
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
        //结束后使能DMA
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
 * 名称 : DMA2_Channel4_IRQHandler
 * 功能 : DMA 3 通道中断
 * 输入 : 无
 * 输出 : 无
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
      //结束后使能DMA
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