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
  // 缓冲状态 
  typedef enum _PINGPANG_STATUS{
    PINGPANG_NULL,
    PINGPANG_USED,
    PINGPANG_FULL,
    PINGPANG_USED_BY_GUI,
  }PINGPANG_STATUS;
  //缓冲类型
  typedef enum _PINGPANG_TYPE{
    PINGPANG_IN,
    PINGPANG_OUT,
  }PINGPANG_TYPE;
  #define PINGPANG_LEN 400
  #define PINGPANG_DATA_LIST_LEN 16
  #define PINGPANG_GETED_LEN 12
  typedef u16 PINGPANG_DATATYPE;
  /*乒乓数据*/
  typedef struct __pingpang_data
  {
    PINGPANG_DATATYPE data[PINGPANG_LEN];
    u16 len;
    PINGPANG_STATUS status;
    struct __pingpang_data* next;
  } _pingpang_data;
  /*乒乓通道*/
  typedef struct _pingpang
  {
    _pingpang_data *busy;//正在处理的
    
    _pingpang_data *geted[PINGPANG_GETED_LEN];//已经得到的
  } pingpang;
  typedef struct __pingpang_data* free_node;
  
  
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
   * 输入 : 乒乓缓冲通道指针 缓冲类型
   * 输出 : 无
   ***********************************************************************/
  extern void PingPang_Init(pingpang* data,PINGPANG_TYPE type);
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
  /********************************************************************
   * 名称 : PingPang_Free
   * 功能 : 释放缓冲
   * 输入 : pingpang*通道
   ***********************************************************************/
  extern void PingPang_Free(_pingpang_data *i);
#endif /* PINGPANG_H_ */
