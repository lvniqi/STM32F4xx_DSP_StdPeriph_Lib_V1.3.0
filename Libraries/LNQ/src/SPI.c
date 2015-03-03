/*
 * SPI.c
 *
 *  Created on: 2014年5月4日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#include "SPI.h"
void Spi2_GPIO_Config(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  //B14 MISO复用
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  //B13 CLK  B15 MOSI  推挽输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //B12 NSS
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
  
}
u8 InitSpi(SPI_TypeDef* SPIx, u8 master_flag){
  SPI_InitTypeDef SPI_InitStructure;
  if (SPI2 == SPIx){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    Spi2_GPIO_Config();
    SPI_Cmd(SPI2, DISABLE); //必须先禁能,才能改变MODE
    //主机状态
    if (master_flag){
      //SPI设置
      SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
      //两线全双工
      SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主
      SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位
      SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //CPOL=0 时钟悬空低
      SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //CPHA=0 数据捕获第1个
      SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //软件NSS
      //警告！ 使用库函数时，APB2时钟为72M 而SPI需要低于18M 所以至少需要4分频
      SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
      //4分频 18Mhz
      SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //高位在前
      SPI_InitStructure.SPI_CRCPolynomial = 7;
      //CRC7 片子复位后，该位的值默认为0x07，因此将其设为7。
      SPI_Init(SPI2, &SPI_InitStructure);
      PT_DEBUG(3,"SPI2 USE MASTER!\r\n");
    }
    else{
      //SPI设置
      SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
      //两线全双工
      SPI_InitStructure.SPI_Mode = SPI_Mode_Slave; //从
      SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位
      SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //CPOL=0 时钟悬空低
      SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //CPHA=0 数据捕获第1个
      SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //软件NSS
      //警告！ 使用库函数时，APB2时钟为72M 而SPI需要低于18M 所以至少需要4分频
      SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; //4分频
      SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //高位在前
      SPI_InitStructure.SPI_CRCPolynomial = 7;
      //CRC7 片子复位后，该位的值默认为0x07，因此将其设为7。
      SPI_Init(SPI2, &SPI_InitStructure);
      PT_DEBUG(3,"SPI2 USE SLAVE!\r\n");
    }
    return True;
  }
  return False;
}

void SPI2_WriteByte(u8 TxData){
  while ((SPI2->SR &1 << 1) == 0)
    ;
  //等待发送区空
  SPI2->DR = TxData; //发送一个byte
}

u8 SPI2_ReadByte(){
  while ((SPI2->SR &1 << 0) == 0)
    ;
  //等待接收完一个byte
  return SPI2->DR; //返回收到的数据
}

u8 SPI2_ReadWriteByte(u8 TxData){
  SPI2_WriteByte(TxData);
  return SPI2_ReadByte();
}
