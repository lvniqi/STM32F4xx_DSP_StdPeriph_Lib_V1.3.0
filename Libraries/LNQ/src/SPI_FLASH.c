/*
 * SPI_FLASH.c
 *
 *  Created on: 2014年7月28日
 *      Author: lvniqi
 */
#include "SPI_FLASH.h"
spi_flash_buffer SPI_FLASH_BUFFER;
u16 _SPI_FLASH_BUFFER[SPI_FLASH_BUFFER_LEN];
void Spi_Flash_Init(void){
  SPI_InitTypeDef SPI_InitStructure;
  Spi2_GPIO_Config();
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  /* SPI2 configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

  /* Enable SPI2  */
  SPI_Cmd(SPI2, ENABLE);
  SPI_FLASH_BUFFER.address_max = 0x200000;
  SPI_FLASH_BUFFER.address = 0;
  Sequeue_Init(&(SPI_FLASH_BUFFER.buffer), _SPI_FLASH_BUFFER,
               SPI_FLASH_BUFFER_LEN);
}
/*******************************************************************************
 * Function Name  : SPI_FLASH_SectorErase
 * Description    : Erases the specified FLASH sector.
 * Input          : SectorAddr: address of the sector to erase.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPI_FLASH_SectorErase(u32 SectorAddr){
  /* Send write enable instruction */
  if (SectorAddr % SPI_FLASH_PageSize){
    return ;
  }
  SPI_FLASH_WriteEnable();
  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send Sector Erase instruction */
  SPI_FLASH_SendByte(SE);
  /* Send SectorAddr high nibble address byte */
  SPI_FLASH_SendByte((SectorAddr &0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SPI_FLASH_SendByte((SectorAddr &0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SPI_FLASH_SendByte(SectorAddr &0xFF);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_BulkErase
 * Description    : Erases the entire FLASH.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPI_FLASH_BulkErase(void){
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Bulk Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send Bulk Erase instruction  */
  SPI_FLASH_SendByte(BE);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_PageWrite
 * Description    : Writes more than one byte to the FLASH with a single WRITE
 *                  cycle(Page WRITE sequence). The number of byte can't exceed
 *                  the FLASH page size.
 * Input          : - pBuffer : pointer to the buffer  containing the data to be
 *                    written to the FLASH.
 *                  - WriteAddr : FLASH's internal address to write to.
 *                  - NumByteToWrite : number of bytes to write to the FLASH,
 *                    must be equal or less than "SPI_FLASH_PageSize" value.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPI_FLASH_PageWrite(u16* pBuffer, u32 WriteAddr, u16 NumByteToWrite){
  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send "Write to Memory " instruction */
  SPI_FLASH_SendByte(WRITE);
  /* Send WriteAddr high nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr &0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr &0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI_FLASH_SendByte(WriteAddr &0xFF);

  /* while there is data to be written on the FLASH */
  while (NumByteToWrite--){
    /* Send the current byte */
    SPI_FLASH_SendByte((*pBuffer) &0xFF);
    SPI_FLASH_SendByte(((*pBuffer) &0xFF00) >> 8);
    /* Point on the next byte to be written */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_PageWrite
 * Description    : Writes more than one byte to the FLASH with a single WRITE
 *                  cycle(Page WRITE sequence). The number of byte can't exceed
 *                  the FLASH page size.
 * Input          : - pBuffer : pointer to the buffer  containing the data to be
 *                    written to the FLASH.
 *                  - WriteAddr : FLASH's internal address to write to.
 *                  - NumByteToWrite : number of bytes to write to the FLASH,
 *                    must be equal or less than "SPI_FLASH_PageSize" value.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPI_FLASH_PageRead(u16* pBuffer, u32 ReadAddr, u16 NumByteToRead){
  SPI_FLASH_CS_LOW();
  //NumByteToRead=NumByteToRead+1;
  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(READ);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr &0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr &0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr &0xFF);

  while (NumByteToRead--)
  /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = ((SPI_FLASH_SendByte(Dummy_Byte))) + (SPI_FLASH_SendByte
                (Dummy_Byte) << 8);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_BufferWrite
 * Description    : Writes block of data to the FLASH. In this function, the
 *                  number of WRITE cycles are reduced, using Page WRITE sequence.
 * Input          : - pBuffer : pointer to the buffer  containing the data to be
 *                    written to the FLASH.
 *                  - WriteAddr : FLASH's internal address to write to.
 *                  - NumByteToWrite : number of bytes to write to the FLASH.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPI_FLASH_BufferWrite(u16* pBuffer, u32 WriteAddr, u32 NumByteToWrite){
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PageSize;
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage = 2 * NumByteToWrite / SPI_FLASH_PageSize;
  NumOfSingle = (2* NumByteToWrite) % SPI_FLASH_PageSize;

  if (Addr == 0)
  /* WriteAddr is SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0)
    /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else
    /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--){
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize / 2);
        WriteAddr += SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize / 2;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle / 2);
    }
  }
  else
  /* WriteAddr is not SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0)
    /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count)
      /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count / 2);
        WriteAddr += count;
        pBuffer += count / 2;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp / 2);
      }
      else{
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite / 2);
      }
    }
    else
    /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage = (2* NumByteToWrite) / SPI_FLASH_PageSize;
      NumOfSingle = (2* NumByteToWrite) % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count / 2);
      WriteAddr += count;
      pBuffer += count / 2;

      while (NumOfPage--){
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize / 2);
        WriteAddr += SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize / 2;
      }

      if (NumOfSingle != 0){
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle / 2);
      }
    }
  }
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_BufferRead
 * Description    : Reads a block of data from the FLASH.
 * Input          : - pBuffer : pointer to the buffer that receives the data read
 *                    from the FLASH.
 *                  - ReadAddr : FLASH's internal address to read from.
 *                  - NumByteToRead : number of bytes to read from the FLASH.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPI_FLASH_BufferRead(u16* pBuffer, u32 ReadAddr, u32 NumByteToRead){
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  //NumByteToRead=NumByteToRead+1;
  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(READ);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr &0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr &0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr &0xFF);

  while (NumByteToRead--)
  /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = ((SPI_FLASH_SendByte(Dummy_Byte))) + (SPI_FLASH_SendByte
                (Dummy_Byte) << 8);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_ReadID
 * Description    : Reads FLASH identification.
 * Input          : None
 * Output         : None
 * Return         : FLASH identification
 *******************************************************************************/
u32 SPI_FLASH_ReadID(void){
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(0x9F);

  /* Read a byte from the FLASH */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_StartReadSequence
 * Description    : Initiates a read data byte (READ) sequence from the Flash.
 *                  This is done by driving the /CS line low to select the device,
 *                  then the READ instruction is transmitted followed by 3 bytes
 *                  address. This function exit and keep the /CS line low, so the
 *                  Flash still being selected. With this technique the whole
 *                  content of the Flash is read with a single READ instruction.
 * Input          : - ReadAddr : FLASH's internal address to read from.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPI_FLASH_StartReadSequence(u32 ReadAddr){
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(READ);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr &0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr &0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr &0xFF);
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_ReadByte
 * Description    : Reads a byte from the SPI Flash.
 *                  This function must be used only if the Start_Read_Sequence
 *                  function has been previously called.
 * Input          : None
 * Output         : None
 * Return         : Byte Read from the SPI Flash.
 *******************************************************************************/
u8 SPI_FLASH_ReadByte(void){
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_SendByte
 * Description    : Sends a byte through the SPI interface and return the byte
 *                  received from the SPI bus.
 * Input          : byte : byte to send.
 * Output         : None
 * Return         : The value of the received byte.
 *******************************************************************************/
u8 SPI_FLASH_SendByte(u8 byte){
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
    ;

  /* Send byte through the SPI2 peripheral */
  SPI_I2S_SendData(SPI2, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    ;

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_SendHalfWord
 * Description    : Sends a Half Word through the SPI interface and return the
 *                  Half Word received from the SPI bus.
 * Input          : Half Word : Half Word to send.
 * Output         : None
 * Return         : The value of the received Half Word.
 *******************************************************************************/
u16 SPI_FLASH_SendHalfWord(u16 HalfWord){
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
    ;

  /* Send Half Word through the SPI2 peripheral */
  SPI_I2S_SendData(SPI2, HalfWord);

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    ;

  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_WriteEnable
 * Description    : Enables the write access to the FLASH.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPI_FLASH_WriteEnable(void){
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Write Enable" instruction */
  SPI_FLASH_SendByte(WREN);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_WaitForWriteEnd
 * Description    : Polls the status of the Write In Progress (WIP) flag in the
 *                  FLASH's status  register  and  loop  until write  opertaion
 *                  has completed.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void){
  u8 FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(RDSR);

  /* Loop as long as the memory is busy with a write cycle */
  do{
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

  }
  while ((FLASH_Status &WIP_Flag) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

void SPI_FLASH_PageWrite_Squeue(u16_sequeue* sq, u32 WriteAddr, u16
                                NumByteToWrite){
  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send "Write to Memory " instruction */
  SPI_FLASH_SendByte(WRITE);
  /* Send WriteAddr high nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr &0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr &0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI_FLASH_SendByte(WriteAddr &0xFF);

  /* while there is data to be written on the FLASH */
  while (NumByteToWrite--){
    /* Send the current byte */
    u16 temp;
    temp = Sequeue_Get_Front(sq);
    SPI_FLASH_SendByte(temp &0xFF);
    SPI_FLASH_SendByte((temp &0xFF00) >> 8);
    Sequeue_Out_Queue(sq);
  }
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

int count_2;
void SPI_FLASH_Sequeue_Write(spi_flash_buffer* sq){
  int count;
  if (sq->address >= sq->address_max-SPI_FLASH_PageSize){
    return ;
  }
  while ((sq->buffer).len){
    /*单页剩余空间*/
    count = SPI_FLASH_PageSize - ((sq->address) % SPI_FLASH_PageSize);
    /*新空间 清理先*/
    /*if(count == SPI_FLASH_PageSize)
    {
    SPI_FLASH_SectorErase(sq->address);
    }*/
    /*2字节为单位*/
    count /= 2;
    /*剩余空间已用完 用了再说*/
    if (count < (sq->buffer).len){
      SPI_FLASH_PageWrite_Squeue(&(sq->buffer), sq->address, count);
      sq->address += count * 2;
    }
    /*剩余空间未用完，使用这一空间*/
    else{
      u32 temp = (sq->address);
      (sq->address) += sq->buffer.len* 2;
      SPI_FLASH_PageWrite_Squeue(&(sq->buffer), temp, (sq->buffer).len);
    }
  }
}

void SPI_FLASH_Sequeue_Read(spi_flash_buffer* sq){

  u32 ReadAddr;
  int count;
  ReadAddr = sq->address;
  count = Sequeue_GetlenMax(&(sq->buffer)) - Sequeue_Getlen(&(sq->buffer)) - 3;
  if (count > (sq->address_max) - (sq->address)){
    count = (sq->address_max - sq->address);
  }
  if (count < 0 or sq->address >= sq->address_max){
    return ;
  }
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  //NumByteToRead=NumByteToRead+1;
  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(READ);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr &0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr &0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr &0xFF);
  u16 temp;
  while (count--)
  /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    temp = ((SPI_FLASH_SendByte(Dummy_Byte))) + (SPI_FLASH_SendByte(Dummy_Byte)
            << 8);
    Sequeue_In_Queue(&(sq->buffer), temp);
    /* Point to the next location where the byte read will be saved */
    (sq->address) += 2;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}
