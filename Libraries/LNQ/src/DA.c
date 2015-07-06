/*
 * DA.c
 *
 *  Created on: 2014��7��27��
 *      Author: lvniqi
 */
#include "DA.h"
/*da ������ɱ�־*/
u8 DacTxFinishedFlag = 1;
/*ƹ���� da*/
pingpang pingpang_da;
u16 DAC_DATA[64] ={2048, 2148, 2247, 2344, 2439, 2530, 2616, 2696, 2771, 2838, 2898, 2950, 2993, 3026, 3051, 3066, 3071, 3066, 3051, 3026, 2993, 2950, 2898, 2838, 2771, 2696, 2616, 2530, 2439, 2344, 2247, 2148, 2048, 1947, 1848, 1751, 1656, 1565, 1479, 1399, 1324, 1257, 1197, 1145, 1102, 1069, 1044, 1029, 1025, 1029, 1044, 1069, 1102, 1145, 1197, 1257, 1324, 1399, 1479, 1565, 1656, 1751, 1848, 1947};
/********************************************************************
 * ���� : DAC_GPIO_Config
 * ���� : DAC GPIO����
 * ���� : DACͨ��
 * ��� : �� 
 ***********************************************************************/
u8 DAC_GPIO_Config(u32 DAC_Channelx){
  GPIO_InitTypeDef GPIO_InitStructure;
  if(DAC_Channel_1 == DAC_Channelx){
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
  }
  else if(DAC_Channel_2 == DAC_Channelx){
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
  }
  else{
    return 0;
  }
  return 1;
}
/********************************************************************
 * ���� : DAC_Dma_Config
 * ���� : ADC1 Dma����
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
u8 DAC_Dma_Config(u32 DAC_Channelx,u32 DMA_Mode){
  if(DAC_Channel_1 == DAC_Channelx){
    //DMA����
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); //ʹ��DMA1ʱ��
    DMA_DeInit(DAC1_DMA_STREAM);
    DMA_InitStructure.DMA_Channel = DAC1_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC1_DR_BASE;
    if(DMA_Mode_Circular == DMA_Mode){
      DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DAC_DATA;
      DMA_InitStructure.DMA_BufferSize = 64;
    }
    else{
      DMA_InitStructure.DMA_Memory0BaseAddr = (u32)(pingpang_da.busy->data);
      DMA_InitStructure.DMA_BufferSize = PINGPANG_LEN;
    }
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Mode = DMA_Mode;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DAC1_DMA_STREAM, &DMA_InitStructure);
    if(DMA_Mode_Normal == DMA_Mode){
      NVIC_InitTypeDef NVIC_InitStructure;
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
    }
    return 1;
  }
  return 0;
}
/********************************************************************
 * ���� : Dac_Soft_Init
 * ���� : DAC������� ��ʼ��
 * ���� : DACͨ�� ������ʽ
 * ��� : �� 
 ***********************************************************************/
u8 Dac_Soft_Init(uint32_t DAC_Channelx)
{
  if (not(IS_DAC_CHANNEL(DAC_Channelx))){
    return 0;
  }
  DAC_GPIO_Config(DAC_Channelx);
  DAC_InitTypeDef DAC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channelx,&DAC_InitStructure);
  DAC_Cmd(DAC_Channelx,ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_R,0);
  DAC_SoftwareTriggerCmd(DAC_Channelx,ENABLE);
  return 1;
}
/********************************************************************
 * ���� : Dac_Init
 * ���� : DAC��ʼ��
 * ���� : DACͨ�� ������ʽ
 * ��� : �� 
 ***********************************************************************/
u8 Dac_Init(uint32_t DAC_Channel, uint32_t DAC_TRIGGER,uint32_t isDMA,uint32_t DMA_Mode){
  if (not(IS_DAC_CHANNEL(DAC_Channel)and IS_DAC_TRIGGER(DAC_TRIGGER))){
    return 0;
  }
  else if (DAC_Channel_1 == DAC_Channel){
    //GPIO��ʼ��
    DAC_GPIO_Config(DAC_Channel);
    //DMA��ʼ��
    if(isDMA){
      DAC_Dma_Config(DAC_Channel,DMA_Mode);
    }
    //DAC��ʼ��
    DAC_InitTypeDef DAC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    DAC_InitStructure.DAC_Trigger = DAC_TRIGGER; //ʹ�ô�������
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; 
      //��ʹ�ò��η�������
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; 
      //�����η�ֵѡ��
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable; //ʹ�û������
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    return 1;
  }
  return 0;
}

/********************************************************************
 * ���� :Dac_Send_Service
 * ���� : DAC���ͷ�����
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void Dac_Send_Service(){
  if (DacTxFinishedFlag){
    DMA_Cmd(DAC1_DMA_STREAM, DISABLE);
    DMA_ITConfig(DAC1_DMA_STREAM, DMA_IT_TC, DISABLE);
    if ((pingpang_da.geted)[PINGPANG_GETED_LEN - 1]){
      DAC1_DMA_STREAM->M0AR = (u32)((pingpang_da.geted)[PINGPANG_GETED_LEN - 1]);
      DAC1_DMA_STREAM->NDTR = PINGPANG_LEN;
      DMA_ITConfig(DAC1_DMA_STREAM, DMA_IT_TC, ENABLE);
      DMA_Cmd(DAC1_DMA_STREAM, ENABLE);
      DAC_Cmd(DAC_Channel_1, ENABLE);
      DAC_DMACmd(DAC_Channel_1, ENABLE);
      PingPang_Out(&pingpang_da);
      DacTxFinishedFlag = False;
    }
  }
}
/********************************************************************
 * ���� : DAC_Soft_Set
 * ���� : ����12λDAC
 * ���� : DACͨ�� 12λ��
 * ��� : �� 
 ***********************************************************************/
void DAC_Soft_Set(uint32_t DAC_Channelx,u16 i){
  if(DAC_Channel_1 == DAC_Channelx){
    DAC_SetChannel1Data(DAC_Align_12b_R, i); //���������Ҷ���
    DAC_SoftwareTriggerCmd(DAC_Channelx, ENABLE); //�������ͨ��1 ��ʼת��
  }
  else if(DAC_Channel_2 == DAC_Channelx){
    DAC_SetChannel2Data(DAC_Align_12b_R, i); //���������Ҷ���
    DAC_SoftwareTriggerCmd(DAC_Channelx, ENABLE); //�������ͨ��2 ��ʼת��
  }
}
/********************************************************************
 * ���� : DMA1_Stream5_IRQHandler
 * ���� : DACͨ���ж�
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void DMA1_Stream5_IRQHandler(void){
  if (DMA_GetFlagStatus(DAC1_DMA_STREAM,DMA_IT_TCIF5) != RESET){
    //DAC_Cmd(DAC_Channel_1, DISABLE);
    //DAC_DMACmd(DAC_Channel_1, DISABLE);
    if(DAC_GetFlagStatus(DAC_Channel_1,DAC_FLAG_DMAUDR)){
      DAC_ClearFlag(DAC_Channel_1,DAC_FLAG_DMAUDR);
    }
    DMA_ClearITPendingBit(DAC1_DMA_STREAM,DMA_IT_TCIF5);
    (DacTxFinishedFlag) = True;
    _pingpang_data* temp = (void*)(DAC1_DMA_STREAM->M0AR);
    PingPang_Free(temp);
    Dac_Send_Service();
  }
}
