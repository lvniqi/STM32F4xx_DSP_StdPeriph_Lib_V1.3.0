/*
 * PINGPANG.c
 *
 *  Created on: 2014��5��6��
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#include "PINGPANG.h"
/*ƹ�������б�*/
_pingpang_data _PINGPANG_DATA_LIST[PINGPANG_DATA_LIST_LEN];
/*ƹ�ҿ��������б�*/
_pingpang_data* _PINGPANG_FREE[PINGPANG_DATA_FREE_LIST_LEN];
/********************************************************************
 * ���� : PingPang_Service
 * ���� : ƹ�ҷ����� Ϊ��ʵʱ�õ�����ƹ������
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void PingPang_Service(){
  u8 i, j;
  __disable_irq();
  for (i = 0, j = 0; (i < PINGPANG_DATA_LIST_LEN)and(j <
       PINGPANG_DATA_FREE_LIST_LEN); i++){

    if ((_PINGPANG_DATA_LIST + i)->status == PINGPANG_NULL){
      _PINGPANG_FREE[j] = (_PINGPANG_DATA_LIST + i);
      j++;
    }
  }
  while(j <PINGPANG_DATA_FREE_LIST_LEN){
    _PINGPANG_FREE[j++] = NULL;
  }
  __enable_irq();
}

/********************************************************************
 * ���� : PingPang_GetFree
 * ���� : �õ�����ƹ������
 * ���� : ��
 * ��� : _pingpang_data* ƹ������ָ��
 ***********************************************************************/
_pingpang_data* PingPang_GetFree(){
  char i;
  _pingpang_data* temp;
  __disable_irq();
  for (i = 0; i < PINGPANG_DATA_FREE_LIST_LEN; i++){
    if (_PINGPANG_FREE[i]){
      temp = _PINGPANG_FREE[i];
      temp->status = PINGPANG_USED;
      temp->len = 0;
      _PINGPANG_FREE[i] = NULL;
      __enable_irq();
      return temp;
    }
  }
  __enable_irq();
  return NULL;
}

/********************************************************************
 * ���� : PingPang_Init
 * ���� : ƹ�һ���ͨ��1��ʼ��
 * ���� : ƹ�һ���ͨ��ָ��
 * ��� : ��
 ***********************************************************************/
void PingPang_Init(pingpang* data){
  u8 i;
  (data)->busy = PingPang_GetFree();
  for (i = 0; i < PINGPANG_GETED_LEN; i++){
    (data)->geted[i] = NULL;
  }
}

/********************************************************************
 * ���� : _PingPang_Data_Free_Init
 * ���� : ƹ�һ������ͨ����ʼ��
 * ���� : ƹ�һ������ͨ��ָ��
 * ��� : ��
 ***********************************************************************/
void PingPang_Data_Free_Init(){
  u8 i;
  for (i = 0; i < PINGPANG_DATA_FREE_LIST_LEN; i++){
    _PINGPANG_FREE[i] = &(_PINGPANG_DATA_LIST[i]);
  }
  PingPang_Service();
}

/********************************************************************
 * ���� : PingPang_ChangeBusy
 * ���� : ��æµ����ͨ������
 * ���� : pingpang*ͨ��
 * ��� : �ɹ� 1 �� 0
 ***********************************************************************/
u8 PingPang_ChangeBusy(pingpang* i){
  __disable_irq();
  _pingpang_data* temp = (i->busy);
  int count;
  if (i->geted[0]){
    return 0;
  }
  //�Ƶ��ѵõ��ĵط�
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
 * ���� : PingPang_In
 * ���� : �����ݴ���ƹ��ͨ�� 
 * ���� : pingpang*ͨ��  PINGPANG_DATATYPE data ����
 * ��� : �ɹ� 1 �� 0
 ***********************************************************************/
u8 PingPang_In(pingpang* i, PINGPANG_DATATYPE data){
  _pingpang_data* temp = (i->busy);
  int count;
  if (temp){
    //δ���
    if ((temp->len) < PINGPANG_LEN){
      (temp->data)[temp->len] = data;
      (temp->len)++;
    }
    else{
      //�ϴεĻ�û����....
      if (i->geted[0]){
        return 0;
      }
      //�Ƶ��ѵõ��ĵط�
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
 * ���� : PingPang_In_By_PingPang
 * ���� : �����ݴ���ƹ��ͨ�� 
 * ���� : pingpang*ͨ��  ƹ�� ����
 * ��� : �ɹ� 1 �� 0
 ***********************************************************************/
u8 PingPang_In_By_PingPang(pingpang* i, _pingpang_data* data){
  int count;
  //�ϴεĻ�û����....
  if (i->geted[0]){
    return 0;
  }
  //�Ƶ��ѵõ��ĵط�
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
 * ���� : PingPang_Out
 * ���� : ��ƹ��ͨ����������
 * ���� : pingpang*  ƹ��ͨ��ָ��
 * ��� : _pingpang_data* ƹ������ָ��
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
  /*���PingPang_GetFree()�ò�����������...����
    
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
 * ���� : PingPang_Process
 * ���� : ģ��DMA�жϴ���
 * ���� : _pingpang_data*  ƹ������
 * ��� : ��
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
 * ���� : _Display_PingpPang
 * ���� : ��ʾpingpang����
 * ���� : _pingpang
 * ��� : ��
 ***********************************************************************/
void _Display_PingpPang(pingpang p1){
  _pingpang_data* temp;
  u16 i;
  temp = p1.geted[2];
  if (temp){
    for (i = 0; i < temp->len; i++){
      printf("geted3:%d:\t%d\n", i, (temp->data)[i]);
    }
  }
  temp = p1.geted[1];
  if (temp){
    for (i = 0; i < temp->len; i++){
      printf("geted2:%d:\t%d\n", i, (temp->data)[i]);
    }
  }
  temp = p1.geted[0];
  if (temp){
    for (i = 0; i < temp->len; i++){
      printf("geted1:%d:\t%d\n", i, (temp->data)[i]);
    }
  }
  temp = p1.busy;
  if (temp->status != PINGPANG_NULL){
    for (i = 0; i < temp->len; i++){
      printf("busy:%d:\t%d\n", i, (temp->data)[i]);
    }
  }
}
