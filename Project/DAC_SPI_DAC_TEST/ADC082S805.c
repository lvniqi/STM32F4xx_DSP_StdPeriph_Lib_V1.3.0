#include"ADC082S805.h"
void InitAdc082Spi(){
  SPI_Cmd(SPI2, DISABLE); //�����Ƚ���,���ܸı�MODE
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  SPI_InitTypeDef SPI_InitStructure;
  Spi2_GPIO_Config();
  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  //����ȫ˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //��
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; //8λ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //CPOL=0 ʱ�����յ�
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //CPHA=0 ���ݲ����1��
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //���NSS
  //���棡 ʹ�ÿ⺯��ʱ��APB2ʱ��Ϊ72M ��SPI��Ҫ����18M ����������Ҫ4��Ƶ
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
  //4��Ƶ 18Mhz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //��λ��ǰ
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  //CRC7 Ƭ�Ӹ�λ�󣬸�λ��ֵĬ��Ϊ0x07����˽�����Ϊ7��
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