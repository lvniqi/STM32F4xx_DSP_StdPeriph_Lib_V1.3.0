/*
 * DA.c
 *
 *  Created on: 2014年7月27日
 *      Author: lvniqi
 */
#include "DA.h"
/*da 发送完成标志*/
u8 DacTxFinishedFlag = 1;
/*乒乓组 da*/
pingpang pingpang_da;
u16 DAC_DATA[512] ={2048, 2066, 2084, 2103, 2121, 2139, 2158, 2176, 2194, 2213, 2231, 2249, 2267, 2286, 2304, 2322, 2340, 2358, 2376, 2394, 2412, 2430, 2447, 2465, 2483, 2500, 2518, 2535, 2552, 2570, 2587, 2604, 2621, 2638, 2655, 2672, 2688, 2705, 2721, 2738, 2754, 2770, 2786, 2802, 2818, 2834, 2849, 2865, 2880, 2896, 2911, 2926, 2940, 2955, 2970, 2984, 2998, 3013, 3027, 3040, 3054, 3068, 3081, 3094, 3107, 3120, 3133, 3146, 3158, 3170, 3183, 3194, 3206, 3218, 3229, 3240, 3252, 3262, 3273, 3284, 3294, 3304, 3314, 3324, 3333, 3343, 3352, 3361, 3369, 3378, 3386, 3395, 3403, 3410, 3418, 3425, 3432, 3439, 3446, 3453, 3459, 3465, 3471, 3477, 3482, 3487, 3492, 3497, 3502, 3506, 3510, 3514, 3518, 3521, 3524, 3527, 3530, 3533, 3535, 3537, 3539, 3541, 3542, 3544, 3545, 3545, 3546, 3546, 3547, 3546, 3546, 3545, 3545, 3544, 3542, 3541, 3539, 3537, 3535, 3533, 3530, 3527, 3524, 3521, 3518, 3514, 3510, 3506, 3502, 3497, 3492, 3487, 3482, 3477, 3471, 3465, 3459, 3453, 3446, 3439, 3432, 3425, 3418, 3410, 3403, 3395, 3386, 3378, 3369, 3361, 3352, 3343, 3333, 3324, 3314, 3304, 3294, 3284, 3273, 3262, 3252, 3240, 3229, 3218, 3206, 3194, 3183, 3170, 3158, 3146, 3133, 3120, 3107, 3094, 3081, 3068, 3054, 3040, 3027, 3013, 2998, 2984, 2970, 2955, 2940, 2926, 2911, 2896, 2880, 2865, 2849, 2834, 2818, 2802, 2786, 2770, 2754, 2738, 2721, 2705, 2688, 2672, 2655, 2638, 2621, 2604, 2587, 2570, 2552, 2535, 2518, 2500, 2483, 2465, 2447, 2430, 2412, 2394, 2376, 2358, 2340, 2322, 2304, 2286, 2267, 2249, 2231, 2213, 2194, 2176, 2158, 2139, 2121, 2103, 2084, 2066, 2048, 2029, 2011, 1992, 1974, 1956, 1937, 1919, 1901, 1882, 1864, 1846, 1828, 1809, 1791, 1773, 1755, 1737, 1719, 1701, 1683, 1665, 1648, 1630, 1612, 1595, 1577, 1560, 1543, 1525, 1508, 1491, 1474, 1457, 1440, 1423, 1407, 1390, 1374, 1357, 1341, 1325, 1309, 1293, 1277, 1261, 1246, 1230, 1215, 1199, 1184, 1169, 1155, 1140, 1125, 1111, 1097, 1082, 1068, 1055, 1041, 1027, 1014, 1001, 988, 975, 962, 949, 937, 925, 912, 901, 889, 877, 866, 855, 843, 833, 822, 811, 801, 791, 781, 771, 762, 752, 743, 734, 726, 717, 709, 700, 692, 685, 677, 670, 663, 656, 649, 642, 636, 630, 624, 618, 613, 608, 603, 598, 593, 589, 585, 581, 577, 574, 571, 568, 565, 562, 560, 558, 556, 554, 553, 551, 550, 550, 549, 549, 549, 549, 549, 550, 550, 551, 553, 554, 556, 558, 560, 562, 565, 568, 571, 574, 577, 581, 585, 589, 593, 598, 603, 608, 613, 618, 624, 630, 636, 642, 649, 656, 663, 670, 677, 685, 692, 700, 709, 717, 726, 734, 743, 752, 762, 771, 781, 791, 801, 811, 822, 833, 843, 855, 866, 877, 889, 901, 912, 925, 937, 949, 962, 975, 988, 1001, 1014, 1027, 1041, 1055, 1068, 1082, 1097, 1111, 1125, 1140, 1155, 1169, 1184, 1199, 1215, 1230, 1246, 1261, 1277, 1293, 1309, 1325, 1341, 1357, 1374, 1390, 1407, 1423, 1440, 1457, 1474, 1491, 1508, 1525, 1543, 1560, 1577, 1595, 1612, 1630, 1648, 1665, 1683, 1701, 1719, 1737, 1755, 1773, 1791, 1809, 1828, 1846, 1864, 1882, 1901, 1919, 1937, 1956, 1974, 1992, 2011, 2029};
/********************************************************************
 * 名称 : DAC_GPIO_Config
 * 功能 : DAC GPIO设置
 * 输入 : DAC通道
 * 输出 : 无 
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
 * 名称 : DAC_Dma_Config
 * 功能 : ADC1 Dma设置
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
u8 DAC_Dma_Config(u32 DAC_Channelx,u32 DMA_Mode){
  if(DAC_Channel_1 == DAC_Channelx){
    //DMA设置
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); //使能DMA1时钟
    DMA_DeInit(DAC1_DMA_STREAM);
    DMA_InitStructure.DMA_Channel = DAC1_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC1_DR_BASE;
    if(DMA_Mode_Circular == DMA_Mode){
      DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DAC_DATA;
      DMA_InitStructure.DMA_BufferSize = 512;
    }
    /*else{
      DMA_InitStructure.DMA_Memory0BaseAddr = (u32)(pingpang_da.busy->data);
      DMA_InitStructure.DMA_BufferSize = PINGPANG_LEN;
    }*/
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
 * 名称 : Dac_Soft_Init
 * 功能 : DAC软件触发 初始化
 * 输入 : DAC通道 触发方式
 * 输出 : 无 
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
 * 名称 : Dac_Init
 * 功能 : DAC初始化
 * 输入 : DAC通道 触发方式
 * 输出 : 无 
 ***********************************************************************/
