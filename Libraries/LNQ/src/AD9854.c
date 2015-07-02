#include "AD9854.h"

volatile unsigned long  * const AD9854_ADDRESS[6] = {
  PBout_ADDR(12),PBout_ADDR(13),PAout_ADDR(7),
  PAout_ADDR(5),PAout_ADDR(3),PAout_ADDR(1)
};
volatile unsigned long  * const AD9854_DATA[8] = {
  PEout_ADDR(5),PEout_ADDR(6),PEout_ADDR(3),PEout_ADDR(4),
  PEout_ADDR(1),PEout_ADDR(2),PBout_ADDR(9),PEout_ADDR(0)
};
#define AD9854_WR PBout(14)
#define AD9854_RD PBout(15)
#define AD9854_RST PBout(8)
#define AD9854_UDCLK PBout(6)

#define CLK_Set 12

void AD9854_GPIO_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  //Enable GPIOB clock 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = 
    GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9|
    GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //Enable GPIOE clock 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = 
    GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
    GPIO_Pin_5|GPIO_Pin_6;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  //Enable GPIOA clock 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = 
    GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void AD9854_WriteAddr(u8 addr){
  for(int i=0;i<6;i++){
    *(AD9854_ADDRESS[i]) = addr&0x01;
    addr>>=1;
  }
}


void AD9854_WriteData(u8 data){
  for(int i=0;i<8;i++){
    *(AD9854_DATA[i]) = data&0x01;
    data>>=1;
  }
}

void AD9854_WR_Byte(u8 addr,u8 dat){
  AD9854_WriteAddr(addr);
  AD9854_WriteData(dat);
  AD9854_WR=0;
  AD9854_WR=1;
}
void AD9854_Init(void){
  AD9854_GPIO_Init();
  AD9854_WR=1;//将读、写控制端口设为无效
  AD9854_RD=1;
                                                          
  AD9854_UDCLK=0;
  AD9854_RST=1;//复位AD9854
  AD9854_RST=0;
  AD9854_WR_Byte(0x1d,0x10);//关闭比较器
  AD9854_WR_Byte(0x1e,CLK_Set);//设置系统时钟倍频
  AD9854_WR_Byte(0x1f,0x00);//设置系统为模式0，由外部更新
  AD9854_WR_Byte(0x20,0x60);//设置为可调节幅度，取消插值补偿

  AD9854_UDCLK=1;//更新AD9854输出
  AD9854_UDCLK=0;

}  
const double Freq_mult_doulle = 938249.9223688533;
void Freq_convert(u8* temp_data,long Freq){
  u32 FreqBuf;
  u8 Array_Freq[4];//将输入频率因子分为四个字节
  for(int i=0;i<4;i++){
    Array_Freq[i]=(u8)(Freq>>(i*8));
  }
  FreqBuf=Freq_mult_doulle*Array_Freq[0];
  temp_data[0]=FreqBuf;
  FreqBuf>>=8;

  FreqBuf+=(Freq_mult_doulle*Array_Freq[1]);
  temp_data[1]=FreqBuf;
  FreqBuf>>=8;

  FreqBuf+=(Freq_mult_doulle*Array_Freq[2]);
  temp_data[2]=FreqBuf;
  FreqBuf>>=8;

  FreqBuf+=(Freq_mult_doulle*Array_Freq[3]);
  temp_data[3]=FreqBuf;
  FreqBuf>>=8;

  temp_data[4]=FreqBuf;
  temp_data[5]=FreqBuf>>8;
}
void AD9854_SetSinc(u32 Freq,u16 Shape){
  int count;
  u8 temp_data[6];//6个字节频率控制字
  int Adress=0x04;//选择频率控制字地址的初值
  Freq_convert(temp_data,Freq);//频率转换
  for(count=6;count>0;)//写入6字节的频率控制字
  {
          AD9854_WR_Byte(Adress++,temp_data[--count]);
  }
  AD9854_WR_Byte(0x21,Shape>>8);//设置I通道幅度
  AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
  AD9854_WR_Byte(0x23,Shape>>8);//设置Q通道幅度
  AD9854_WR_Byte(0x24,(u8)(Shape&0xff));
  AD9854_UDCLK=1;//更新AD9854输出
  AD9854_UDCLK=0;
}