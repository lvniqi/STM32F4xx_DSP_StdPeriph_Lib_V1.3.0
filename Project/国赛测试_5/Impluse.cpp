#include "NumBar_CPP.h"
#include "Impluse.h"
//频率数据
double_sequeue IMPLUSE_SEQ[CHANNEL_LEN][2];
double _IMPLUSE_SEQ_BASE[CHANNEL_LEN*2][SHIFT_FITHER_LEN+1];
u8 IMPLUSE_USED_POS = 1;
double IMPLUSE_RESULT;
//通道1接收
double IMPLUSE_CHINNAL_1_RECV(){
  long t_b = FPGA_Read_Register(1,0);
  long t_t = FPGA_Read_Register(1,1);
  return ((double)t_b)/(t_b+t_t);
}
//通道3接收
double IMPLUSE_CHINNAL_3_RECV(){
  long t_b2 = FPGA_Read_Register(1,2);
    
  long t_t2  = FPGA_Read_Register(0,4);
  
  return ((double)(t_t2-t_b2))/t_t2;
}
//读取寄存器
u8 IMPLUSE_READ_REGISTER(int i){
  if(0 == i){
    return ((*(EXTERN_RAM)(MODE_REG[1]+0x600a000c))&0xff00)>>8;
  }else if(3 == i){
    return ((*(EXTERN_RAM)(MODE_REG[1]+0x600a000c))&0x00ff);
  }
}
//初始化全部缓冲区
void Impluse_Init_ALL(){
  for(int i=0;i<CHANNEL_LEN;i++){
    for(int j=0;j<2;j++){
      Impluse_Init(i,j);
    }
  }
}

//初始化单个缓冲区
void Impluse_Init(int channel,int x){
  Sequeue_Init(&(IMPLUSE_SEQ[channel][x]),_IMPLUSE_SEQ_BASE[channel*2+x],SHIFT_FITHER_LEN+1);
}
//输入单个double型数据到通道
void Impluse_Input(int channel ,int x,double percent){
  if(percent>0.001&&percent<0.98){
    if(Sequeue_Getlen(&(IMPLUSE_SEQ[channel][x]))>SHIFT_FITHER_LEN-1){
      Sequeue_Out_Queue(&(IMPLUSE_SEQ[channel][x]));
    }
    Sequeue_In_Queue(&(IMPLUSE_SEQ[channel][x]),percent);
    //0通道 更新RESULT读数
    if(channel == 0 && x == 0 &&IMPLUSE_USED_POS == 1){
      IMPLUSE_RESULT = percent;
    }
    //3通道 更新RESULT读数
    else if(channel == 1 && x == 1 &&IMPLUSE_USED_POS == 3){
      IMPLUSE_RESULT = percent;
    }
  }
}
//得到数据 给用户预留
double Impluse_Get(){
  if(IMPLUSE_USED_POS == 1){
    return getMid(&(IMPLUSE_SEQ[0][0]));
  }
  else if(IMPLUSE_USED_POS == 3){
    /*if(FREQ_RESULT<FREQ_LEVEL_D){
      return FREQ_RESULT;
    }else*/{
      return getMid(&(IMPLUSE_SEQ[1][1]));
    }
  }
}