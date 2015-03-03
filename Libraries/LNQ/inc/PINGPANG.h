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
  #define PINGPANG_LEN 315
  #define PINGPANG_NULL 0
  #define PINGPANG_USED 1
  #define PINGPANG_FULL 2
  #define PINGPANG_USED_BY_GUI 3

  #define PINGPANG_DATA_LIST_LEN 15
  #define PINGPANG_DATA_FREE_LIST_LEN 6
  #define PINGPANG_GETED_LEN 3
  typedef u16 PINGPANG_DATATYPE;
  /*ƹ������*/
  typedef struct __pingpang_data
  {
    PINGPANG_DATATYPE data[PINGPANG_LEN];
    u16 len;
    u8 status;
  } _pingpang_data;
  /*ƹ��ͨ��*/
  typedef struct _pingpang
  {
    _pingpang_data *busy;
    _pingpang_data *geted[PINGPANG_GETED_LEN];
  } pingpang;

  #define PingPang_Free(i)	\
  {	\
    __disable_irq();            \
    (i)->status = PINGPANG_NULL;\
    __enable_irq();             \
  }
  /*ƹ��ͨ��1*/
  /********************************************************************
   * ���� : PingPang_Service
   * ���� : ƹ�ҷ����� Ϊ��ʵʱ�õ�����ƹ������
   * ���� : ��
   * ��� : ��
   ***********************************************************************/
  extern void PingPang_Service();
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
   * ���� : ƹ�һ���ͨ��ָ��
   * ��� : ��
   ***********************************************************************/
  extern void PingPang_Init(pingpang *data);
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
#endif /* PINGPANG_H_ */
