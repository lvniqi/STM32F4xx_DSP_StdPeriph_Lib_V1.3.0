/*
 * SPI.h
 *
 *  Created on: 2014Äê5ÔÂ4ÈÕ
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#ifndef SPI_H_
  #define SPI_H_
  #include "common.h"
  extern u8 InitSpi(SPI_TypeDef *SPIx, u8 master_flag);
  extern u8 SPI2_ReadByte();
  extern void SPI2_WriteByte(u8 TxData);
  extern u8 SPI2_ReadWriteByte(u8 TxData);
  extern void Spi2_GPIO_Config(void);
  extern void SPI2_WriteByte_16(u16 TxData);
#endif /* SPI_H_ */