u8 Dac_Init(uint32_t DAC_Channel, uint32_t DAC_TRIGGER,uint32_t isDMA,uint32_t DMA_Mode){
  if (not(IS_DAC_CHANNEL(DAC_Channel)and IS_DAC_TRIGGER(DAC_TRIGGER))){
    return 0;
  }
  else if (DAC_Channel_1 == DAC_Channel){
    //GPIO初始化
    DAC_GPIO_Config(DAC_Channel);
    //DMA初始化
    if(isDMA){
      DAC_Dma_Config(DAC_Channel,DMA_Mode);
    }
    //DAC初始化
    DAC_InitTypeDef DAC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    DAC_InitStructure.DAC_Trigger = DAC_TRIGGER; //使用触发功能
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; 
      //不使用波形发生功能
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; 
      //不屏蔽幅值选择
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable; //使用缓存输出
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    if(DMA_Mode_Circular == DMA_Mode){
      DAC_Cmd(DAC_Channel_1,ENABLE);
      DMA_Cmd(DAC1_DMA_STREAM, ENABLE);
      DAC_Cmd(DAC_Channel_1, ENABLE);
      DAC_DMACmd(DAC_Channel_1, ENABLE);
    }
    return 1;
  }
  return 0;
}

/********************************************************************
 * 名称 :Dac_Send_Service
 * 功能 : DAC发送服务函数
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void Dac_Send_Service(){
  if (DacTxFinishedFlag){
    //DMA_Cmd(DAC1_DMA_STREAM, DISABLE);
    //DMA_ITConfig(DAC1_DMA_STREAM, DMA_IT_TC, DISABLE);
    if ((pingpang_da.geted)[PINGPANG_GETED_LEN - 1]&&(pingpang_da.geted)[PINGPANG_GETED_LEN - 2]){
      DAC1_DMA_STREAM->M0AR = (u32)((pingpang_da.geted)[PINGPANG_GETED_LEN - 1]);
      DAC1_DMA_STREAM->NDTR = PINGPANG_LEN;
      //双缓冲模式
      DMA_DoubleBufferModeConfig(DAC1_DMA_STREAM, (u32)((pingpang_da.geted)[PINGPANG_GETED_LEN - 2]), DMA_Memory_0);
      DMA_DoubleBufferModeCmd(DAC1_DMA_STREAM, ENABLE);
      DMA_ITConfig(DAC1_DMA_STREAM, DMA_IT_TC, ENABLE);
      DMA_Cmd(DAC1_DMA_STREAM, ENABLE);
      DAC_Cmd(DAC_Channel_1, ENABLE);
      DAC_DMACmd(DAC_Channel_1, ENABLE);
      PingPang_Out(&pingpang_da);
      PingPang_Out(&pingpang_da);
      DacTxFinishedFlag = False;
    }
  }
}
/********************************************************************
 * 名称 : DAC_Soft_Set
 * 功能 : 设置12位DAC
 * 输入 : DAC通道 12位数
 * 输出 : 无 
 ***********************************************************************/
