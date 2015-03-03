/*
 * SPI.c
 *
 *  Created on: 2014��5��4��
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
  //B14 MISO����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  //B13 CLK  B15 MOSI  �������
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
    SPI_Cmd(SPI2, DISABLE); //�����Ƚ���,���ܸı�MODE
    //����״̬
    if (master_flag){
      //SPI����
      SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
      //����ȫ˫��
      SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //��
      SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λ
      SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //CPOL=0 ʱ�����յ�
      SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //CPHA=0 ���ݲ����1��
      SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //���NSS
      //���棡 ʹ�ÿ⺯��ʱ��APB2ʱ��Ϊ72M ��SPI��Ҫ����18M ����������Ҫ4��Ƶ
      SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
      //4��Ƶ 18Mhz
      SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //��λ��ǰ
      SPI_InitStructure.SPI_CRCPolynomial = 7;
      //CRC7 Ƭ�Ӹ�λ�󣬸�λ��ֵĬ��Ϊ0x07����˽�����Ϊ7��
      SPI_Init(SPI2, &SPI_InitStructure);
      PT_DEBUG(3,"SPI2 USE MASTER!\r\n");
    }
    else{
      //SPI����
      SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
      //����ȫ˫��
      SPI_InitStructure.SPI_Mode = SPI_Mode_Slave; //��
      SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λ
      SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //CPOL=0 ʱ�����յ�
      SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //CPHA=0 ���ݲ����1��
      SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //���NSS
      //���棡 ʹ�ÿ⺯��ʱ��APB2ʱ��Ϊ72M ��SPI��Ҫ����18M ����������Ҫ4��Ƶ
      SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; //4��Ƶ
      SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //��λ��ǰ
      SPI_InitStructure.SPI_CRCPolynomial = 7;
      //CRC7 Ƭ�Ӹ�λ�󣬸�λ��ֵĬ��Ϊ0x07����˽�����Ϊ7��
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
  //�ȴ���������
  SPI2->DR = TxData; //����һ��byte
}

u8 SPI2_ReadByte(){
  while ((SPI2->SR &1 << 0) == 0)
    ;
  //�ȴ�������һ��byte
  return SPI2->DR; //�����յ�������
}

u8 SPI2_ReadWriteByte(u8 TxData){
  SPI2_WriteByte(TxData);
  return SPI2_ReadByte();
}
