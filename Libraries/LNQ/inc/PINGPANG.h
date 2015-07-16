/*
 * PingPang.h
 *
 *  Created on: 2014��5��6��
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#ifndef PINGPANG_H_
  #define PINGPANG_H_
  #include "common.h"
  // ����״̬ 
  typedef enum _PINGPANG_STATUS{
    PINGPANG_NULL,
    PINGPANG_USED,
    PINGPANG_FULL,
    PINGPANG_USED_BY_GUI,
  }PINGPANG_STATUS;
  //��������
  typedef enum _PINGPANG_TYPE{
    PINGPANG_IN,
    PINGPANG_OUT,
  }PINGPANG_TYPE;
  #define PINGPANG_LEN 400
  #define PINGPANG_DATA_LIST_LEN 16
  #define PINGPANG_GETED_LEN 12
  typedef u16 PINGPANG_DATATYPE;
  /*ƹ������*/
  typedef struct __pingpang_data
  {
    PINGPANG_DATATYPE data[PINGPANG_LEN];
    u16 len;
    PINGPANG_STATUS status;
    struct __pingpang_data* next;
  } _pingpang_data;
  /*ƹ��ͨ��*/
  typedef struct _pingpang
  {
    _pingpang_data *busy;//���ڴ����
    
    _pingpang_data *geted[PINGPANG_GETED_LEN];//�Ѿ��õ���
  } pingpang;
  typedef struct __pingpang_data* free_node;
  
  
  /********************************************************************
   * ���� : PingPang_GetFree
   * ���� : �õ�����ƹ������
   * ���� : ��
   * ��� : _pingpang_data* ƹ������ָ��
   ***********************************************************************/
  extern _pingpang_data *PingPang_GetFree();
  /********************************************************************
   * ���� : PingPang_Init
   * ���� : ƹ�һ���ͨ��1��ʼ��
   * ���� : ƹ�һ���ͨ��ָ�� ��������
   * ��� : ��
   ***********************************************************************/
  extern void PingPang_Init(pingpang* data,PINGPANG_TYPE type);
  /********************************************************************
   * ���� : _PingPang_Data_Free_Init
   * ���� : ƹ�һ������ͨ����ʼ��
   * ���� : ƹ�һ������ͨ��ָ��
   * ��� : ��
   ***********************************************************************/
  extern void PingPang_Data_Free_Init(void);
  /********************************************************************
   * ���� : PingPang_In
   * ���� : �����ݴ���ƹ��ͨ�� 
   * ���� : pingpang*ͨ��  PINGPANG_DATATYPE data ����
   * ��� : �ɹ� 1 �� 0
   ***********************************************************************/
  extern u8 PingPang_In(pingpang *i, PINGPANG_DATATYPE data);
  /********************************************************************
   * ���� : PingPang_In_By_PingPang
   * ���� : �����ݴ���ƹ��ͨ�� 
   * ���� : pingpang*ͨ��  ƹ�� ����
   * ��� : �ɹ� 1 �� 0
   ***********************************************************************/
  extern u8 PingPang_In_By_PingPang(pingpang *i, _pingpang_data *data);
  /********************************************************************
   * ���� : PingPang_Out
   * ���� : ��ƹ��ͨ����������
   * ���� : pingpang*  ƹ��ͨ��ָ��
   * ��� : _pingpang_data* ƹ������ָ��
   ***********************************************************************/
  extern _pingpang_data *PingPang_Out(pingpang *i);
  /********************************************************************
   * ���� : PingPang_Process
   * ���� : ģ��DMA�жϴ���
   * ���� : _pingpang_data*  ƹ������
   * ��� : ��
   ***********************************************************************/
  extern void PingPang_Process(_pingpang_data *data);
  /********************************************************************
   * ���� : PingPang_ChangeBusy
   * ���� : ��æµ����ͨ������
   * ���� : pingpang*ͨ��
   * ��� : �ɹ� 1 �� 0
   ***********************************************************************/
  extern u8 PingPang_ChangeBusy(pingpang *i);
  /********************************************************************
   * ���� : PingPang_Free
   * ���� : �ͷŻ���
   * ���� : pingpang*ͨ��
   ***********************************************************************/
  extern void PingPang_Free(_pingpang_data *i);
#endif /* PINGPANG_H_ */
