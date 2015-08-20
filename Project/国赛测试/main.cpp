/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "FPGA.h"
extern "C"{
  #include "main.h"
}
const u32 CH1_REG = 0x20;
const u32 CH2_REG = 0x40;
const u32 CH3_REG = 0x60;
u32 FPGA_Read_Register(u16 ch,u16 i){
  i *= 2;
  u32 t;
  t = *(EXTERN_RAM)(ch+0x600a0000+(i<<1));
  t |= *(EXTERN_RAM)(ch+0x600a0000+((i+1)<<1))<<16;
  return t;
}
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  {
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Configure PB1 pin as input floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  }
  u16 i;
  while(1){
    //*(EXTERN_RAM)(0x600a0000) = ++i;
    //LCD_ShowNumBig_L(0,10,1,i,WHITE);
    //PC4
    //PB0
    //u32 t_b,t_t,t_b2;
    /*for(int i=0;i<8;i++){
        LCD_ShowNumBig_L(0,10,i,FPGA_Read_Register(CH2_REG,i),WHITE);
    }*/
    /*u32 regs = FPGA_Read_Register(CH1_REG,5);
    u8 reg_1 = regs&0xff;
    u8 reg_2 = (regs&0xff00)>>8;
    u8 reg_3 = (regs&0xff0000)>>16;
    LCD_ShowNumBig_L(0,10,5,reg_1,WHITE);
    LCD_ShowNumBig_L(0,10,6,reg_2,WHITE);
    LCD_ShowNumBig_L(0,10,7,reg_3,WHITE);
    t_t = FPGA_Read_Register(CH1_REG,1);
    t_b2 = FPGA_Read_Register(CH1_REG,2);
    double t_t_1 = 100000000.0*t_t/t_b;
    double t_t2 =100000000.0/(FPGA_Read_Register(CH1_REG,4));
    LCD_ShowDoubleBig_L(0,7,8,t_t_1,WHITE);
    LCD_ShowDoubleBig_L(0,7,9,t_t2,WHITE);*/
    LCD_ShowNumBig_L(0,7,8,FPGA_Read_Register(CH3_REG,1),WHITE);
    LCD_ShowNumBig_L(0,7,9,FPGA_Read_Register(CH3_REG,0),WHITE);
    double t_t = ((double)FPGA_Read_Register(CH3_REG,1))/(FPGA_Read_Register(CH3_REG,0)+FPGA_Read_Register(CH3_REG,1));
    LCD_ShowDoubleBig_L(0,7,0,t_t*180,WHITE);
  }
}