void DAC_Soft_Set(uint32_t DAC_Channelx,u16 i){
  if(DAC_Channel_1 == DAC_Channelx){
    DAC_SetChannel1Data(DAC_Align_12b_R, i); //设置数据右对齐
    DAC_SoftwareTriggerCmd(DAC_Channelx, ENABLE); //软件触发通道1 开始转换
  }
  else if(DAC_Channel_2 == DAC_Channelx){
    DAC_SetChannel2Data(DAC_Align_12b_R, i); //设置数据右对齐
    DAC_SoftwareTriggerCmd(DAC_Channelx, ENABLE); //软件触发通道2 开始转换
  }
}
/********************************************************************
 * 名称 : DMA1_Stream5_IRQHandler
 * 功能 : DAC通道中断
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void DMA1_Stream5_IRQHandler(void){
  if (DMA_GetFlagStatus(DAC1_DMA_STREAM,DMA_IT_TCIF5) != RESET){
    //DAC_Cmd(DAC_Channel_1, DISABLE);
    //DAC_DMACmd(DAC_Channel_1, DISABLE);
    if(DAC_GetFlagStatus(DAC_Channel_1,DAC_FLAG_DMAUDR)){
      DAC_ClearFlag(DAC_Channel_1,DAC_FLAG_DMAUDR);
    }
    _pingpang_data* temp;
    if(DAC1_DMA_STREAM->CR&DMA_SxCR_CT){
      temp = (_pingpang_data *)(DAC1_DMA_STREAM->M0AR);
    }else{
      temp = (_pingpang_data *)(DAC1_DMA_STREAM->M1AR);
    }
    PingPang_Free(temp);
    if ((pingpang_da.geted)[PINGPANG_GETED_LEN - 1]){
      _pingpang_data* temp = PingPang_Out(&pingpang_da);
      if(DAC1_DMA_STREAM->CR&DMA_SxCR_CT){
        DAC1_DMA_STREAM->M0AR = (u32)temp;
      }else{
        DAC1_DMA_STREAM->M1AR = (u32)temp;
      }
    }
    else{
      if(DAC1_DMA_STREAM->CR&DMA_SxCR_CT){
        DAC1_DMA_STREAM->M0AR = 0;
      }else{
        DAC1_DMA_STREAM->M1AR = 0;
      }
    }
    if(DAC1_DMA_STREAM->M0AR == 0 && DAC1_DMA_STREAM->M1AR == 0){
      DMA_ITConfig(DAC1_DMA_STREAM, DMA_IT_TC, DISABLE);
      DMA_Cmd(DAC1_DMA_STREAM, DISABLE);
      DAC_Cmd(DAC_Channel_1, DISABLE);
      DAC_DMACmd(DAC_Channel_1, DISABLE);
      (DacTxFinishedFlag) = True;
    }
    DMA_ClearITPendingBit(DAC1_DMA_STREAM,DMA_IT_TCIF5);
    //Dac_Send_Service();
  }
}