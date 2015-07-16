/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"
#include "SelectBar_L.h"
extern "C"{
  #include "main.h"
}
Lcd_Curve Lcd_Curve1;
void REC_EN(SelectItem_L* p){
  String_L t = String_L(25,0,"WAIT!",RED);
  t.show();
  SPI_FLASH_BulkErase();
  t.hide();
  String_L(25,0,"OK!",GREEN).show();
  SPI_FLASH_BUFFER.address_max = 0x200000;
  SPI_FLASH_BUFFER.address = 0;
  Lcd_Curve1.point_color = GREEN;
}
void REC_UN(SelectItem_L* p){
  String_L(25,0,"    ",GREEN).show();
}
void PLAY_EN(SelectItem_L* p){
  SPI_FLASH_BUFFER.address = 0;
  Lcd_Curve1.point_color = YELLOW;
}
void PLAY_UN(SelectItem_L* p){  
  SPI_FLASH_BUFFER.address = 0;
  String_L(25,0,"    ",GREEN).show();
}
void Show_EN(SelectItem_L* p){  
  Lcd_Curve1.point_color = WHITE;
}
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  Spi_Flash_Init();
  TIM2_Configuration(350);
  //¿ªÆôÆ¹ÅÒ»º³å
  PingPang_Data_Free_Init();
  ADC1_Init(ADC_Channel_6);
  LCD_Curve_Init(&Lcd_Curve1,10,40,YELLOW);
  Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO,true,DMA_Mode_Normal);
  
  SelectBar_L t_select(0,0,false);
  t_select.setTitle(String_L(0,0,63,6,BLUE));
  t_select.addSelect(String_L(0,0,69,2,WHITE));
  t_select.getSelect(0)->setFunc_En(Show_EN);
  t_select.addSelect(String_L(0,0,71,2));
  t_select.getSelect(1)->setFunc_En(PLAY_EN);
  t_select.getSelect(1)->setFunc_Un(PLAY_UN);
  t_select.addSelect(String_L(0,0,73,2,GREEN));
  t_select.getSelect(2)->setFunc_En(REC_EN);
  t_select.getSelect(2)->setFunc_Un(REC_UN);
  t_select.show();
  
  while(1){
    Ad_Get_Service();
    Dac_Send_Service();
    if(MAIN_KEY.keysign){
      MAIN_KEY.keysign = 0;
      switch(MAIN_KEY.keycode){
      case 9:
        t_select.shiftLast();
        break;
      case 10:
        t_select.shiftNext();
        break;
      }
    }
    if(2 == t_select.getPos()){
      _pingpang_data *temp = PingPang_Out(&pingpang_ad);
      if(temp){
        LCD_Curve_Go_0(&Lcd_Curve1);
        for(int i=0;i<temp->len;i++){
          Sequeue_In_Queue(&(SPI_FLASH_BUFFER.buffer),temp->data[i]);
        }
        SPI_FLASH_Sequeue_Write(&SPI_FLASH_BUFFER);
        if(not Sequeue_Empty(&(SPI_FLASH_BUFFER.buffer))){
          String_L(25,0,"END!",RED).show();
          while(not Sequeue_Empty(&(SPI_FLASH_BUFFER.buffer))){
            Sequeue_Out_Queue(&(SPI_FLASH_BUFFER.buffer));
          }
        }
        PingPang_Free(temp);
      }else{
        LCD_Curve_Go_0(&Lcd_Curve1);
        for(int i=0;i<LCD_CURVE_WIDTH;i++){
          LCD_Curve_Show(&Lcd_Curve1,(SPI_FLASH_BUFFER.buffer.dataspace[i])>>4,1);
        }
      }
    }
    else if(1 == t_select.getPos()){
      _pingpang_data *temp = PingPang_Out(&pingpang_ad);
      if(temp){
        PingPang_Free(temp);
      }
      if(not pingpang_da.geted[0]){
        _pingpang_data *da_send = PingPang_GetFree();
        if(da_send){
          da_send->len = 0;
          SPI_FLASH_Sequeue_Read(&SPI_FLASH_BUFFER);
          while(not Sequeue_Empty(&(SPI_FLASH_BUFFER.buffer))){
            da_send->data[da_send->len] = Sequeue_Get_Front(&(SPI_FLASH_BUFFER.buffer));
            (da_send->len)++;
            Sequeue_Out_Queue(&(SPI_FLASH_BUFFER.buffer));
          }
          if(da_send->len){
            PingPang_In_By_PingPang(&pingpang_da,da_send);
          }else{
            String_L(25,0,"END!",RED).show();
            PingPang_Free(da_send);
          }
        }
      }else{
        u32 addr_t = SPI_FLASH_BUFFER.address;
        SPI_FLASH_Sequeue_Read(&SPI_FLASH_BUFFER);
        LCD_Curve_Go_0(&Lcd_Curve1);
        for(int i=0;i<LCD_CURVE_WIDTH;i++){
          LCD_Curve_Show(&Lcd_Curve1,(SPI_FLASH_BUFFER.buffer.dataspace[i])>>4,1);
        }
        while(not Sequeue_Empty(&(SPI_FLASH_BUFFER.buffer))){
            Sequeue_Out_Queue(&(SPI_FLASH_BUFFER.buffer));
        }
        SPI_FLASH_BUFFER.address = addr_t;
      }
    }
    else if(0 == t_select.getPos()){
      _pingpang_data *temp = PingPang_Out(&pingpang_ad);
      if(temp){
        LCD_Curve_Go_0(&Lcd_Curve1);
        for(int i=0;i<LCD_CURVE_WIDTH;i++){
          LCD_Curve_Show(&Lcd_Curve1,(temp->data[i])>>4,1);
          }
        PingPang_Free(temp);
      }
    }
  }
}