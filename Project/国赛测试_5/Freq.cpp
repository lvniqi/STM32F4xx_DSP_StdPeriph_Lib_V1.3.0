#include "NumBar_CPP.h"
#include "Freq.h";
//频率数据
double_sequeue FREQ_SEQ[CHANNEL_LEN][2];
double _FREQ_SEQ_BASE[CHANNEL_LEN*2][SHIFT_FITHER_LEN+1];
u8 FREQ_USED_POS = 2;
double FREQ_RESULT;
//通道1接收
double FREQ_CHINNAL_1_RECV(){
  long t_b = FPGA_Read_Register(0,2);
  long t_t = FPGA_Read_Register(0,3);
  return ((double)t_t)/t_b*100000000;
}
//通道2接收
double FREQ_CHINNAL_2_RECV(){
  long t_b = FPGA_Read_Register(0,0);
  long t_t = FPGA_Read_Register(0,1);
  return ((double)t_t)/t_b*100000000;
}
//通道3接收
double FREQ_CHINNAL_3_RECV(){
  long t_b2 = FPGA_Read_Register(0,4);
  return ((double)100000000)/t_b2;
}
//读取寄存器
u8 FREQ_READ_REGISTER(int i){
  if(1 == i){
    return (FPGA_Read_Register(0,5)&0xff000000)>>24;
  }
  else if(2 == i){
    return (FPGA_Read_Register(0,5)&0xff00)>>8;
  }
  else if(3 == i){
    return (FPGA_Read_Register(0,5)&0xff0000)>>16;
  }
}

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
  if(freq>0){
    if(Sequeue_Getlen(&(FREQ_SEQ[channel][x]))>SHIFT_FITHER_LEN-1){
      Sequeue_Out_Queue(&(FREQ_SEQ[channel][x]));
    }
    Sequeue_In_Queue(&(FREQ_SEQ[channel][x]),freq);
    //1通道 更新RESULT读数
    if(channel == 0 && x == 0 &&FREQ_USED_POS == 1){
      FREQ_RESULT = freq;
    }
    //2通道 更新RESULT读数
    if(channel == 1 && x == 0 &&FREQ_USED_POS == 1){
      FREQ_RESULT = freq;
    }
    //3通道 更新RESULT读数
    else if(channel == 1 && x == 1 &&FREQ_USED_POS == 3){
      FREQ_RESULT = freq;
    }
  }  
}
//修正频率值单位
void Freq_Value_UnitFix(NumBar_CPP* p,int value){
  static u8 UnitLevel = 0;
  double *value_t = (double*)(void*)value;
  if(*value_t>1050000){
    p->setValueReal(*value_t/1000000);
    if(UnitLevel != 2){
      p->setRtag(String_L(0,0,"MHz",WHITE));
    }
    UnitLevel = 2;
  }
  else if(*value_t>1050){
    p->setValueReal(*value_t/1000);
    if(UnitLevel != 1){
      p->setRtag(String_L(0,0,"kHz",WHITE));
    }
    UnitLevel = 1;
  }
  else{
    if(UnitLevel != 0){
      p->setRtag(String_L(0,0,"Hz ",WHITE));
    }
    UnitLevel = 0;
  }
}
//修正周期值单位
void Tim_Value_UnitFix(NumBar_CPP* p,int value){
  static u8 UnitLevel = 0;
  double *value_t = (double*)(void*)value;
  *value_t *= 1000020000;
  if(*value_t>1050000){
    p->setValueReal(*value_t/1000000);
    if(UnitLevel != 2){
      p->setRtag(String_L(0,0,"ms",WHITE));
    }
    UnitLevel = 2;
  }
  else if(*value_t>1050){
    p->setValueReal(*value_t/1000);
    if(UnitLevel != 1){
      p->setRtag(String_L(0,0,"us",WHITE));
    }
    UnitLevel = 1;
  }
  else{
    if(UnitLevel != 0){
      p->setRtag(String_L(0,0,"ns",WHITE));
    }
    UnitLevel = 0;
  }
}
//得到数据 给用户预留
double Freq_Get(){
  if(FREQ_USED_POS == 1){
    return getMid(&(FREQ_SEQ[0][0]));
  }
  else if(FREQ_USED_POS == 2){
    return getMid(&(FREQ_SEQ[1][0]));
  }
  else if(FREQ_USED_POS == 3){
    if(FREQ_RESULT<FREQ_LEVEL_D){
      return FREQ_RESULT;
    }else{
      return getMid(&(FREQ_SEQ[1][1]));
    }
  }
}
//FREQ数据获得服务函数
PT_THREAD(FREQ_GET_SERVICE(PT *pt)){
  static u8 counter_1 = 0;
  static u8 counter_2 = 0;
  static u8 counter_3 = 0;
  PT_BEGIN(pt);
  while(1){
    PT_WAIT_UNTIL(pt,pt->ready);
    pt->ready = 0;
    u8 reg_1 = FREQ_READ_REGISTER(1);
    u8 reg_2 = FREQ_READ_REGISTER(2);
    u8 reg_3 = FREQ_READ_REGISTER(3);
    //reg_2已经读不到数了 或者值太小
    if(counter_2 == reg_2 || FREQ_RESULT<FREQ_LEVEL_D){
      //启用通道3
      __disable_irq();
      Freq_Init(1,1);
      __enable_irq();
      FREQ_USED_POS = 3;
      pt->count = 5;
      while(1){
        PT_WAIT_UNTIL(pt,pt->ready);
        pt->ready = 0;
        FREQ_RESULT = FREQ_CHINNAL_3_RECV();
        if(FREQ_RESULT>FREQ_LEVEL_U){
          pt->count = 100;
          //重启自动
          __disable_irq();
          Freq_Init(1,0);
          __enable_irq();
          FREQ_USED_POS = 2;
          break;
        }
        Freq_Input(1,1,FREQ_RESULT);
      }
    }
    counter_1 = reg_1;
    counter_2 = reg_2;
    counter_3 = reg_3;
  }
  PT_END(pt);
}