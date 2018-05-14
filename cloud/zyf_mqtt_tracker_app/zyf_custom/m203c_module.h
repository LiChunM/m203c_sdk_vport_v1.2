/******************************************************************************

                  ��Ȩ���� (C) 2017, ��ͨ�Ƽ�

 ******************************************************************************
  �ļ�   : quectel_module.h
  �汾   : ����
  ����   : zhoujie
  ����   : 2017��7��28��
  �޸�   :
  ����   : quectel_module.c ��ͷ�ļ�
  �����б�   :
  �޸�   :
  ����   : 2017��7��28��
    ����   : zhoujie
    ����   : �����ļ�

******************************************************************************/
#ifndef __QUECTEL_MODULE_H__
#define __QUECTEL_MODULE_H__

#include <inttypes.h>
#include <stdint.h>
#include "ql_type.h"
#include <stdlib.h>



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
enum AT_ERROR_CODE
{
    AT_SUCCESS = 0x00,     // ����������,����ɹ�
    AT_ERROR_SEND_CMD,     // ���ͳ���
    AT_ERROR_SEND_CR,      // ���ͻس�������
    AT_ERROR_RESP_TIMEOUT, // ������Ӧ��ʱ
    AT_ERROR_PARSE_FAIL,   // ����������,���ʧ��
    AT_URC_DISCONNECT,     // ����������,���ʧ�� failed times limit!
    AT_URC_TCPIP_RECV,     // TCP �յ�����
    AT_URC_SSL_RECV,       // SSL �յ�����
};
typedef struct
{
    // 16/06/28,15:42:55+32
    char year[2];
    char se1;
    char month[2];
    char se2;
    char day[2];
    char se3;
    char hour[2];
    char re1;
    char minutes[2];
    char re2;
    char secons[2];
    char zone[3]; // ���õ���ʱ��� GMT ʱ��֮��Ĳ������ʾ�����ķ�֮һСʱ��ʽ����ʾ����Χ-47~+48��
} asc_time_str_t;
typedef struct
{
    int32_t act_length;   // QIRD ָ���ȡ������ʵ���ݳ���
    int32_t total_length; // ��ǰ��������δ��ȡ�����ֽ�������
    uint8_t* data_ptr;    // ����ָ��
} tcpip_recv_t;

typedef struct
{
    uint32_t file_size; // total size
    uint32_t fhandle;   // file handle
    uint32_t blk_n;
    uint32_t blk_actual_size; // ��ȡ�ĵ�ǰ���С
    uint8_t* blk_data_ptr;
} file_access_parm_t;

typedef struct 
{
    uint8_t* addr;
    uint8_t* port;
    uint8_t* user;
    uint8_t* passwd;
    uint8_t* path;
    uint8_t* file_name;
} ftp_download_t;
extern int32_t GNSS_AGPS_get0();
extern int32_t GNSS_AGPS_get1();
extern int32_t GNSS_AGPS_get2();
extern int32_t GNSS_AGPS_get3();
extern int32_t GNSS_AGPS_get4();
extern int32_t GSM_ATV_Test();
extern int32_t GSM_AT_sync(uint32_t option);
extern int32_t GSM_config_base();
extern int32_t GSM_deactivate_pdp();
extern int32_t GSM_GPRS_activate_pdp();
extern int32_t GSM_GPRS_prepare(uint8_t * po_imei, uint8_t * po_ccid, uint8_t * po_imsi);
extern int32_t GSM_SSL_close();
extern int32_t GSM_SSL_config();
extern int32_t GSM_SSL_delete_cert();
extern int32_t GSM_SSL_open(const char* host, uint16_t port);
extern int32_t GSM_SSL_retrieve(char* buf, uint32_t len, uint32_t timeout_ms);
extern int32_t GSM_SSL_transfer_mode_set();
extern int32_t GSM_SSL_transmit(const char* buf, uint32_t len, uint32_t timeout_ms);
extern int32_t GSM_SSL_upload_CACert(const char* ca_cert, u32 ca_crt_len);
extern int32_t GSM_SSL_upload_ClientCert(const char* ca_cert, u32 ca_crt_len);
extern int32_t GSM_SSL_upload_ClientKey(const char* ca_cert, u32 ca_crt_len);
extern int32_t GSM_TCP_close();
extern int32_t GSM_TCP_open(const char* host, uint16_t port);
extern int32_t GSM_TCP_retrieve(char* buf, uint32_t len, uint32_t timeout_ms);
extern int32_t GSM_TCP_transfer_mode_set();
extern int32_t GSM_TCP_transmit(const char* buf, uint32_t len, uint32_t timeout_ms);
extern void OnURCHandler_QSSLOPEN(const char* strURC, void* reserved);
extern void OnURCHandler_QISEND(const char* strURC, void* reserved);
extern void OnURCHandler_QSSLURC(const char* strURC, void* reserved);
extern void OnURCHandler_QSECWRITE(const char* strURC, void* reserved);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __QUECTEL_MODULE_H__ */
