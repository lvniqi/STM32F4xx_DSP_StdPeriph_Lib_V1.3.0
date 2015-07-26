#include "LCD_Tools.h"
/********************************************************************
* ���� : Lcd_Show_Line_X
* ���� : ��X��
* ���� : x1 ,x2 �� y, ��ɫ
* ��� : �� 
***********************************************************************/
void Lcd_Show_Line_X(u16 x1,u16 x2,u16 y,u16 color){
  if(x1>x2){
    swap_u16(x1,x2);
  }
  u16 len = x2-x1+1;
  while(len){
    LCD_SetCursor(x1, y);		//���ù��λ��
    LCD_WriteRAM_Prepare();	//��ʼд��GRAM
    for (u16 t1 = 0; t1 < 8&&(--len); t1++){
      LCD_WRITE_COLOR(color);
    }
    x1 += 8;
  }
}
/********************************************************************
* ���� : Lcd_Show_Line_X
* ���� : ��Y��
* ���� : x ,y1 �� y2, ��ɫ
* ��� : �� 
***********************************************************************/
void Lcd_Show_Line_Y(u16 x,u16 y1,u16 y2,u16 color){
  if(y1>y2){
    swap_u16(y1,y2);
  }
  LCD_WRITE_CMD(0x0036);												//��ʾ��������
  LCD_WRITE_DATA(0x0009);
  u16 len = y2-y1+1;
  while(len){
    LCD_SetCursor(y1, x);		//���ù��λ��
    LCD_WriteRAM_Prepare();	//��ʼд��GRAM
    for (u16 t1 = 0; t1 < 8&&(--len); t1++){
      LCD_WRITE_COLOR(color);
    }
    y1 += 8;
  }
  LCD_WRITE_CMD(0x0036);												//��ʾ��������
  LCD_WRITE_DATA(0x0029);
}
/********************************************************************
* ���� : Lcd_Show_Line
* ���� : ����
* ���� : x1,x2 ,y1 �� y2, ��ɫ
* ��� : �� 
***********************************************************************/
void Lcd_Show_Line(u16 x1,u16 y1,u16 x2,u16 y2,u16 color){
  /*else if(y1>y2){
    swap_u16(x1,x2);
    swap_u16(y1,y2);
  }*/
  int x_different = x2 - x1;
  int y_different = y2 - y1;
  if(abs_int(y_different)>abs_int(x_different)){
    if(x_different<0){
      swap_u16(x1,x2);
      swap_u16(y1,y2);
      y_different = y2 - y1;
    }
    x_different = x2 - x1 + 1;
    float div = (float)y_different/x_different;
    for(float y1_t = y1;x1<=x2;x1++,y1_t+= div){
      Lcd_Show_Line_Y(x1,y1_t,y1_t+div,color);
    }
  }else{
    if(y_different<0){
      swap_u16(x1,x2);
      swap_u16(y1,y2);
      x_different = x2 - x1;
    }
    y_different = y2 - y1 + 1;
    float div = (float)x_different/y_different;
    for(float x1_t = x1;y1<=y2;y1++,x1_t+= div){
      Lcd_Show_Line_X(x1_t,x1_t+div,y1,color);
    }
  }
}

/********************************************************************
 * ���� : LCD_Show_Rect
 * ���� : ������
 * ���� : x , y �� lenth��width�� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_Show_Rect(u16 x, u16 y, u16 lenth, u16 width, u16 point_color){
  Lcd_Show_Line(x,y,x+lenth,y,point_color);
  Lcd_Show_Line(x,y+width,x+lenth,y+width,point_color);
  Lcd_Show_Line(x,y,x,y+width,point_color);
  Lcd_Show_Line(x+lenth,y,x+lenth,y+width,point_color);
}
/********************************************************************
  * ���� : LCD_Show_Rect_FULL
  * ���� : ��������
  * ���� : x , y �� lenth��width�� ��ɫ
  * ��� : �� 
  ***********************************************************************/
void LCD_Show_Rect_FULL(u16 x, u16 y, u16 lenth, u16 width, u16 point_color){
  u16 i, j;
  for(i=0;i<width;i++){
    Lcd_Show_Line(x+i,y,x+i,y+lenth,point_color);
  }
}
/********************************************************************
 * ���� : LCD_Clear_Rect
 * ���� : �������
 * ���� : x , y �� lenth��width�� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_Clear_Rect(u16 x, u16 y, u16 lenth, u16 width, u16 point_color){
  u16 i, j;
  for (i = 0; i < lenth; i++){
    for (j = 0; j < width; j++){
      _LCD_Set_Pixel(x + i, y + j, point_color);
    }
  }
}
