

/******************************************************************************

                  ��Ȩ���� (C) 2018, �촴��ũ�Ƽ�

 ******************************************************************************
  �ļ�   : zyf_protocol.h
  �汾   : ����
  ����   : LiCM
  ����   : 2018��02��24��
  ����   : �����ļ�
  ����   : ���Ʒ�MQTT�汾ͨ��Э��

  �޸ļ�¼:
  ����   : 2018��02��24��
    ����   : LiCM
    ����   : �����ļ�

******************************************************************************/



#ifndef __ZYF_PROTOCOL_
#define __ZYF_PROTOCOL_

#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_trace.h"


#define ZYF_PRO_DEBUG

//#define _USE_USER_DWQ_

#define SW_VERSION	"18041801"



#define BLE_CMD_HEAD 0xAA  // �� App ���͸�����
#define BLE_RESP_HEAD 0x55 // ���������͸� App ������Ϊ��֡ͷ��1 �ֽڣ�0x55

#define APP_SUCCESS 0x00
#define APP_FAIL 0x01
#define APP_ERROR_CMD 0x02



typedef enum
{
    BLE_CMD_AUTH_KEY  = 0X01,      
    BLE_CMD_AUTH_REQU = 0x02,           
    BLE_CMD_DEVRESET = 0XF0,
    BLE_ERROR_CMD = 0xFF,
#ifdef _USE_USER_DWQ_
	BLE_DWQ_CMD = 0xC0,
#endif
} BLECMD_CODE_t;


extern u8 ZYFRECMD[2][20];

extern volatile u8 ZYF_MQTT_CONFIG;






#ifdef _USE_USER_DWQ_


#define WZUD_L	 Ql_GPIO_SetLevel(PINNAME_SIM2_CLK, PINLEVEL_LOW)
#define WZUD_H	 Ql_GPIO_SetLevel(PINNAME_SIM2_CLK, PINLEVEL_HIGH)
#define WZINS_L	 Ql_GPIO_SetLevel(PINNAME_SIM2_DATA, PINLEVEL_LOW)
#define WZINS_H	 Ql_GPIO_SetLevel(PINNAME_SIM2_DATA, PINLEVEL_HIGH)
#define WZCS_L	 Ql_GPIO_SetLevel(PINNAME_SIM2_RST, PINLEVEL_LOW)
#define WZCS_H	 Ql_GPIO_SetLevel(PINNAME_SIM2_RST, PINLEVEL_HIGH)


#define M203C_SDDATAIN_L	 Ql_GPIO_SetLevel(PINNAME_SD_CMD, PINLEVEL_LOW)
#define M203C_SDDATAIN_H	 Ql_GPIO_SetLevel(PINNAME_SD_CMD, PINLEVEL_HIGH)


typedef struct 
{
	u32 sec;
	u8 per;
}mux_mode;


typedef struct 
{
	u8 mode;
	u8 per;
	mux_mode muxmode[8];
}_DJKZ_mode;


extern _DJKZ_mode djkzmode;

u8 WzAynsMode(u8 *pbuf);



#endif


s32 zyf_msg_register_data(u8 *data_ptr,void *user_data);
s32 zyf_msg_Sensor_data(u8 *data_ptr,void *user_data);
s32 zyf_decode_cmd(char *Payload,u16 len);

#endif


