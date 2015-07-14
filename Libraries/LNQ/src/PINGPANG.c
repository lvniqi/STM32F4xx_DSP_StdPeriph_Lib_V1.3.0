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
/*���� ��������*/
free_node pingpang_free_head = NULL;
/********************************************************************
 * ���� : PingPang_GetFree
 * ���� : �õ�����ƹ������
 * ���� : ��
 * ��� : _pingpang_data* ƹ������ָ��
 ***********************************************************************/
_pingpang_data* PingPang_GetFree(){
  __disable_irq();
  //��������
  if(pingpang_free_head){
    //�õ�һ�����л���
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
 * ���� : PingPang_Init
 * ���� : ƹ�һ���ͨ��1��ʼ��
 * ���� : ƹ�һ���ͨ��ָ��
 * ��� : ��
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
 * ���� : _PingPang_Data_Free_Init
 * ���� : ƹ�һ������ͨ����ʼ��
 * ���� : ƹ�һ������ͨ��ָ��
 * ��� : ��
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
  //��������
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