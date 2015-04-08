#include "DDS.h"
#include "LCD.h"
u32 FREQ;
u16 VPP;
void Select_R(int L)
{
  static int count = 0;
  u16 temp_color = BACK_COLOR;
  /*上次删除*/
  //LCD_Select_Chinese();
  if(L) 
  {     
    count = ((count+1)%4);      
  }     
  else
  {
    count = ((count+4-1)%4);
  }
  BACK_COLOR = RED;
  //LCD_Select_Chinese();
  BACK_COLOR = temp_color;
  //LCD_Select_Wave();
}
void Freq_Service(int L)
{
  static int position=0;
  u16 temp_color = BACK_COLOR;
  u32 temp=10;
  u16 i,num;
  u16 len = Num_Len(FREQ);
  if(position >= len)
  {
    if(len > 0)
    {
      position = len-1;
    }
    else
    {
      position = 0;
    }
  }
  if(L == 0)
  {
    BACK_COLOR = RED;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(164-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowNumBig(164-position*16,0,num,YELLOW);
    }
  }
  /*左移*/
  else if(L == 1)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(164-position*16-16,0,num,YELLOW);
    } 
    else
    {
      LCD_ShowNumBig(164-position*16,0,num,YELLOW);
    }
    BACK_COLOR = RED;
    if(position+1 < len)
    {
      position += 1;
    }
    temp = 10;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(164-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowNumBig(164-position*16,0,num,YELLOW);
    }
  }
  /*右移*/
  else if(L == 2)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(164-position*16-16,0,num,YELLOW);
    } 
    else
    {
      LCD_ShowNumBig(164-position*16,0,num,YELLOW);
    }
    BACK_COLOR = RED;
    if(position > 0)
    {
      position -= 1;
    }
    temp = 10;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(164-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowNumBig(164-position*16,0,num,YELLOW);
    }
  }
  /*加数*/
  else if(L == 3)
  {
    temp = 1;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    if(temp + FREQ < 5000000)
    {
      FREQ += temp;
    }
    Freq_Service(0);
  }
  /*减数*/
  else if(L == 4)
  {
    temp = 1;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    if(temp < FREQ)
    {
      FREQ -= temp;
    }
  }
  /*清除关注*/
  else if(L == 5)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(164-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowNumBig(164-position*16,0,num,YELLOW);
    }
  }
  BACK_COLOR = temp_color;
}
void Vpp_Service(int L)
{
  static int position=0;
  u16 temp_color = BACK_COLOR;
  u32 temp=10;
  u16 i,num;
  u16 len = Num_Len(VPP);
  if(position >= len)
  {
    if(len > 0)
    {
      position = len-1;
    }
    else
    {
      position = 0;
    }
  }
  if(L == 0)
  {
    BACK_COLOR = RED;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(354-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowNumBig(354-position*16,0,num,YELLOW);
    }
  }
  /*左移*/
  else if(L == 1)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(354-position*16-16,0,num,YELLOW);
    } 
    else
    {
      LCD_ShowNumBig(354-position*16,0,num,YELLOW);
    }
    BACK_COLOR = RED;
    if(position+1 < len)
    {
      position += 1;
    }
    temp = 10;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(354-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowNumBig(354-position*16,0,num,YELLOW);
    }
  }
  /*右移*/
  else if(L == 2)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(354-position*16-16,0,num,YELLOW);
    } 
    else
    {
      LCD_ShowNumBig(354-position*16,0,num,YELLOW);
    }
    BACK_COLOR = RED;
    if(position > 0)
    {
      position -= 1;
    }
    temp = 10;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(354-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowNumBig(354-position*16,0,num,YELLOW);
    }
  }
  /*加数*/
  else if(L == 3)
  {
    temp = 1;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    if(temp + VPP <= 5000)
    {
      VPP += temp;
    }
    Vpp_Service(0);
  }
  /*减数*/
  else if(L == 4)
  {
    temp = 1;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    if(2040 <= VPP-temp)
    {
      VPP -= temp;
    }
  }
  /*清除关注*/
  else if(L == 5)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowNumBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowNumBig(354-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowNumBig(354-position*16,0,num,YELLOW);
    }
  }
  BACK_COLOR = temp_color;
}
