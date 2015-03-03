#ifndef LCD_H_
  #define LCD_H_
  #include "common.h"
  #include "exti.h"
  #include "pt.h"
  #include "LCD_Curve.h"
  #include "LCD_Tools.h"
  #include "LCD_String.h"
  /*
   * LCD.H
   *
   *  Created on: 2014年5月21日
   *      Author: lvniqi
   *      Email: lvniqi@gmail.com
   */
  //#define LCD_CMD              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */
  //#define LCD_DATA              (*((volatile unsigned short *) 0x60000002)) /* RS = 1 */
  //#define LCD_REST              (*((volatile unsigned short *) 0x60000004)) //0x0002
  //#define LCD_CMD              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */
  #define LCD_DATA              (*((volatile unsigned short *) 0x60000002)) /*
    RS = 1 */
  #define LCD_REST              (*((volatile unsigned short *) 0x60000008)) 
    //0x0002
  #define DMA_ADDR              0x60000002
  //#define LCD_WRITE_CMD(Command)  (LCD_CMD= (Command|0x8000) )
  #define LCD_WRITE_CMD(Command)  (LCD_DATA= Command&(~(0x8000)))
  #define LCD_READ_DATA()  (LCD_DATA)
  #define LCD_READ_CMD()  (LCD_CMD)
  #define LCD_WRITE_DATA(Data)    (LCD_DATA= (Data|0x8000) )
  #define LCD_WRITE_COLOR(Color)  (LCD_DATA= (Color|0xC000) )

  //LCD重要参数集

  extern u16 POINT_COLOR; //默认红色
  extern u16 BACK_COLOR; //背景颜色.默认为白色
  //画笔颜色
  //2bit 保留 4bit RED 5bit GREEN 5bit BLUE
  #define WHITE         	 0xFFFF
  #define BLACK         	 0x0000	  
  #define BLUE         	         0x001F  
  #define RED           	 0x3C00
  #define GREEN         	 0x03E0

  /*液晶DMA缓冲长度*/
  #define LCD_DMA_BUFFER_LEN 14000
  #define LCD_WriteRAM_Prepare() LCD_WRITE_CMD(0x002C);

  #define LCD_WRITE_DATA_DMA(Data)    Lcd_Dma_Send(Data|0x8000)
  #define LCD_WRITE_CMD_DMA(Command)  Lcd_Dma_Send(Command&(~(0x8000)))
  #define LCD_WriteRAM_Prepare_DMA() LCD_WRITE_CMD_DMA(0x002C)
  #define LCD_WRITE_COLOR_DMA(Color)  Lcd_Dma_Send(Color|0xC000)

  /*设置单点像素*/
  #define _LCD_Set_Pixel(x,y,color)	\
  {	\
  LCD_WRITE_CMD(0x002A);\
  LCD_WRITE_DATA(x >> 8);	\
  LCD_WRITE_DATA(0x00FF & x);	\
  LCD_WRITE_DATA(x >> 8);	\
  LCD_WRITE_DATA(0x00FF & x);	\
  LCD_WRITE_CMD(0x002B);  	\
  LCD_WRITE_DATA(y >> 8);	\
  LCD_WRITE_DATA(0x00FF & y);	\
  LCD_WRITE_DATA(y >> 8);	\
  LCD_WRITE_DATA(0x00FF & y);	\
  LCD_WriteRAM_Prepare();\
  LCD_WRITE_COLOR(color);\
  }
  /*设置单点像素*/
  #define _LCD_Set_Pixel_DMA(x,y,color)	\
  {	\
  LCD_WRITE_CMD_DMA(0x002A);\
  LCD_WRITE_DATA_DMA(x >> 8);	\
  LCD_WRITE_DATA_DMA(0x00FF & x);	\
  LCD_WRITE_DATA_DMA(x >> 8);	\
  LCD_WRITE_DATA_DMA(0x00FF & x);	\
  LCD_WRITE_CMD_DMA(0x002B);  	\
  LCD_WRITE_DATA_DMA(y >> 8);	\
  LCD_WRITE_DATA_DMA(0x00FF & y);	\
  LCD_WRITE_DATA_DMA(y >> 8);	\
  LCD_WRITE_DATA_DMA(0x00FF & y);	\
  LCD_WRITE_CMD_DMA(0x002C);;\
  LCD_WRITE_COLOR_DMA(color);\
  }
  typedef struct _Lcd_Dma_Buffer{
    u16 data[LCD_DMA_BUFFER_LEN];
    u8 flag;
    u32 len;
    u32 len_max;
  } Lcd_Dma_Buffer;

  extern Lcd_Dma_Buffer lcd_dma_buffer;
  #define LCD_DMA_FREE 0
  #define LCD_DMA_FULL 2
  #define LCD_DMA_RUNNING 3
  #define LCD_DMA_LITTLE 4

  #define Lcd_Dma_Buffer_Init(p)  \
  {       \
  (p)->flag = LCD_DMA_FREE;     \
  (p)->len = 0;                 \
  (p)->len_max = LCD_DMA_BUFFER_LEN;\
  }
  #define Lcd_Dma_Buffer_Free_Len()   (lcd_dma_buffer.len_max-lcd_dma_buffer.len)
  #define Lcd_Dma_Buffer_Status()   (lcd_dma_buffer.flag)
  #define Lcd_Dma_Buffer_CAN_USE()   (lcd_dma_buffer.flag == LCD_DMA_FREE)


  /********************************************************************
   * 名称 : LCD_Init
   * 功能 : 液晶初始化
   * 输入 : 无
   * 输出 : 无
   ***********************************************************************/
  extern void LCD_Init(void);
  /********************************************************************
   * 名称 : LCD_SetCursor
   * 功能 : 设置位置
   * 输入 : Xpos  Ypos
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_SetCursor(u16 Xpos, u16 Ypos);
  /********************************************************************
   * 名称 : LCD_Clear1
   * 功能 : 清空画面
   * 输入 : 颜色
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_Clear1(u16 Colour);
  /********************************************************************
   * 名称 : LCD_Dma_Init
   * 功能 : DMA初始化
   * 输入 : 曲线
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_Dma_Init(void);
  extern u8 Lcd_Dma_Send(u16 data);
  /********************************************************************
   * 名称 : LCD_SetCursor_DMA
   * 功能 : 设置位置
   * 输入 : Xpos  Ypos
   * 输出 : 无 
   ***********************************************************************/
  extern void LCD_SetCursor_DMA(u16 Xpos, u16 Ypos);
  extern void Lcd_Dma_Send_Start(void);
#endif
