
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   main.c
 *
 * Project:
 * --------
 *   OpenCPU
 *
 *
 * Usage:
 * ------
 *   Compile & Run:
 *
 *     Set "C_PREDEF=-D __ZFY_MQTT_APP__; CLOUD_SOLUTION =ZYF_MQTT_SOLUTION " in gcc_makefile file. And compile the 
 *     app using "make clean/new".
 *     Download image bin to module to run.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * 
 ****************************************************************************/

/******************************************************************************

                  ��Ȩ���� (C) 2018, �촴��ũ�Ƽ�

 ******************************************************************************
  �ļ�   : main.c
  �汾   : ����
  ����   : LiCM
  ����   : 2018��02��24��
  ����   : �����ļ�
  ����   : M203C3.0�����嵥�̰߳汾

  �޸ļ�¼:
  ����   : 2018��02��24��
    ����   : LiCM
    ����   : �����ļ�

******************************************************************************/


#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "iot203c_msg.h"
#include "uart.h"
#include "ql_timer.h"
#include "user_mqtt.h"
#include "sys.h"
#include "fota_main.h"
#include "fota.h"


/*!
 * @brief ������:��ʼ��BSP,��ɶ�ʱ�ɼ����ݲ��ҷ��͵����Ʒ�
 * \n
 *
 * @param taskId ��ϢͨѶID
 * @return NULL
 * \n
 * @see
 */

void proc_main_task(s32 taskId)
{
	ST_MSG msg;	
    M203C_BSP_init();
    SYS_Parameter_Init();
	Led_Timer_init(TIMER_ID_USER_START + LED_TIMER_ID, LED_TIMER_MS);
	aliot_Timer_init(TIMER_ID_USER_START + AIOT_TIMER_ID, 1000);
    while (1)
    {
        
        Ql_OS_GetMessage(&msg);
        switch (msg.message)
        {
	        case MSG_ID_RIL_READY:				
		        {
		            Ql_RIL_Initialize();				//�ȴ�RIL���ʼ�����,�ſ��Ե���AT CMDָ��,�û��������
		           	Ql_SleepEnable();  
		            SendMsg2KernelForBLEStart();		//������Ϣ��BLE����,����BLE����
		        }
			 break;
			case MSG_IOT_TASK_GET_SENSOR_DATA:				
		        {
		           //GetUserSensorData();					//��ȡ���贫��������
		        }
			 break;
			case MSG_IOT_TASK_START_MQTT:
				  Mqtt_InitConnect_Start();
				  Iotdata_Timer_init(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
			 break;
			case MSG_ID_FTP_RESULT_IND:
	            mprintf("\r\n<##### Restart FTP 3s later #####>\r\n");
	            Ql_Sleep(3000);
	            ftp_downfile_timer(FotaUpdate_Start_TmrId, NULL);
             break;
        	case MSG_ID_RESET_MODULE_REQ:
	            mprintf("\r\n<##### Restart the module... #####>\r\n");
	            Ql_Sleep(50);
	            Ql_Reset(0);
			 break;
	        default:
	            break;
        }
    }
}

/*!
 * @brief MQTT����:��ʼ������TCP/IP��Ȼ�������ӵ����Ʒ�,����MQTT����������,���ҿ����Զ���ɶ�������
 * \n
 *
 * @param taskId ��ϢͨѶID
 * @return NULL
 * \n
 * @see
 */


void user_mqtt_task(s32 TaskId)
{
    ST_MSG msg;
    while (1)
    {
        Ql_OS_GetMessage(&msg);
        switch (msg.message)
        {
        	 
        	 case MSG_IOT_TASK_START_BLE:
				  Ble_InitConnect_Start();
				  SendMsg2KernelForMqttStart();
				break;
			 case MSG_IOT_TASK_Get_ModuData:
				  GetModuleData();
				break;
			 case MSG_IOT_TASK_HeartDATA_MQTT:				
			 	   Iotdata_Timer_Stop(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
		           MqttPubUserHeartData();					//������������
		           Iotdata_Timer_Start(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
		        break;
			 case MSG_IOT_TASK_SENDDATA_MQTT:
			 	   Iotdata_Timer_Stop(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
		           MqttPubUserSensorData();					//�����������ݵ�������
		           Iotdata_Timer_Start(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
		        break;
			 case MSG_IOT_TASK_REDATA_MQTT:
				   MqttPubUserReSensorData();
				break;
			 case MSG_IOT_TASK_OTA_MQTT:
				   fota_app();
				break;
	        default:
	            break;

        }

    }
}


/*!
 * @brief ��������:��ֹϵͳ����
 * \n
 *
 * @param taskId ��ϢͨѶID
 * @return NULL
 * \n
 * @see
 */

void user_overlook_task(s32 TaskId)
{
    ST_MSG msg;
    while (1)
    {
        Ql_OS_GetMessage(&msg);
        switch (msg.message)
        {
	        default:
	            break;
        }

    }
}



