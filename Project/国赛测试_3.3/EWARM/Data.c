#include "main.h"
const u32 MODE_REG[MODE_LEN] = {0x10,0x20};
//频率数据
double_sequeue FREQ_SEQ[CHANNEL_LEN][2];
double _FREQ_SEQ_BASE[CHANNEL_LEN*2][SHIFT_FITHER_LEN+1];
u8 FREQ_USED_POS;
double FREQ_RESULT;

//初始化全部缓冲区
void Freq_Init_ALL(){
  for(int i=0;i<CHANNEL_LEN;i++){
    for(int j=0;j<2;j++){
      Freq_Init(i,j);
    }
  }
}
//初始化单个缓冲区
void Freq_Init(int channel,int x){
  Sequeue_Init(&(FREQ_SEQ[channel][x]),_FREQ_SEQ_BASE[channel*2+x],SHIFT_FITHER_LEN+1);
}
//输入单个double型数据到通道
void Freq_Input(int channel ,int x,double freq){
  if(Sequeue_Getlen(&(FREQ_SEQ[channel][x]))>SHIFT_FITHER_LEN-1){
    Sequeue_Out_Queue(&(FREQ_SEQ[channel][x]));
  }
  Sequeue_In_Queue(&(FREQ_SEQ[channel][x]),freq);
}
//队列复制到数组
void Sequeue_Copy(int* len,double* r,double_sequeue* s){
  __disable_irq();
  *len = Sequeue_Getlen(s);
  for(int i=0;i<*len;i++){
    r[i] = Sequeue_Get_One(s,i);
  }
  __enable_irq();
}
//得到中点值
double getMid(double_sequeue* s){
  double temp_data[SHIFT_FITHER_LEN];
  int len;
  Sequeue_Copy(&len,temp_data,s);
  for(int i=0;i<len;i++){
    for(int j=len-1;j>i;j--){
      if(temp_data[j]>temp_data[j-1]){
        double ADC_1 = temp_data[j];
        temp_data[j] = temp_data[j-1];
        temp_data[j-1] = ADC_1;
      }
    }
  }
  if(len > 3){
    return (temp_data[len/2-1]
        +temp_data[len/2]
        +temp_data[len/2+1])/3;
  }else{
    return temp_data[len/2];
  }
}
//得到数据
double Freq_Get(){
  if(FREQ_USED_POS == 0){
    return getMid(&(FREQ_SEQ[0][0]));
  }else if(FREQ_USED_POS == 2){
    return getMid(&(FREQ_SEQ[1][1]));
  }
}
double CHINNAL_1_RECV(){
  long t_b = *(EXTERN_RAM)(MODE_REG[0]+0x600a0000);
  t_b |= *(EXTERN_RAM)(MODE_REG[0]+0x600a0002)<<16;
  long t_t = *(EXTERN_RAM)(MODE_REG[0]+0x600a0004);
  t_t |= *(EXTERN_RAM)(MODE_REG[0]+0x600a0006)<<16;
  return 0.99999*((double)t_t)/t_b*25000000;
}
double CHINNAL_3_RECV(){
  long t_b2 = *(EXTERN_RAM)(MODE_REG[0]+0x600a0008);
  t_b2 |= *(EXTERN_RAM)(MODE_REG[0]+0x600a000a)<<16;
  return 0.99999*((double)25000000)/t_b2;
}