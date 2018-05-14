#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "ql_type.h"


#define MOTOR_NSLEEP_ON		Ql_GPIO_SetLevel(PINNAME_SIM2_CLK, PINLEVEL_HIGH)
#define MOTOR_NSLEEP_OFF	Ql_GPIO_SetLevel(PINNAME_SIM2_CLK, PINLEVEL_LOW)
#define MOTOR_REVERSE_ON		Ql_GPIO_SetLevel(PINNAME_SIM2_RST, PINLEVEL_HIGH)
#define MOTOR_REVERSE_OFF	Ql_GPIO_SetLevel(PINNAME_SIM2_RST, PINLEVEL_LOW)
#define MOTOR_FORWARD_ON		Ql_GPIO_SetLevel(PINNAME_SIM2_DATA, PINLEVEL_HIGH)
#define MOTOR_FORWARD_OFF	Ql_GPIO_SetLevel(PINNAME_SIM2_DATA, PINLEVEL_LOW)

#define GPIO_OUT_UNLOCKED_SENSOR_ON		Ql_GPIO_SetLevel(PINNAME_PCM_CLK, PINLEVEL_HIGH)
#define GPIO_OUT_UNLOCKED_SENSOR_OFF	Ql_GPIO_SetLevel(PINNAME_PCM_CLK, PINLEVEL_LOW)
#define GPIO_OUT_LOCKED_SENSOR_ON		Ql_GPIO_SetLevel(PINNAME_PCM_IN, PINLEVEL_HIGH)
#define GPIO_OUT_LOCKED_SENSOR_OFF		Ql_GPIO_SetLevel(PINNAME_PCM_IN, PINLEVEL_LOW)
#define GPIO_IN_UNLOCKED_SENSOR			Ql_GPIO_GetLevel(PINNAME_PCM_OUT)		//in1
#define GPIO_IN_LOCKED_SENSOR			Ql_GPIO_GetLevel(PINNAME_PCM_SYNC)		//in2

#define RETURN_TIMEOUT_SEC 10 //�ȴ�����������ʱ����
#define RENT_TIMEOUT_SEC 10 //�⳵��������������
#define SCANBLE_TIMEOUTSEC_NOSTATION 10 //����ʱ����ⲻ������׮��ɨ����
#define SCANBLE_TIMEOUTSEC_INSTATION 10 //�����ǣ���⵽����׮��ɨ����
#define SCANBLE_TIMEOUTSEC_WAITLOCK  10//�����ȴ�����ʱ�䣬����ʱ�俪������ɨ�裻

// ��״̬����
typedef enum
{
    LOCK_STATUS_UNLOCKED = 0x51,     ///!<���Ѵ�
    LOCK_STATUS_UNLOCKED_ERR = 0x61, ///!<��δ��ȷ��
    LOCK_STATUS_LOCKED = 0xA1,       ///!<���ѹر�(������)
    LOCK_STATUS_LOCKABLE = 0xB1,     ///!<��δ��ȷ�رգ����ڿ���״̬
    LOCK_CLOSE_PILE = 0xC1,          ///!<�ڵ���׮��Χ������ todo
    LOCK_STATUS_INIT = 0x01,         // ��ʼ����״̬
    LOCK_REVISE_LOCK = 0x71,         ///!<����
} LOCK_STATUS_t;

// �����������
typedef enum
{
    LOCK_CTRL_CMD_CLEAR = 0,   ///!<�޲��� �������������
    LOCK_CTRL_CMD_UNLOCK,      ///!<���Ѵ�
    LOCK_CTRL_CMD_LOCKABLE,     ///!<��δ��ȷ�رգ����ڿ���״̬
} LOCK_CTRL_CMD_t;

typedef struct
{
    u8 cCmd;    // ����
    u8 cTime;   // ��ʱ��ʹ�ñ�־
    u8 cStatus; // ��״̬
    u8 cStatusCopy; // ��״̬����
    u8 nCount; // �����������
    u16 nUnLockTime; // ���ֿ�����ʱ��,����ʱ��0���±���
    u16 nScanbleTime;
    u16 nWaitlockTime;//�����ȴ�����ʱ��
} motor_ctrl_t;


extern motor_ctrl_t motor_ctrl;



#endif



