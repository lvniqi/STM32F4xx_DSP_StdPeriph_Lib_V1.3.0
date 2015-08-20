#include "Data.h"
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
