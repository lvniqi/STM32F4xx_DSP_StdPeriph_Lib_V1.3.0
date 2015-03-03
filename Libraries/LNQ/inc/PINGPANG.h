/*
 * PingPang.h
 *
 *  Created on: 2014年5月6日
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
  /*乒乓数据*/
  typedef struct __pingpang_data
  {
    PINGPANG_DATATYPE data[PINGPANG_LEN];
    u16 len;
    u8 status;
  } _pingpang_data;
  /*乒乓通道*/
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
  /*乒乓通道1*/
  /********************************************************************
   * 名称 : PingPang_Service
   * 功能 : 乒乓服务函数 为了实时得到空闲乒乓数据
   * 输入 : 无
   * 输出 : 无
   ***********************************************************************/
  extern void PingPang_Service();
  /********************************************************************
   * 名称 : PingPang_GetFree
   * 功能 : 得到空闲乒乓数据
   * 输入 : 无
   * 输出 : _pingpang_data* 乒乓数据指针
   ***********************************************************************/
  extern _pingpang_data *PingPang_GetFree();
  /********************************************************************
   * 名称 : PingPang_Init
   * 功能 : 乒乓缓冲通道1初始化
   * 输入 : 乒乓缓冲通道指针
   * 输出 : 无
   ***********************************************************************/
  extern void PingPang_Init(pingpang *data);
  /********************************************************************
   * 名称 : _PingPang_Data_Free_Init
   * 功能 : 乒乓缓冲空闲通道初始化
   * 输入 : 乒乓缓冲空闲通道指针
   * 输出 : 无
   ***********************************************************************/
  extern void PingPang_Data_Free_Init(void);
  /********************************************************************
   * 名称 : PingPang_In
   * 功能 : 从数据传入乒乓通道 
   * 输入 : pingpang*通道  PINGPANG_DATATYPE data 数据
   * 输出 : 成功 1 ， 0
   ***********************************************************************/
  extern u8 PingPang_In(pingpang *i, PINGPANG_DATATYPE data);
  /********************************************************************
   * 名称 : PingPang_In_By_PingPang
   * 功能 : 从数据传入乒乓通道 
   * 输入 : pingpang*通道  乒乓 数据
   * 输出 : 成功 1 ， 0
   ***********************************************************************/
  extern u8 PingPang_In_By_PingPang(pingpang *i, _pingpang_data *data);
  /********************************************************************
   * 名称 : PingPang_Out
   * 功能 : 从乒乓通道传出数据
   * 输入 : pingpang*  乒乓通道指针
   * 输出 : _pingpang_data* 乒乓数据指针
   ***********************************************************************/
  extern _pingpang_data *PingPang_Out(pingpang *i);
  /********************************************************************
   * 名称 : PingPang_Process
   * 功能 : 模拟DMA中断处理
   * 输入 : _pingpang_data*  乒乓数据
   * 输出 : 无
   ***********************************************************************/
  extern void PingPang_Process(_pingpang_data *data);
  /********************************************************************
   * 名称 : PingPang_ChangeBusy
   * 功能 : 将忙碌数据通道更换
   * 输入 : pingpang*通道
   * 输出 : 成功 1 ， 0
   ***********************************************************************/
  extern u8 PingPang_ChangeBusy(pingpang *i);
#endif /* PINGPANG_H_ */
