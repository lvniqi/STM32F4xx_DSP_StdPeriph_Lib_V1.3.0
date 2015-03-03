/*
 * NRF2401.h
 *
 *  Created on: 2014��5��11��
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#ifndef NRF2401_H_
#define NRF2401_H_
#include "SPI.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NRF24L01�Ĵ�����������
#define NRF_READ_REG        0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define NRF_WRITE_REG       0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define NRF_RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define NRF_WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define NRF_FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define NRF_FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define NRF_REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NRF_NOP             0xFF  //�ղ���,����������״̬�Ĵ���	 
//SPI(NRF24L01)�Ĵ�����ַ
#define NRF_CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
//bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define NRF_EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define NRF_EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define NRF_SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define NRF_SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define NRF_RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define NRF_RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define NRF_STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
//bit5:���ݷ�������ж�;bit6:���������ж�;
#define NRF_MAX_TX  		0x10  //�ﵽ����ʹ����ж�
#define NRF_TX_OK   		0x20  //TX��������ж�
#define NRF_RX_OK   		0x40  //���յ������ж�
#define NRF_OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define NRF_CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define NRF_RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define NRF_RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define NRF_RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define NRF_RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define NRF_RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define NRF_RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define NRF_TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define NRF_RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_NRF_FIFO_STATUS 0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
#define NRF_DYNPD	    0x1C
#define NRF_FEATURE	    0x1D
//bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;


/* Bit Mnemonics */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define PLL_LOCK    4
#define RF_DR       3
#define RF_PWR      6
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0
#define DPL_P5	    5
#define DPL_P4	    4
#define DPL_P3	    3
#define DPL_P2	    2
#define DPL_P1	    1
#define DPL_P0	    0
#define EN_DPL	    2
#define EN_ACK_PAY  1
#define EN_DYN_ACK  0


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//24L01������
#define NRF24L01_CE   PBout(2) //ʹ���ź�
#define NRF24L01_CSN  PBout(12) //SPIƬѡ�ź�
#define NRF24L01_NSS  NRF24L01_CSN	   //SPIƬѡ�ź�
#define NRF24L01_IRQ  PCin(5)  //IRQ������������
//24L01���ͽ������ݿ�ȶ���
#define NRF_TX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define NRF_RX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define NRF_TX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��
#define NRF_RX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��

extern const u8 P0_ADDRESS[NRF_TX_ADR_WIDTH];
extern const u8 P1_ADDRESS[NRF_RX_ADR_WIDTH];
extern void InitNrf2401(void);
extern u8 Nrf2401_Check(void);
extern void Nrf2401_Open(void);
extern void NRF24L01_TX_Mode(void);
#endif /* NRF2401_H_ */
