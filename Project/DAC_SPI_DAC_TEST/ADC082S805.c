#include"ADC082S805.h"
void InitAdc082Spi(){
  SPI_Cmd(SPI2, DISABLE); //必须先禁能,才能改变MODE
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  SPI_InitTypeDef SPI_InitStructure;
  Spi2_GPIO_Config();
  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  //两线全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; //8位
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //CPOL=0 时钟悬空低
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //CPHA=0 数据捕获第1个
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //软件NSS
  //警告！ 使用库函数时，APB2时钟为72M 而SPI需要低于18M 所以至少需要4分频
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
  //4分频 18Mhz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //高位在前
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  //CRC7 片子复位后，该位的值默认为0x07，因此将其设为7。
  SPI_Init(SPI2, &SPI_InitStructure);
}

void Set_Adc082(int x,u16 data){
  
  if(x>1){
    PAout(7) = 0;  
    if(x%2)   
      SPI2_WriteByte_16(0x5000|(data)<<4);
    else    
      SPI2_WriteByte_16(0x1000|(data)<<4);
    PAout(7) = 1;
  }
  else{
    PBout(12) = 0;     
    if(x%2)   
      SPI2_WriteByte_16(0x5000|(data)<<4);
    else    
      SPI2_WriteByte_16(0x1000|(data)<<4);
    PBout(12) = 1;
  }
  
}