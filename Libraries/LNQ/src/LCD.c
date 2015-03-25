#include "LCD.h" 
/*
 * LCD.c
 *
 *  Created on: 2014年5月21日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR = 0xFFFF; //画笔颜色
u16 BACK_COLOR = 0x0000; //背景色
/*缓冲数组源*/
Lcd_Dma_Buffer lcd_dma_buffer;
/********************************************************************
 * 名称 : LCD_SetCursor
 * 功能 : 设置位置
 * 输入 : Xpos  Ypos
 * 输出 : 无 
 ***********************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos){
  LCD_WRITE_CMD(0x002A); //Set column start address
  LCD_WRITE_DATA(Xpos >> 8);
  LCD_WRITE_DATA(0x00FF &Xpos);
  Xpos += 7;
  LCD_WRITE_DATA(Xpos >> 8);
  LCD_WRITE_DATA(0x00FF &Xpos);
  LCD_WRITE_CMD(0x002B); //Set page start address
  LCD_WRITE_DATA(Ypos >> 8);
  LCD_WRITE_DATA(0x00FF &Ypos);
  Ypos += 15;
  LCD_WRITE_DATA(Ypos >> 8);
  LCD_WRITE_DATA(0x00FF &Ypos);
}

/********************************************************************
 * 名称 : LCD_Init
 * 功能 : 液晶初始化
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void LCD_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;

  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); //使能FSMC时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD |
                         RCC_AHB1Periph_GPIOE, ENABLE); 
                         //使能PORTB,D,E,G以及AFIO复用功能时钟
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用推挽输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

  //PORTD复用推挽输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 |
    GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
    GPIO_Pin_12 | GPIO_Pin_13|
    GPIO_Pin_14 | GPIO_Pin_15; //	//PORTD复用推挽输出
  GPIO_Init(GPIOD, &GPIO_InitStructure);


  //PORTE复用推挽输出  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 |
    GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
    GPIO_Pin_15; //	//PORTE复用推挽输出
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_FSMC);

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity =
    FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive =
    FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait =
    FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = 
    &FSMC_NORSRAMTimingInitStructure;
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 7;
  //3<8，TFT控制芯片的地址建立时间最快是10ns，所以这个和下一行的参数加起来要保证能让地址建立的时间高于这个数字，
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 4; //1or2<16
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 20;
  //5<64（+1）,相比译码写入的速度慢很多 因此加大写入延时可以提高数据稳定性，这也是与tft和fpga通形唯一差异的地方
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
  //以上参数通过调试已经比极限要高一点，极限要求低于10MHz的点数据传输速度。
  //FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;
  //也许模式B更好？
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_B;
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);



  LCD_REST = 0;
  delay_ms(100);
  LCD_REST = 1;

  LCD_WRITE_CMD(0x0011);
  delay_ms(100);
  LCD_WRITE_CMD(0x00D0);
  LCD_WRITE_DATA(0x0007);
  LCD_WRITE_DATA(0x0041);
  LCD_WRITE_DATA(0x0018);

  LCD_WRITE_CMD(0x00D1);

  LCD_WRITE_DATA(0x0000);
  LCD_WRITE_DATA(0x000a);
  LCD_WRITE_DATA(0x0010);

  LCD_WRITE_CMD(0x00D2);
  LCD_WRITE_DATA(0x0001);
  LCD_WRITE_DATA(0x0011);

  LCD_WRITE_CMD(0x00C0);
  LCD_WRITE_DATA(0x0010);
  LCD_WRITE_DATA(0x003B);
  LCD_WRITE_DATA(0x0000);
  LCD_WRITE_DATA(0x0002);
  LCD_WRITE_DATA(0x0011);

  LCD_WRITE_CMD(0x00C1);
  LCD_WRITE_DATA(0x0010);
  LCD_WRITE_DATA(0x0013);
  LCD_WRITE_DATA(0x0088);

  LCD_WRITE_CMD(0x00C5);
  LCD_WRITE_DATA(0x0002);

  LCD_WRITE_CMD(0x00C8);
  LCD_WRITE_DATA(0x0000);
  LCD_WRITE_DATA(0x0037);
  LCD_WRITE_DATA(0x0025);
  LCD_WRITE_DATA(0x0006);
  LCD_WRITE_DATA(0x0004);
  LCD_WRITE_DATA(0x001e);
  LCD_WRITE_DATA(0x0026);
  LCD_WRITE_DATA(0x0042);
  LCD_WRITE_DATA(0x0077);
  LCD_WRITE_DATA(0x0044);
  LCD_WRITE_DATA(0x000f);
  LCD_WRITE_DATA(0x0012);

  LCD_WRITE_CMD(0x00F3);
  LCD_WRITE_DATA(0x0040);
  LCD_WRITE_DATA(0x000A);

  LCD_WRITE_CMD(0x00F6);
  LCD_WRITE_DATA(0x0080);

  LCD_WRITE_CMD(0x00F7);
  LCD_WRITE_DATA(0x0080);

  LCD_WRITE_CMD(0x0036); //显示行列设置
  LCD_WRITE_DATA(0x0029);

  LCD_WRITE_CMD(0x003a);
  LCD_WRITE_DATA(0x0055);

  LCD_WRITE_CMD(0x0020);
  //	 LCD_WRITE_CMD(0x0021);

  LCD_WRITE_CMD(0x002A);
  LCD_WRITE_DATA(0x0000);
  LCD_WRITE_DATA(0x0000);
  LCD_WRITE_DATA(0x0001);
  LCD_WRITE_DATA(0x003F);

  LCD_WRITE_CMD(0x002B);
  LCD_WRITE_DATA(0x0000);
  LCD_WRITE_DATA(0x0000);
  LCD_WRITE_DATA(0x0001);
  LCD_WRITE_DATA(0x00df);

  LCD_WRITE_CMD(0x00C1);
  LCD_WRITE_DATA(0x0000);
  LCD_WRITE_DATA(0x0010);
  LCD_WRITE_DATA(0x0022);

  delay_ms(100);
  LCD_WRITE_CMD(0x0029);
  delay_ms(100);
  LCD_WRITE_CMD(0x002C);
  //		

  LCD_Clear1(BLACK);
}

/********************************************************************
 * 名称 : LCD_Clear1
 * 功能 : 清空画面
 * 输入 : 颜色
 * 输出 : 无 
 ***********************************************************************/
