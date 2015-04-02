#include "main.h"
typedef volatile unsigned short* EXTERN_RAM_TYPE;
#define _FPGA_RAM_ADD (u32)0x600a0000
#define _FPGA_RAM_LEN 256
#define _FPGA_RAM_POS(x) (EXTERN_RAM_TYPE)(_FPGA_RAM_ADD|(((u32)i)<<1))
#define FPGA_RAM_WRITE(pos,value)       {*(EXTERN_RAM_TYPE)_FPGA_RAM_ADD = (1);}
#define FPGA_RAM_READ(pos)       (*(volatile unsigned short *)(_FPGA_RAM_ADD+((pos)<<1)))
#define _FPGA_REG_L (*((volatile unsigned short *) 0x600e0004))
#define _FPGA_REG_H (*((volatile unsigned short *) 0x600e0006))
