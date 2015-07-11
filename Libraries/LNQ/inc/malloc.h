#ifndef __MALLOC_H
#define __MALLOC_H
#include "common.h"
#include "stdbool.h"  
#ifndef NULL
#define NULL 0
#endif
#define MEM_BLOCK_SIZE   32          //�ڴ���СΪ32�ֽ�
#define MAX_MEM_SIZE   2*1024       //�������ڴ� 1K
#define MEM_ALLOC_TABLE_SIZE (MAX_MEM_SIZE/MEM_BLOCK_SIZE) //�ڴ����С
//�ڴ����������
struct _m_mallco_dev
{
void (*init)(void);     //��ʼ��
u8 (*perused)(void);         //�ڴ�ʹ����
u8  membase[MAX_MEM_SIZE];   //�ڴ��
u8 memmap[MEM_ALLOC_TABLE_SIZE];  //�ڴ����״̬��
bool  memrdy;        //�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;  //��mallco.c���涨��
void mymemset(void *s,u8 c,u32 count);  //�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ�  
void mem_init(void);      //�ڴ������ʼ������(��/�ڲ�����)
u32 mem_malloc(u32 size);     //�ڴ����(�ڲ�����)
u8 mem_free(u32 offset);     //�ڴ��ͷ�(�ڲ�����)
u8 mem_perused(void);      //����ڴ�ʹ����(��/�ڲ�����) 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(void *ptr);       //�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u32 size);     //�ڴ����(�ⲿ����)
void *myrealloc(void *ptr,u32 size);  //���·����ڴ�(�ⲿ����)
   
#endif