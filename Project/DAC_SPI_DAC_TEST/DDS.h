#include "main.h"
#define _FPGA_RAM_ADD (u32)0x600a0000
#define _FPGA_RAM_LEN 256
#define _FPGA_RAM_POS(x) (EXTERN_RAM)(_FPGA_RAM_ADD|(((u32)i)<<1))
#define FPGA_RAM_WRITE(pos,value)       {*(EXTERN_RAM)_FPGA_RAM_ADD = (1);}
#define FPGA_RAM_READ(pos)       (*((EXTERN_RAM))(_FPGA_RAM_ADD+(((u16)pos)<<1)))
#define _FPGA_REG_L (*((EXTERN_RAM) 0x600b0800))
#define _FPGA_REG_H (*((EXTERN_RAM) 0x600b1000))
#define FPGA_SET_FREQ_H(a)      {_FPGA_REG_H = (a);}
#define FPGA_SET_FREQ_L(a)      {_FPGA_REG_L = (a);}

#define Dac_Wave_Init() \
{       \
    Dac_Wave[0] = Dac_Wave_Use_S;\
    Dac_Wave[1] = Dac_Wave_Use_F;\
    Dac_Wave[2] = Dac_Wave_Use_T;\
    Dac_Wave[3] = Dac_Wave_Use_C;\
}

#define Set_Vpp(x)      \
{       \
  VPP = x;      \
  DAC_Task((VPP-1960)*1.3470394736842104);      \
}
#define FPGA_Set_Freq(x) \
{       \
  FREQ = x;     \
  u32 _FREQ_TEMP = FREQ/0.0093132;      \
  FPGA_SET_FREQ_H((_FREQ_TEMP&0xffff0000)>>16);       \
  FPGA_SET_FREQ_L((_FREQ_TEMP&0x0000ffff));     \
  /*Freq_Service(0);*/\
}
//extern void Freq_Service(int L);
extern u32 FREQ;
//extern u16 VPP;
