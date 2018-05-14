/******************************************************************************

                  ��Ȩ���� (C) 2018, �촴��ũ�Ƽ�

 ******************************************************************************
  �ļ�   : module_data.h
  �汾   : ����
  ����   : LiCM
  ����   : 2018��03��8��
  ����   : �����ļ�
  ����   : ��ȡģ���ڲ�����API

  �޸ļ�¼:
  ����   : 2018��03��8��
    ����   : LiCM
    ����   : �����ļ�

******************************************************************************/

#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"


#ifndef __MODULE_DATA__
#define __MODULE_DATA__


//#define MODULE_DATA_DEBUG				//ģ���ڲ����ݵ��Կ���


typedef struct
{
	u8  creg;			//����״̬
	u8  cgreg;			//GPRS״̬
	u8  csq;			//�ź�ǿ��
	u8  bvol;			//��ѹ�ٷֱ�
	u16 mvol;		//ģ���ѹ
} Module_Data_T;

extern Module_Data_T module_data;

#endif

