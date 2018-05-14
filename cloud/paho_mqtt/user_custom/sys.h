#ifndef __SYS_H__
#define __SYS_H__


#include "ql_gpio.h"
#include "ql_adc.h"



typedef enum
{
    MSG_LOCK_CTRL_SERVICE_NONE = 0x100,  		//Ԥ��    
    MSG_LOCK_CTRL_SERVICE_RENT ,     			//�⳵
    MSG_RD_ALIYUN_MQTT_RE,						//����������������
    MSG_LOCK_CTRL_SERVICE_RETURN,       		//����
    MSG_SEND_ALIYUN_MQTT_HRET, 				//����������
    MSG_SEND_ALIYUN_IOTSTA, 		//����ҵ������ 
    MSG_SEND_ALIYUN_INIT, 		//��ʼ������������
} MSG_LOCK_CTRL_SERVICE_TYPE_T;



typedef struct 
{
	u32 HandInter;			//��������ʱ����
	u32 Interval;				//�����Զ��ϴ�ʱ����
	u8 SN[16];				//SN
	u8 snuser;				//SN�Ƿ��Ѿ�����
	u8 saveflag;
	u8 fotaflag;				//������־
	u8 fotaaddr[200];			//�̼���ַ����Ϣ
	u32 SysTime;
	u8 CallMode;				//Ԥ��
}_system_setings;


extern volatile u8 SocketBufRdBit;




extern u8 Need_Lbs_Data;

extern _system_setings	systemset;






void GpsDelay(u16 delaytime);
void LbsDelay(u16 delaytime);
void power_drv_init(void);
void LcdPin_init(void);
void Delayus(u32 data);
void SysSleep(void);
void  LoadDefualtCfg(void);
void SYS_Parameter_Init(void);
void ShowSysInfo(void);
void InternetsetSN_Analysis(u8 *snuf,u8 len);
void Callback_ChageInit(Enum_ADCPin adcPin, u32 adcValue, void *customParam);
void SysWakeUp(void);
void ADC_Program(void);
void DrvBell_Ring_R(u8 num);
void M203C_BSP_init(void);
#endif