void LCD_Clear1(u16 color){
  u32 i;
  LCD_WRITE_CMD(0x002A); //Set column address
  LCD_WRITE_DATA(0);
  LCD_WRITE_DATA(0);
  LCD_WRITE_DATA(1);
  LCD_WRITE_DATA(223);
  LCD_WRITE_CMD(0x002B); //Set page address
  LCD_WRITE_DATA(0);
  LCD_WRITE_DATA(0);
  LCD_WRITE_DATA(1);
  LCD_WRITE_DATA(63);
  LCD_WRITE_CMD(0x002C); //write memory start
  for (i = 0; i < 0x25800; i++){
    LCD_WRITE_COLOR(color);
  }
}

/********************************************************************
 * 名称 : LCD_Dma_Init
 * 功能 : DMA初始化
 * 输入 : 曲线
 * 输出 : 无 
 ***********************************************************************/
/*void LCD_Dma_Init(void)
{
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
Lcd_Dma_Buffer_Init(&lcd_dma_buffer);
DMA_InitTypeDef    DMA_InitStructure;
DMA_DeInit(DMA1_Channel2);
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DMA_ADDR;  
DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(void*)(lcd_dma_buffer.data);
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
DMA_InitStructure.DMA_BufferSize = 0;
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
DMA_Init(DMA1_Channel2, &DMA_InitStructure);

NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
}*/

/********************************************************************
 * 名称 : DMA1_Channel2_IRQHandler
 * 功能 : DMA 3 通道中断
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
/*void DMA1_Channel2_IRQHandler(void)
{
if (DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET)
{
lcd_dma_buffer.len = 0;
lcd_dma_buffer.flag = LCD_DMA_FREE;
DMA_ClearITPendingBit(DMA1_IT_TC2);
}
}


void Lcd_Dma_Send_Start(void)
{
if(lcd_dma_buffer.len >0)
{
DMA_Cmd(DMA1_Channel2, DISABLE);
DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, DISABLE);
lcd_dma_buffer.flag = LCD_DMA_FULL;
DMA1_Channel2->CMAR = (u32)(lcd_dma_buffer.data);
DMA1_Channel2->CNDTR = lcd_dma_buffer.len;
DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
DMA_Cmd(DMA1_Channel2, ENABLE);
}
}


u8 Lcd_Dma_Send(u16 data)
{
lcd_dma_buffer.data[(lcd_dma_buffer.len)] = data;
(lcd_dma_buffer.len)++;
}*/

/********************************************************************
 * 名称 : LCD_SetCursor_DMA
 * 功能 : 设置位置
 * 输入 : Xpos  Ypos
 * 输出 : 无 
 ***********************************************************************/
void LCD_SetCursor_DMA(u16 Xpos, u16 Ypos){
  LCD_WRITE_CMD_DMA(0x002A); //Set column start address
  LCD_WRITE_DATA_DMA(Xpos >> 8);
  LCD_WRITE_DATA_DMA(0x00FF &Xpos);
  Xpos += 7;
  LCD_WRITE_DATA_DMA(Xpos >> 8);
  LCD_WRITE_DATA_DMA(0x00FF &Xpos);
  LCD_WRITE_CMD_DMA(0x002B); //Set page start address
  LCD_WRITE_DATA_DMA(Ypos >> 8);
  LCD_WRITE_DATA_DMA(0x00FF &Ypos);
  Ypos += 15;
  LCD_WRITE_DATA_DMA(Ypos >> 8);
  LCD_WRITE_DATA_DMA(0x00FF &Ypos);
}
