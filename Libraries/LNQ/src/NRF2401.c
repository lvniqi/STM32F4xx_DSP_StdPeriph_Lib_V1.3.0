/*
 * NRF2401.c
 *
 *  Created on: 2014��5��11��
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */

#include "NRF2401.h"
const u8 TX_ADDRESS[NRF_TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���ص�ַ
const u8 RX_ADDRESS[NRF_RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���յ�ַ
void InitNrf2401(void){ 
  // Enable SYSCFG clock 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  InitSpi(SPI2, True);
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  /*--------------------------IRQ C5�� �ж�����-----------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; /*����ģʽ*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* �����ж���2Ϊ�½�����*/
  //SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);
  //EXTI_InitStructure.EXTI_Line = EXTI_Line5; //��2
  //EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�����ж�
  //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  //EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  //NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);
  
  /*--------------------------EN B2���ź�---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; /*�������*/
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*--------------------------CSN�ź� B12 ��---------------------------------*/
}

void Nrf2401_Open(void){
  //ʹ���ź�
  NRF24L01_CE = 0; //��ʹ�ܷ���
  NRF24L01_CSN = 1; //������
  SPI_Cmd(SPI2, ENABLE); //����Nrf2401
}

void Nrf2401_Close(void){
  //ʹ���ź�
  NRF24L01_CE = 0; //��ʹ�ܷ���
  NRF24L01_CSN = 1; //������
  SPI_Cmd(SPI2, DISABLE); //�ر�Nrf2401
}
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
  u8 status;	
  NRF24L01_CSN=0;                 //ʹ��SPI����
  status =SPI2_ReadWriteByte(reg);//���ͼĴ����� 
  SPI2_ReadWriteByte(value);      //д��Ĵ�����ֵ
  NRF24L01_CSN=1;                 //��ֹSPI����	   
  return(status);       			//����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
  u8 reg_val;	    
  NRF24L01_CSN = 0;          //ʹ��SPI����		
  SPI2_ReadWriteByte(reg);   //���ͼĴ�����
  reg_val=SPI2_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
  NRF24L01_CSN = 1;          //��ֹSPI����		    
  return(reg_val);           //����״ֵ̬
}
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg, u8* pBuf, u8 len){
  u8 status, u8_ctr;
  NRF24L01_CSN = 0; //ʹ��SPI����
  status = SPI2_ReadWriteByte(reg); //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for (u8_ctr = 0; u8_ctr < len; u8_ctr++){
    pBuf[u8_ctr] = SPI2_ReadWriteByte(0XFF); //��������
  }
  NRF24L01_CSN = 1; //�ر�SPI����
  return status; //���ض�����״ֵ̬
}

//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8* pBuf, u8 len){
  u8 status, u8_ctr;
  NRF24L01_CSN = 0; //ʹ��SPI����
  status = SPI2_ReadWriteByte(reg); //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for (u8_ctr = 0; u8_ctr < len; u8_ctr++){
    SPI2_ReadWriteByte(*pBuf++);
  }
  NRF24L01_CSN = 1; //�ر�SPI����
  return status; //���ض�����״ֵ̬
}

u8 Nrf2401_Check(void){
  u8 buf[5] = {
    0XA5, 0XA5, 0XA5, 0XA5, 0XA5
  };
  u8 i;
  NRF24L01_Write_Buf(NRF_WRITE_REG + NRF_TX_ADDR, buf, 5); //д��5���ֽڵĵ�ַ.
  delay_ms(1);
  NRF24L01_Read_Buf(NRF_TX_ADDR, buf, 5); //����д��ĵ�ַ
  printf("%d \r\n", buf[0]);
  for (i = 0; i < 5; i++){
    if (buf[i] != 0XA5){
      break;
    }
  }
  if (i != 5){
    return 0; //���24L01����
  }
  return 1; //��⵽24L01
}
u8 Get_Status(void){
  u8 status;
  NRF24L01_CSN = 0; //ʹ��SPI����
  status = SPI2_ReadWriteByte(NRF_NOP);
  NRF24L01_CSN = 1; //�ر�SPI����
  return status; //���ض�����״ֵ̬
}
void Print_Status(u8 status){
  printf("STATUS\t\t = 0x%02x RX_DR=%02x TX_DS=%02x MAX_RT=%02x RX_P_NO=%02x TX_FULL=%02x\r\n",
           status,
           (status & BIT(RX_DR))?1:0,
           (status & BIT(TX_DS))?1:0,
           (status & BIT(MAX_RT))?1:0,
           ((status >> RX_P_NO)&0x07),
           (status & BIT(TX_FULL))?1:0
          );
}

