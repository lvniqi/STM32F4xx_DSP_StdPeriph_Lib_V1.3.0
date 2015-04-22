#include "LCD_String.h"
/*sprintf����*/
char LCD_BUFFER[100];
/********************************************************************
 * ���� : LCD_ShowChar
 * ���� : ��ʾ1608�ַ� 
 * ���� :  x y �ַ� ��С ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowChar(u16 x, u16 y2, u8 num, u8 size, u16 color){
  u8 a0 = 1;
  u8 temp, t1, t;
  u16 color_2 = POINT_COLOR;
  num = num - ' ';
  LCD_SetCursor(x, y2); //���ù��λ��
  LCD_WriteRAM_Prepare(); //��ʼд��GRAM
  for (t = size - 1; t > 0; t--){
    temp = asc3_1608[num][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        POINT_COLOR = color;
      }
      else{
        POINT_COLOR = BACK_COLOR;
      }LCD_WRITE_COLOR(POINT_COLOR);
      temp <<= 1;
    }
    if (((a0 == 1) &(t == 1)) == 1){
      t++;
      a0 = 0;
    }
  }
  POINT_COLOR = color_2;
}

/********************************************************************
 * ���� : LCD_ShowCharBig
 * ���� : ��ʾ3216�ַ�
 * ���� :  x y  ��С �ַ�λ��asc_3216�����е�λ�� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowCharBig(u16 x, u16 y, u8 num, u16 color){
  int temp, t1, t;
  u16 color_2 = POINT_COLOR;
  LCD_SetCursor(x, y + 16); //���ù��λ��
  LCD_WriteRAM_Prepare(); //��ʼд��GRAM
  for (t = 15; t >= 0; t--){
    temp = asc_3216[num - ' '][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        POINT_COLOR = color;
      }
      else{
        POINT_COLOR = BACK_COLOR;
      }LCD_WRITE_COLOR(POINT_COLOR);
      temp <<= 1;
    }
  }
  LCD_SetCursor(x, y); //���ù��λ��
  LCD_WriteRAM_Prepare(); //��ʼд��GRAM
  for (t = 31; t >= 16; t--){
    temp = asc_3216[num - ' '][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        POINT_COLOR = color;
      }
      else{
        POINT_COLOR = BACK_COLOR;
      }LCD_WRITE_COLOR(POINT_COLOR);
      temp <<= 1;
    }
  }
  LCD_SetCursor(x + 8, y + 16); //���ù��λ��
  LCD_WriteRAM_Prepare(); //��ʼд��GRAM
  for (t = 47; t >= 32; t--){
    temp = asc_3216[num - ' '][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        POINT_COLOR = color;
      }
      else{
        POINT_COLOR = BACK_COLOR;
      }LCD_WRITE_COLOR(POINT_COLOR);
      temp <<= 1;
    }
  }
  LCD_SetCursor(x + 8, y); //���ù��λ��
  LCD_WriteRAM_Prepare(); //��ʼд��GRAM
  for (t = 63; t >= 48; t--){
    temp = asc_3216[num - ' '][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        POINT_COLOR = color;
      }
      else{
        POINT_COLOR = BACK_COLOR;
      }LCD_WRITE_COLOR(POINT_COLOR);
      temp <<= 1;
    }
  }
  POINT_COLOR = color_2;
}

/********************************************************************
 * ���� : LCD_ShowNumBig
 * ���� : ��ʾ3216����
 * ���� :  x y  ��С ���� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowStringBig(u16 x, u16 y, char* p, u16 color){
  while ((*p <= '~') && (*p >= ' ')){
    //�ж��ǲ��ǷǷ��ַ�!
    LCD_ShowCharBig(x, y, * p, color);
    x += 16;
    p++;
  }
}

u32 LCD_GetCharBig(u16 x, u16 y2, char* string){
  u32 data = 0;
  u16 x2 = x;
  u8 i;
  LCD_ShowStringBig(x, y2, string, BLUE);
  for (i = 1; i < 8; i++){
    while (True){
      if (MAIN_KEY.keysign){
        MAIN_KEY.keysign = 0;
        break;
      }
    }
    if (MAIN_KEY.keycode == 9 and data == 0){
      LCD_ShowCharBig(x, y2, '0', WHITE);
    }
    else if (MAIN_KEY.keycode < 9){
      data *= 10;
      data += MAIN_KEY.keycode + 1;
      LCD_ShowCharBig(x, y2, '0' + MAIN_KEY.keycode + 1, WHITE);
      x += 16;
    }
    else if (MAIN_KEY.keycode == 9){
      data *= 10;
      data += 0;
      LCD_ShowCharBig(x, y2, '0', WHITE);
      x += 16;
    }
    else if (MAIN_KEY.keycode == 15){
      break;
    }
    else{
      i--;
    }

  }
  sprintf(LCD_BUFFER, "GET!");
  LCD_ShowStringBig(x2, y2, LCD_BUFFER, BLUE);
  delay_ms(100);
  LCD_ShowStringBig(x2, y2, "          ", BLUE);
  return data;
}

/********************************************************************
 * ���� : LCD_ShowNumBig
 * ���� : ��ʾ3216����
 * ���� :  x y  ��С ���� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowNumBig(u16 x, u16 y, u32 num, u16 color){
  char temp[20], len = 0, len_max, i, p = 0;
  while (num){
    temp[len] = num % 10;
    num /= 10;
    len++;
    if ((len - p) % 3 == 0){
      temp[len] = 10;
      len++;
      p++;
    }
  }
  if (len){
    len_max = len;
    len--;
  }
  else{
    len_max = 1;
    temp[0] = 0;
  }
  if (temp[len] == 10){
    len--;
    len_max--;
  }
  for (i = 0; i < len_max; i++){
    if (temp[len] == 10){
      temp[len] = ',';
    }
    else{
      temp[len] += '0';
    }
    LCD_ShowCharBig(x + i * 16, y, temp[len], color);
    len--;
  }
}

/********************************************************************
 * ���� : LCD_ShowNumBig_L
 * ���� : �Ҷ��� ��ʾ3216����
 * ���� :  x��ʼ x����λ��  y  ��С ���� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowNumBig_L(u16 x, u16 x_end, u16 y, u32 num, u16 color){
  int temp[20], len = 0, len_max, i, p = 0;
  while (num){
    temp[len] = num % 10+'0';
    num /= 10;
    len++;
    if ((len - p) % 3 == 0){
      temp[len] = ',';
      len++;
      p++;
    }
  }
  len_max = len;
  len--;
  if (len_max == 0){
    len = 0;
    temp[len] = '0';
    len_max = 1;
  }
  else if (temp[len] == ','){
    len--;
    len_max--;
  }
  if (len_max* 16 > (x - x_end)){
    return ; //���ȹ��� ����
  }

  else if (len_max){
    x -= len_max * 16;
    for (i = x_end; i <= x; i += 16){
      LCD_ShowCharBig(i, y, ' ', color);
    }
    for (i = 0; i < len_max; i++){
      LCD_ShowCharBig(x + i * 16, y, temp[len], color);
      len--;
    }
  }
}

/********************************************************************
 * ���� : LCD_ShowString
 * ���� : ��ʾ1608�ַ���
 * ���� :  x y  ��С �ַ��� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowString(u16 x, u16 y2, u8 size, char* p, u16 color){
  while ((*p <= '~') && (*p >= ' ')){
    //�ж��ǲ��ǷǷ��ַ�!
    LCD_ShowChar(x, y2, * p, size, color);
    x += size / 2;
    p++;
  }
}

/********************************************************************
 * ���� : LCD_ShowChinese
 * ���� : ��ʾ1608�����ַ�
 * ���� :  x y  ��С ����λ��asc4_1608�����е�λ�� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowChinese(u16 x, u16 y, u8 size, u8 num, u16 color){
  ;
  u8 temp, t1;
  int t;
  LCD_SetCursor(x, y); //���ù��λ��
  LCD_WriteRAM_Prepare(); //��ʼд��GRAM
  for (t = size - 1; t > 0; t--){
    temp = asc4_1608[num][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        LCD_WRITE_COLOR(color);
      }
      else{
        LCD_WRITE_COLOR(BACK_COLOR);
      }temp <<= 1;
    }
  }
  LCD_SetCursor(x + 8, y);
  LCD_WriteRAM_Prepare(); //��ʼд��GRAM
  for (t = size - 1; t > 0; t--){
    temp = asc4_1608[num + 1][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        LCD_WRITE_COLOR(color);
      }
      else{
        LCD_WRITE_COLOR(BACK_COLOR);
      }temp <<= 1;
    }
  }
}
/********************************************************************
* ���� : LCD_ShowChineseBig
* ���� : ��ʾ3216�ַ�
* ���� :  x y  ��С �ַ�λ��asc_3216�����е�λ�� ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_ShowChineseBig(u16 x,u16 y,u8 num,u16 color)
{
	int temp, t1,t2,t3;
	u16 color_2 = POINT_COLOR;
        for(t3 = 0;t3<128;t3+=64)
        {
          LCD_SetCursor(x+t3/4, y+16);		//���ù��λ��
          LCD_WriteRAM_Prepare();	//��ʼд��GRAM
          for (t2 = 15+t3; t2 >= t3; t2--)
          {
                  temp = asc_3232[num][t2];		//����1608����
                  for (t1 = 0; t1 < 8; t1++)
                  {
                          if (temp & 0x80)
                                  POINT_COLOR = color;
                          else
                                  POINT_COLOR = BACK_COLOR;
                          LCD_WRITE_COLOR(POINT_COLOR);
                          temp <<= 1;
                  }
          }
          LCD_SetCursor(x+t3/4, y);		//���ù��λ��
          LCD_WriteRAM_Prepare();	//��ʼд��GRAM
          for (t2 = 31+t3; t2 >= 16+t3; t2--)
          {
                  temp = asc_3232[num][t2];		//����1608����
                  for (t1 = 0; t1 < 8; t1++)
                  {
                          if (temp & 0x80)
                                  POINT_COLOR = color;
                          else
                                  POINT_COLOR = BACK_COLOR;
                          LCD_WRITE_COLOR(POINT_COLOR);
                          temp <<= 1;
                  }
          }
          LCD_SetCursor(x+8+t3/4, y+16);//���ù��λ��
          LCD_WriteRAM_Prepare();	//��ʼд��GRAM
          for (t2 = 47+t3; t2 >= 32+t3; t2--)
          {
                  temp = asc_3232[num][t2];		//����1608����
                  for (t1 = 0; t1 < 8; t1++)
                  {
                          if (temp & 0x80)
                                  POINT_COLOR = color;
                          else
                                  POINT_COLOR = BACK_COLOR;
                          LCD_WRITE_COLOR(POINT_COLOR);
                          temp <<= 1;
                  }
          }
          LCD_SetCursor(x+8+t3/4, y);			//���ù��λ��
          LCD_WriteRAM_Prepare();	//��ʼд��GRAM
          for (t2 = 63+t3; t2 >= 48+t3; t2--)
          {
                  temp = asc_3232[num][t2];		//����1608����
                  for (t1 = 0; t1 < 8; t1++)
                  {
                          if (temp & 0x80)
                                  POINT_COLOR = color;
                          else
                                  POINT_COLOR = BACK_COLOR;
                          LCD_WRITE_COLOR(POINT_COLOR);
                          temp <<= 1;
                  }
          }
        }
	POINT_COLOR = color_2;
}
/********************************************************************
* ���� : LCD_ShowChineseStringBig
* ���� : ��ʾ3216�ַ�
* ���� :  x y  ��С �ַ�λ��asc_3216�����е�λ�� ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_ShowChineseStringBig(u16 x,u16 y,u8 num,u8 len,u16 color)
{
  for(int i=0;i<len;i++)
  {
    LCD_ShowChineseBig(x+32*i,y,num+i,color);
  }
}
/********************************************************************
 * ���� : LCD_GetChar
 * ���� : ��������
 * ���� : x , y ��С
 * ��� : �� 
 ***********************************************************************/
