/*
 * SPI_FLASH.h
 *
 *  Created on: 2014��7��28��
 *      Author: lvniqi
 */

#ifndef SPI_FLASH_H_
  #define SPI_FLASH_H_
  #include "SPI.h"
  #include "Sequeue.h"
  /******************* (C) COPYRIGHT 2007 STMicroelectronics *****/
  /*SPI ҳ��С*/
  #define SPI_FLASH_PageSize 256
  /*SPI д*/
  #define WRITE      0x02  /* Write to Memory instruction */
  /*SPI д�Ĵ���*/
  #define WRSR       0x01  /* Write Status Register instruction */
  /*SPI дʹ��*/
  #define WREN       0x06  /* Write enable instruction */
  /*SPI ��*/
  #define READ       0x03  /* Read from Memory instruction */
  /*SPI ��״̬�Ĵ���*/
  #define RDSR       0x05  /* Read Status Register instruction  */
  /*SPI ��ID*/
  #define RDID       0x9F  /* Read identification */
  /*�������ָ��*/
  #define SE         0xD8  /* Sector Erase instruction */
  /*�����ָ��*/
  #define BE         0xC7  /* Bulk Erase instruction */
  /*������ָ��*/
  #define WIP_Flag   0x01  /* Write In Progress (WIP) flag */
  /*�������ַ�*/
  #define Dummy_Byte 0xA5
  /******************************************************************************/
  #define SPI_FLASH_BUFFER_LEN 10
  typedef struct _spi_flash_buffer{
    u32 address;
    u32 address_max;
    int_sequeue buffer;
  } spi_flash_buffer;
  extern u16 _SPI_FLASH_BUFFER[SPI_FLASH_BUFFER_LEN];
  extern spi_flash_buffer SPI_FLASH_BUFFER;

  /* Ƭѡʹ���ź� �õ�  */
  #define SPI_FLASH_CS_LOW()     GPIO_ResetBits(GPIOA, GPIO_Pin_3)
  /* Ƭѡʹ���ź� �ø�  */
  #define SPI_FLASH_CS_HIGH()    GPIO_SetBits(GPIOA, GPIO_Pin_3)

  extern void Spi_Flash_Init(void);
  extern void SPI_FLASH_SectorErase(u32 SectorAddr);
  extern void SPI_FLASH_BulkErase(void);
  extern void SPI_FLASH_PageWrite(u16 *pBuffer, u32 WriteAddr, u16
    NumByteToWrite);
  extern void SPI_FLASH_PageRead(u16 *pBuffer, u32 WriteAddr, u16 NumByteToRead)
    ;
  extern void SPI_FLASH_BufferWrite(u16 *pBuffer, u32 WriteAddr, u32
    NumByteToWrite);
  extern void SPI_FLASH_BufferRead(u16 *pBuffer, u32 ReadAddr, u32
    NumByteToRead);
  extern u32 SPI_FLASH_ReadID(void);
  extern void SPI_FLASH_StartReadSequence(u32 ReadAddr);
  extern u8 SPI_FLASH_ReadByte(void);
  extern u8 SPI_FLASH_SendByte(u8 byte);
  extern u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
  extern void SPI_FLASH_WriteEnable(void);
  extern void SPI_FLASH_WaitForWriteEnd(void);
  extern void SPI_FLASH_PageWrite_Squeue(int_sequeue *sq, u32 WriteAddr, u16
    NumByteToWrite);
  extern void SPI_FLASH_Sequeue_Write(spi_flash_buffer *sq);
  extern void SPI_FLASH_Sequeue_Read(spi_flash_buffer *sq);
#endif /* SPI_FLASH_H_ */
