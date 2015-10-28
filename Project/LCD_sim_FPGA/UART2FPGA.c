#include "main.h"
//写入数据 地址 二次地址 数据
void AR9331_Write(u8 addr,u8 subaddr,u16 data){
  char result[5];
  //前8位为地址
  result[0] = addr&0xfe;
  //后8位为二次地址
  result[1] = subaddr&0xfe;
  //后16位为数据
  result[2] = (data&0xfe00)>>8;
  result[3] = (data&0xfe);
  result[4] = 
    ((addr&0x01)<<7)|
    ((subaddr&0x01)<<6)|
    ((data&0x0100)>>3)|
    ((data&0x01)<<4)|0x0f;
  for(int i=0;i<5;i++){
      Usart_Send(&USART1_BUFFER,result[i]);
  }
  Usart_Flush();
}
//设定位置
void AR9331_SET_POS(u8 channel,u16 data){
  if(channel == 1){
    AR9331_Write(0xff,0,data);
  }
}
//设定放大
void AR9331_SET_ZOOM(u8 channel,u16 data){
  if(channel == 1){
    AR9331_Write(0xff,5,data);
  }
}
//设定频率
void AR9331_SET_FREQ(u8 channel,u16 data){
  if(channel == 1){
    AR9331_Write(0xfe,0,data/2-1);
  }
  else if(channel == 2){
    AR9331_Write(0xfe,1,data/2-1);
  }
}
//关闭ADC
void AR9331_UN_ADC(){
  AR9331_Write(0x00,0,0);
}
//使能ADC
void AR9331_EN_ADC(){
  AR9331_Write(0x00,0,1);
}

double AR9331_GET_FREQ(u8* data);
//从FIFO中读取数据
void AR9331_SQ_Read(u8* dat,u8_sequeue*p,u8 pos){
  static u32 len = 0;
  if(pos<7){
    return;
  }
  u8 data_e = Sequeue_Get_One(p,pos);
  for(int i=pos-7;i<pos;i++){
    dat[i] = Sequeue_Get_One(p,i);
    dat[i] |= (data_e&(1<<(7-i)))>>(7-i);
  }
  AR9331_GET_FREQ(dat);
  /*char b[30];
  sprintf(b,"%d:%x %x %x %x %x %x %x" 
          ,len++,dat[0],dat[1],dat[2],dat[3],dat[4],dat[5],dat[6]);
  LCD_ShowString(240,300,16,b,RED);*/
}
//读取数据
bool AR9331_Read(u8* result){
  int sq_len = Sequeue_Getlen(&(USART1_BUFFER.RxBuffer));
  for(int i=0;i<sq_len;i++){
    u8 t = Sequeue_Get_One(&(USART1_BUFFER.RxBuffer),i);
    if(t&0x01){
      AR9331_SQ_Read(result,&(USART1_BUFFER.RxBuffer),i);
      for(int tt=0;tt<=i;tt++){
        Sequeue_Out_Queue(&(USART1_BUFFER.RxBuffer));
      }
      return True;
    }
  }
  for(int i=0;i<sq_len-8;i++){
    Sequeue_Out_Queue(&(USART1_BUFFER.RxBuffer));
  }
  return False;
}
//读取频率
double AR9331_GET_FREQ(u8* data){
    u32 base_count = 
      ((u32)(data[1])<<16)|
        ((u32)(data[2])<<8)|
        (data[3]);
    u32 test_count = 
      (((u32)(data[4]))<<16)|
        (((u32)(data[5]))<<8)|
        (data[6]);
    double result = (1000000.0*test_count)/(base_count);
    char b[30];
    sprintf(b,"%.3lfHz",result);
    LCD_ShowString(260,300,16,b,RED);
    return result;
}