void Print_Details(void){
  Print_Status(Get_Status());
  u8 a[6];
  NRF24L01_Read_Buf(NRF_READ_REG|NRF_RX_ADDR_P0,a,5);
  printf("RX_ADDR_P0\t = 0x%02x%02x%02x%02x%02x\r\n",a[0],a[1],a[2],a[3],a[4]);
  NRF24L01_Read_Buf(NRF_READ_REG|NRF_RX_ADDR_P1,a,5);
  printf("RX_ADDR_P1\t = 0x%02x%02x%02x%02x%02x\r\n",a[0],a[1],a[2],a[3],a[4]);
  a[0] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RX_ADDR_P2);
  a[1] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RX_ADDR_P3);
  a[2] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RX_ADDR_P4);
  a[3] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RX_ADDR_P5);
  printf("RX_ADDR_P2-5\t = 0x%02x 0x%02x 0x%02x 0x%02x\r\n",a[0],a[1],a[2],a[3]);
  NRF24L01_Read_Buf(NRF_READ_REG|NRF_TX_ADDR,a,5);
  printf("TX_ADDR\t\t = 0x%02x%02x%02x%02x%02x\r\n",a[0],a[1],a[2],a[3],a[4]);
  a[0] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RX_PW_P0);
  a[1] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RX_PW_P1);
  a[2] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RX_PW_P2);
  a[3] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RX_PW_P3);
  a[4] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RX_PW_P4);
  a[5] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RX_PW_P5);
  printf("RX_PW_P0-6\t = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\r\n",a[0],a[1],a[2],a[3],a[4],a[5]);
  a[0] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_EN_AA);
  printf("EN_AA\t\t = 0x%02x\r\n",a[0]);
  a[0] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_EN_RXADDR);
  printf("EN_RXADDR\t = 0x%02x\r\n",a[0]);
  a[0] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RF_CH);
  printf("RF_CH\t\t = 0x%02x\r\n",a[0]);
  a[0] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_RF_SETUP);
  printf("RF_SETUP\t = 0x%02x\r\n",a[0]);
  a[0] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_CONFIG);
  printf("CONFIG\t\t = 0x%02x\r\n",a[0]);
  a[0] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_DYNPD);
  a[1] = NRF24L01_Read_Reg(NRF_READ_REG|NRF_FEATURE);
  printf("DYNPD/FEATURE\t = 0x%02x 0x%02x\r\n",a[0],a[1]);
}
void NRF24L01_TX_Mode(void){														 
  NRF24L01_CE=0;	    
  NRF24L01_Write_Buf(NRF_WRITE_REG+NRF_TX_ADDR,(u8*)TX_ADDRESS,NRF_TX_ADR_WIDTH);//дTX�ڵ��ַ 
  NRF24L01_Write_Buf(NRF_WRITE_REG+NRF_RX_ADDR_P0,(u8*)RX_ADDRESS,NRF_RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_RF_CH,40);       //����RFͨ��Ϊ110
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
  NRF24L01_CE=1;//CEΪ��,10us����������
}
void NRF24L01_RX_Mode(void){
  NRF24L01_CE=0;	  
  NRF24L01_Write_Buf(NRF_WRITE_REG+NRF_RX_ADDR_P0,(u8*)TX_ADDRESS,NRF_RX_ADR_WIDTH);//дRX�ڵ��ַ
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_EN_AA,0x03);    //ʹ��ͨ��0��1���Զ�Ӧ��    
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_EN_RXADDR,0x03);//ʹ��ͨ��0��1�Ľ��յ�ַ  	 
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_RF_CH,40);	     //����RFͨ��Ƶ��		  
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_RX_PW_P0,NRF_RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
}	
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta; 
	NRF24L01_CE=0;
        NRF24L01_Write_Buf(NRF_WR_TX_PLOAD,txbuf,NRF_TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE=1;//��������	   
	while(NRF24L01_IRQ!=0);//�ȴ��������
	sta=NRF24L01_Read_Reg(NRF_STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&NRF_MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(NRF_FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return NRF_MAX_TX; 
	}
	if(sta&NRF_TX_OK)//�������
	{
		return NRF_TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							     
	sta=NRF24L01_Read_Reg(NRF_STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&NRF_RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(NRF_RD_RX_PLOAD,rxbuf,NRF_RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(NRF_FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}		
