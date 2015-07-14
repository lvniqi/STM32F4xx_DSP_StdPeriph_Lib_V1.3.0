/*
 * PINGPANG.c
 *
 *  Created on: 2014年5月6日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#include "PINGPANG.h"
/*乒乓数据列表*/
_pingpang_data _PINGPANG_DATA_LIST[PINGPANG_DATA_LIST_LEN];
/*测试 空闲链表*/
free_node pingpang_free_head = NULL;
/********************************************************************
 * 名称 : PingPang_GetFree
 * 功能 : 得到空闲乒乓数据
 * 输入 : 无
 * 输出 : _pingpang_data* 乒乓数据指针
 ***********************************************************************/
_pingpang_data* PingPang_GetFree(){
  __disable_irq();
  //测试链表
  if(pingpang_free_head){
    //得到一个空闲缓冲
    _pingpang_data* getone = pingpang_free_head;
    pingpang_free_head = pingpang_free_head->next;
    getone->next = NULL; 
    __enable_irq();
    return getone;
  }
  else{
    __enable_irq();
    return NULL;
  }
}

/********************************************************************
 * 名称 : PingPang_Init
 * 功能 : 乒乓缓冲通道1初始化
 * 输入 : 乒乓缓冲通道指针
 * 输出 : 无
 ***********************************************************************/
void PingPang_Init(pingpang* data,PINGPANG_TYPE type){
  u8 i;
  if(type == PINGPANG_IN){
    (data)->busy = PingPang_GetFree();
  }
  for (i = 0; i < PINGPANG_GETED_LEN; i++){
    (data)->geted[i] = NULL;
  }
}

/********************************************************************
 * 名称 : _PingPang_Data_Free_Init
 * 功能 : 乒乓缓冲空闲通道初始化
 * 输入 : 乒乓缓冲空闲通道指针
 * 输出 : 无
 ***********************************************************************/
void PingPang_Data_Free_Init(){
  u8 i;
  pingpang_free_head = _PINGPANG_DATA_LIST;
  pingpang_free_head->next = NULL;
  _pingpang_data* p = pingpang_free_head;
  for(i=1;i<PINGPANG_DATA_LIST_LEN;i++){
    p->next = &(_PINGPANG_DATA_LIST[i]);
    p = p->next;
  }
  p->next = NULL;
}

/********************************************************************
 * 名称 : PingPang_ChangeBusy
 * 功能 : 将忙碌数据通道更换
 * 输入 : pingpang*通道
 * 输出 : 成功 1 ， 0
 ***********************************************************************/
u8 PingPang_ChangeBusy(pingpang* i){
  __disable_irq();
  _pingpang_data* temp = (i->busy);
  int count;
  if (i->geted[0]){
    return 0;
  }
  //移到已得到的地方
  else{
    temp->status = PINGPANG_FULL;
    for (count = PINGPANG_GETED_LEN - 1; count >= 0; count--){
      if (i->geted[count] == NULL){
        i->geted[count] = temp;
        break;
      }
    }
    i->busy = PingPang_GetFree();
    if (i->busy){
      __enable_irq();
      return 1;
    }
    else{
      __enable_irq();
      return 0;
    }
  }
}

/********************************************************************
 * 名称 : PingPang_In
 * 功能 : 从数据传入乒乓通道 
 * 输入 : pingpang*通道  PINGPANG_DATATYPE data 数据
 * 输出 : 成功 1 ， 0
 ***********************************************************************/
u8 PingPang_In(pingpang* i, PINGPANG_DATATYPE data){
  _pingpang_data* temp = (i->busy);
  int count;
  if (temp){
    //未溢出
    if ((temp->len) < PINGPANG_LEN){
      (temp->data)[temp->len] = data;
      (temp->len)++;
    }
    else{
      //上次的还没处理....
      if (i->geted[0]){
        return 0;
      }
      //移到已得到的地方
      else{
        temp->status = PINGPANG_FULL;
        for (count = PINGPANG_GETED_LEN - 1; count >= 0; count--){
          if (i->geted[count] == NULL){
            i->geted[count] = temp;
            break;
          }
        }
        i->busy = PingPang_GetFree();
        PingPang_In(i, data);
      }
    }
    return 1;
  }
  else{
    return 0;
  }
}

/********************************************************************
 * 名称 : PingPang_In_By_PingPang
 * 功能 : 从数据传入乒乓通道 
 * 输入 : pingpang*通道  乒乓 数据
 * 输出 : 成功 1 ， 0
 ***********************************************************************/
u8 PingPang_In_By_PingPang(pingpang* i, _pingpang_data* data){
  int count;
  //上次的还没处理....
  if (i->geted[0]){
    return 0;
  }
  //移到已得到的地方
  else{
    for (count = PINGPANG_GETED_LEN - 1; count >= 0; count--){
      if (i->geted[count] == NULL){
        i->geted[count] = data;
        break;
      }
    }
    return 1;
  }
}

/********************************************************************
 * 名称 : PingPang_Out
 * 功能 : 从乒乓通道传出数据
 * 输入 : pingpang*  乒乓通道指针
 * 输出 : _pingpang_data* 乒乓数据指针
 ***********************************************************************/
_pingpang_data* PingPang_Out(pingpang* i){
  _pingpang_data* temp;
  if ((i->geted[PINGPANG_GETED_LEN - 1])){
    temp = i->geted[PINGPANG_GETED_LEN - 1];
    int count;
    for (count = PINGPANG_GETED_LEN - 1; count > 0; count--){
      i->geted[count] = i->geted[count - 1];
    }
    i->geted[count] = NULL;
    return temp;
  }
  /*如果PingPang_GetFree()得不到，则会出错...放弃
    
    else if ((i->busy)and((i->busy)->len)){
    temp = (i->busy);
    i->busy = PingPang_GetFree();
    return temp;
  }*/
  else{
    return NULL;
  }
}

/********************************************************************
 * 名称 : PingPang_Process
 * 功能 : 模拟DMA中断处理
 * 输入 : _pingpang_data*  乒乓数据
 * 输出 : 无
 ***********************************************************************/
void PingPang_Process(_pingpang_data* data){
  u16 i;
  for (i = 0; i < (data->len); i++){
    printf("%d ", ((data->data)[i]));
  }
  data->status = PINGPANG_NULL;
  data->len = 0;
}

/********************************************************************
 * 名称 : _Display_PingpPang
 * 功能 : 显示pingpang数据
 * 输入 : _pingpang
 * 输出 : 无
 ***********************************************************************/
void _Display_PingpPang(pingpang p1){
  _pingpang_data* temp;
  for(int j=PINGPANG_GETED_LEN-1;j>=0;j--){
    temp = p1.geted[j];
    if (temp){
      for (int i = 0; i < temp->len; i++){
        printf("geted%d:%d:\t%d\n",j, i, (temp->data)[i]);
      }
    }
  }
  temp = p1.busy;
  if (temp->status != PINGPANG_NULL){
    for (int i = 0; i < temp->len; i++){
      printf("busy:%d:\t%d\n", i, (temp->data)[i]);
    }
  }
}

void PingPang_Free(_pingpang_data *i){
  __disable_irq();            
  i->status = PINGPANG_NULL;
  //测试链表
  if(pingpang_free_head== NULL){
    pingpang_free_head = i;
    i->next = NULL;
  }
  else{
    i->next = pingpang_free_head;
    pingpang_free_head = i;
  }
  __enable_irq();             
}