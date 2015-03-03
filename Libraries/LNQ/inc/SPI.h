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
  u8 InitSpi(SPI_TypeDef *SPIx, u8 master_flag);
  u8 SPI2_ReadByte();
  void SPI2_WriteByte(u8 TxData);
  u8 SPI2_ReadWriteByte(u8 TxData);
#endif /* SPI_H_ */
