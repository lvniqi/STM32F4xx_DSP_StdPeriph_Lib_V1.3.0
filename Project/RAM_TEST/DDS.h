#include "main.h"

#define _FPGA_RAM_ADD 0x600a0000
#define _FPGA_RAM_LEN 256
#define FPGA_RAM_WRITE(pos,value)       {*(volatile unsigned short *)(_FPGA_RAM_ADD+((pos)<<1)) = (value);}
#define _FPGA_REG_L (*((volatile unsigned short *) 0x600e0004))
#define _FPGA_REG_H (*((volatile unsigned short *) 0x600e0006))