u32 LCD_GetChar(u16 x, u16 y2, u8 size){
  u32 data = 0;
  u8 i;
  LCD_ShowString(x, y2, size, "                        ", WHITE);
  for (i = 1; i < 8; i++){
    while (True){
      if (MAIN_KEY.keysign){
        MAIN_KEY.keysign = 0;
        break;
      }
    }
    if (MAIN_KEY.keycode == 0 and data == 0){
      i = 0;
    }
    else if (MAIN_KEY.keycode < 10){
      data *= 10;
      data += MAIN_KEY.keycode;
      LCD_ShowChar(x, y2, '0' + MAIN_KEY.keycode, size, WHITE);
      x += size / 2;
    }
    else if (MAIN_KEY.keycode == 15){
      break;
    }
    else{
      i--;
    }

  }
  sprintf(LCD_BUFFER, "GET_DATA:%d", data);
  LCD_ShowString(x, y2, size, LCD_BUFFER, BLUE);
  return data;
}

/********************************************************************
 * ���� : LCD_ShowChar
 * ���� : ��ʾ1608�ַ� 
 * ���� :  x y �ַ� ��С ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowChar_DMA(u16 x, u16 y2, u8 num, u8 size, u16 color){
  u8 a0 = 1;
  u8 temp, t1, t;
  u16 color_2 = POINT_COLOR;
  num = num - ' ';
  LCD_SetCursor_DMA(x, y2); //���ù��λ��
  LCD_WriteRAM_Prepare_DMA(); //��ʼд��GRAM
  for (t = size - 1; t > 0; t--){
    temp = asc3_1608[num][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        POINT_COLOR = color;
      }
      else{
        POINT_COLOR = BACK_COLOR;
      }LCD_WRITE_DATA_DMA(POINT_COLOR);
      temp <<= 1;
    }
    if (((a0 == 1) &(t == 1)) == 1){
      t++;
      a0 = 0;
    }
  }
  POINT_COLOR = color_2;
}

/********************************************************************
 * ���� : LCD_ShowCharBig_DMA
 * ���� : ��ʾ3216�ַ�
 * ���� :  x y  ��С �ַ�λ��asc_3216�����е�λ�� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowCharBig_DMA(u16 x, u16 y, u8 num, u16 color){
  int temp, t1, t;
  u16 color_2 = POINT_COLOR;
  LCD_SetCursor_DMA(x, y + 16); //���ù��λ��
  LCD_WriteRAM_Prepare_DMA(); //��ʼд��GRAM
  for (t = 15; t >= 0; t--){
    temp = asc_3216[num - ' '][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        POINT_COLOR = color;
      }
      else{
        POINT_COLOR = BACK_COLOR;
      }LCD_WRITE_DATA_DMA(POINT_COLOR);
      temp <<= 1;
    }
  }
  LCD_SetCursor_DMA(x, y); //���ù��λ��
  LCD_WriteRAM_Prepare_DMA(); //��ʼд��GRAM
  for (t = 31; t >= 16; t--){
    temp = asc_3216[num - ' '][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        POINT_COLOR = color;
      }
      else{
        POINT_COLOR = BACK_COLOR;
      }LCD_WRITE_DATA_DMA(POINT_COLOR);
      temp <<= 1;
    }
  }
  LCD_SetCursor_DMA(x + 8, y + 16); //���ù��λ��
  LCD_WriteRAM_Prepare_DMA(); //��ʼд��GRAM
  for (t = 47; t >= 32; t--){
    temp = asc_3216[num - ' '][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        POINT_COLOR = color;
      }
      else{
        POINT_COLOR = BACK_COLOR;
      }LCD_WRITE_DATA_DMA(POINT_COLOR);
      temp <<= 1;
    }
  }
  LCD_SetCursor_DMA(x + 8, y); //���ù��λ��
  LCD_WriteRAM_Prepare_DMA(); //��ʼд��GRAM
  for (t = 63; t >= 48; t--){
    temp = asc_3216[num - ' '][t]; //����1608����
    for (t1 = 0; t1 < 8; t1++){
      if (temp &0x80){
        POINT_COLOR = color;
      }
      else{
        POINT_COLOR = BACK_COLOR;
      }LCD_WRITE_DATA_DMA(POINT_COLOR);
      temp <<= 1;
    }
  }
  POINT_COLOR = color_2;
}

/********************************************************************
 * ���� : LCD_ShowStringBig_DMA
 * ���� : ��ʾ3216�ַ���
 * ���� :  x y  ��С ���� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowStringBig_DMA(u16 x, u16 y, char* p, u16 color){
  while ((*p <= '~') && (*p >= ' ')){
    //�ж��ǲ��ǷǷ��ַ�!
    LCD_ShowCharBig_DMA(x, y, * p, color);
    x += 16;
    p++;
  }
}

/********************************************************************
 * ���� : LCD_ShowNumBig_DMA
 * ���� : ��ʾ3216����
 * ���� :  x y  ��С ���� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowNumBig_DMA(u16 x, u16 y, u32 num, u16 color){
  char temp[20], len = 0, len_max, i, p = 0;
  while (num){
    temp[len] = num % 10;
    num /= 10;
    len++;
    if ((len - p) % 3 == 0){
      temp[len] = 10;
      len++;
      p++;
    }
  }
  if (len){
    len_max = len;
    len--;
  }
  else{
    len_max = 1;
    temp[0] = 0;
  }
  if (temp[len] == 10){
    len--;
    len_max--;
  }
  for (i = 0; i < len_max; i++){
    LCD_ShowCharBig_DMA(x + i * 16, y, temp[len] + '0', color);
    len--;
  }
}

/********************************************************************
 * ���� : LCD_ShowNumBig_L_DMA
 * ���� : �Ҷ��� ��ʾ3216����
 * ���� :  x��ʼ x����λ��  y  ��С ���� ��ɫ
 * ��� : �� 
 ***********************************************************************/
void LCD_ShowNumBig_L_DMA(u16 x, u16 x_end, u16 y, u32 num, u16 color){
  int temp[20], len = 0, len_max, i, p = 0;
  while (num){
    temp[len] = num % 10+'0';
    num /= 10;
    len++;
    if ((len - p) % 3 == 0){
      temp[len] = ',';
      len++;
      p++;
    }
  }
  len_max = len;
  len--;
  if (len_max == 0){
    len = 0;
    temp[len] = '0';
    len_max = 1;
  }
  else if (temp[len] == ','){
    len--;
    len_max--;
  }
  if (len_max* 16 > (x - x_end)){
    return ; //���ȹ��� ����
  }

  else if (len_max){
    x -= len_max * 16;
    for (i = x_end; i < x; i += 16){
      LCD_ShowCharBig_DMA(i, y, ' ', color);
    }
    for (i = 0; i < len_max; i++){
      LCD_ShowCharBig_DMA(x + i * 16, y, temp[len], color);
      len--;
    }
  }
}
