/******************************************************************************

                  ��Ȩ���� (C) 2017, ��ͨ�Ƽ�

 ******************************************************************************
  �ļ�   : quectel_module_cfg.h
  �汾   : ����
  ����   : zhoujie
  ����   : 2017��12��7��
  �޸�   :
  ����   : ��Զ GPRS ģ�� TCPIP/SSL/GNSS ����

  �޸ļ�¼:
  ����   : 2017��12��7��
    ����   : zhoujie
    ����   : �����ļ�

******************************************************************************/


#ifndef __QUECTEL_MODULE_CFG_H
#define __QUECTEL_MODULE_CFG_H



#define WAIT_400MS (400)
#define WAIT_500MS (500)
#define WAIT_600MS (600)
#define WAIT_1S (1000)
#define WAIT_5S (5000)
#define WAIT_6S (6000)
#define WAIT_10S (10000)
#define WAIT_15S (15000)
#define WAIT_20S (20000)
#define WAIT_30S (30000)
#define WAIT_40S (40000)
#define WAIT_75S (75000)
#define WAIT_90S (90000)
#define WAIT_120S (120000)
#define WAIT_150S (150000)
#define AT_RESP_TIMEOUT_GENERAL WAIT_500MS
#define AT_RESP_TIMEOUT_5S WAIT_5S
#define AT_RESP_TIMEOUT_6S WAIT_6S
#define AT_RESP_TIMEOUT_10S WAIT_10S
#define AT_RESP_TIMEOUT_75S WAIT_75S
#define AT_RESP_TIMEOUT_150S WAIT_150S
#define AT_RESP_TIMEOUT_40S WAIT_40S
#define AT_CMD_LEN_MAX 128
#define AT_RESP_LEN_MAX 1024
#define QSCLK_M 2          // ��ʱ��ģʽ2,5�봮���������Զ���������
#define NORMAL_MODE 0      // ��͸��ģʽ
#define TRANSPARENT_MODE 1 // ͸��ģʽ
#define MODE_NOT_SET 2     // ����tcpӦ��ģʽ����
#define MULTIPLE_DISABLE 0 // ���ö�·����
#define MULTIPLE_ENABLE 1  // ���ö�·����
#define MULTIPLE_NOT_SET 2 // ���Զ�·��������


#define AT_DEBUG_VERBOSE 1

/*
��·����---ֻ�ܷ�͸��ģʽ
��·����-͸��/��͸������
*/
// TCP
// QIMODE ָ������
#define TCPIP_TRANSFER_MODE NORMAL_MODE
// QIMUX ָ������
#define TCPIP_SESSIONS_MODE MULTIPLE_DISABLE
// QIOPEN ָ������
#define TCPIP_TRANSFER_OPEN_MODE NORMAL_MODE
// SSL �ݲ�֧��͸��ģʽ
#define SSL_TRANSFER_MODE NORMAL_MODE
#define SSL_SESSIONS_MODE MULTIPLE_DISABLE
#define SSL_TRANSFER_OPEN_MODE NORMAL_MODE
// cert and key settings
#define SSL_USE_SERVER_CERT 1
#define SSL_USE_CLIENT_CERT 0
#define SSL_USE_CLIENT_KEY 0


#define GNSS_GET0_TIMES_MAX 3
#define GNSS_GET1_TIMES_MAX 3
#define GNSS_GET2_TIMES_MAX 3
#define GNSS_GET3_TIMES_MAX 3


#endif /* __QUECTEL_MODULE_CFG_H */
