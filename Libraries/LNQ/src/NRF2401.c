/*
 * NRF2401.c
 *
 *  Created on: 2014年5月11日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */

#include "NRF2401.h"
const u8 TX_ADDRESS[NRF_TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //本地地址
const u8 RX_ADDRESS[NRF_RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //接收地址
void InitNrf2401(void){ 
  // Enable SYSCFG clock 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  InitSpi(SPI2, True);
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  /*--------------------------IRQ C5口 中断请求-----------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; /*上拉模式*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* 配置中断线2为下降触发*/
  //SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);
  //EXTI_InitStructure.EXTI_Line = EXTI_Line5; //线2
  //EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //触发中断
  //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  //EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  //NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);
  
  /*--------------------------EN B2口信号---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; /*推挽输出*/
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*--------------------------CSN信号 B12 口---------------------------------*/
}

void Nrf2401_Open(void){
  //使能信号
  NRF24L01_CE = 0; //不使能发送
  NRF24L01_CSN = 1; //不启用
  SPI_Cmd(SPI2, ENABLE); //开启Nrf2401
}

void Nrf2401_Close(void){
  //使能信号
  NRF24L01_CE = 0; //不使能发送
  NRF24L01_CSN = 1; //不启用
  SPI_Cmd(SPI2, DISABLE); //关闭Nrf2401
}
//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
  u8 status;	
  NRF24L01_CSN=0;                 //使能SPI传输
  status =SPI2_ReadWriteByte(reg);//发送寄存器号 
  SPI2_ReadWriteByte(value);      //写入寄存器的值
  NRF24L01_CSN=1;                 //禁止SPI传输	   
  return(status);       			//返回状态值
}
//读取SPI寄存器值
//reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
  u8 reg_val;	    
  NRF24L01_CSN = 0;          //使能SPI传输		
  SPI2_ReadWriteByte(reg);   //发送寄存器号
  reg_val=SPI2_ReadWriteByte(0XFF);//读取寄存器内容
  NRF24L01_CSN = 1;          //禁止SPI传输		    
  return(reg_val);           //返回状态值
}
//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
u8 NRF24L01_Read_Buf(u8 reg, u8* pBuf, u8 len){
  u8 status, u8_ctr;
  NRF24L01_CSN = 0; //使能SPI传输
  status = SPI2_ReadWriteByte(reg); //发送寄存器值(位置),并读取状态值
  for (u8_ctr = 0; u8_ctr < len; u8_ctr++){
    pBuf[u8_ctr] = SPI2_ReadWriteByte(0XFF); //读出数据
  }
  NRF24L01_CSN = 1; //关闭SPI传输
  return status; //返回读到的状态值
}

//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8* pBuf, u8 len){
  u8 status, u8_ctr;
  NRF24L01_CSN = 0; //使能SPI传输
  status = SPI2_ReadWriteByte(reg); //发送寄存器值(位置),并读取状态值
  for (u8_ctr = 0; u8_ctr < len; u8_ctr++){
    SPI2_ReadWriteByte(*pBuf++);
  }
  NRF24L01_CSN = 1; //关闭SPI传输
  return status; //返回读到的状态值
}

u8 Nrf2401_Check(void){
  u8 buf[5] = {
    0XA5, 0XA5, 0XA5, 0XA5, 0XA5
  };
  u8 i;
  NRF24L01_Write_Buf(NRF_WRITE_REG + NRF_TX_ADDR, buf, 5); //写入5个字节的地址.
  delay_ms(1);
  NRF24L01_Read_Buf(NRF_TX_ADDR, buf, 5); //读出写入的地址
  printf("%d \r\n", buf[0]);
  for (i = 0; i < 5; i++){
    if (buf[i] != 0XA5){
      break;
    }
  }
  if (i != 5){
    return 0; //检测24L01错误
  }
  return 1; //检测到24L01
}
u8 Get_Status(void){
  u8 status;
  NRF24L01_CSN = 0; //使能SPI传输
  status = SPI2_ReadWriteByte(NRF_NOP);
  NRF24L01_CSN = 1; //关闭SPI传输
  return status; //返回读到的状态值
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
  NRF24L01_Write_Buf(NRF_WRITE_REG+NRF_TX_ADDR,(u8*)TX_ADDRESS,NRF_TX_ADR_WIDTH);//写TX节点地址 
  NRF24L01_Write_Buf(NRF_WRITE_REG+NRF_RX_ADDR_P0,(u8*)RX_ADDRESS,NRF_RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_EN_AA,0x01);     //使能通道0的自动应答    
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_EN_RXADDR,0x01); //使能通道0的接收地址  
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_RF_CH,40);       //设置RF通道为110
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
  NRF24L01_CE=1;//CE为高,10us后启动发送
}
void NRF24L01_RX_Mode(void){
  NRF24L01_CE=0;	  
  NRF24L01_Write_Buf(NRF_WRITE_REG+NRF_RX_ADDR_P0,(u8*)TX_ADDRESS,NRF_RX_ADR_WIDTH);//写RX节点地址
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_EN_AA,0x03);    //使能通道0、1的自动应答    
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_EN_RXADDR,0x03);//使能通道0、1的接收地址  	 
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_RF_CH,40);	     //设置RF通信频率		  
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_RX_PW_P0,NRF_RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  NRF24L01_CE = 1; //CE为高,进入接收模式 
}	
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta; 
	NRF24L01_CE=0;
        NRF24L01_Write_Buf(NRF_WR_TX_PLOAD,txbuf,NRF_TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	NRF24L01_CE=1;//启动发送	   
	while(NRF24L01_IRQ!=0);//等待发送完成
	sta=NRF24L01_Read_Reg(NRF_STATUS);  //读取状态寄存器的值	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&NRF_MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(NRF_FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return NRF_MAX_TX; 
	}
	if(sta&NRF_TX_OK)//发送完成
	{
		return NRF_TX_OK;
	}
	return 0xff;//其他原因发送失败
}
//启动NRF24L01接收一次数据
//txbuf:待接收数据首地址
//返回值:0，接收完成；其他，错误代码
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							     
	sta=NRF24L01_Read_Reg(NRF_STATUS);  //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+NRF_STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&NRF_RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(NRF_RD_RX_PLOAD,rxbuf,NRF_RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(NRF_FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}		
