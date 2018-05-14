/******************************************************************************

                  ��Ȩ���� (C) 2018, �촴��ũ�Ƽ�

 ******************************************************************************
  �ļ�   : MQTT_interface.c
  �汾   : ����
  ����   : LuanYang
  ����   : 2018��03��5��
  ����   : �����ļ�
  ����   : M203C3.0�����嵥�̰߳汾

  �޸ļ�¼:
  ����   : 2018��03��5��
    ����   : LuanYang
    ����   : �����ļ�

******************************************************************************/

#include "cJSON.h"
#include <math.h>
#include "ql_common.h"
#include "ql_system.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_timer.h"
#include "ql_stdlib.h"
#include "ql_uart.h"
#include "ql_memory.h"
#include "ql_error.h"

#include "ql_stdlib.h"
#include "uart.h"
#include "jsonAnaysis.h"


#define JSON_ANALYSIS_DEBUG



#ifdef JSON_ANALYSIS_DEBUG

    #define JSON_ANALYSIS_debug(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
#else
   	#define JSON_ANALYSIS_debug(format, ...)
#endif





typedef struct
{
	char sn[50];
	char timestamp[50];
	char cycle[50];
	char num[50];
}BLE_LIST_HEAD;


typedef struct BLE_LIST
{
	struct BLE_LIST *next, *previous;
	BLE_LIST_HEAD *ble_list;
	BLE_INFO_TYPE Ble_info;

}JSON_BLE_CMD_LINK_NODE;






/*!
 * @brief ����ָ�봫�ݽ����ڱ��ļ��ڲ�
 * \n
 *
 * @param p ��Ҫ���д��ݵ�ָ��
 * @return ������ķǿ�ָ��
 * \n
 * @see
 */
JSON_BLE_CMD_LINK_NODE * transfer_the_point(JSON_BLE_CMD_LINK_NODE * p)
{
	static JSON_BLE_CMD_LINK_NODE * pHead = NULL;
	if(p!=NULL)
	{
		pHead = p;
	}
	return pHead;
}



/*!
 * @brief ����һ��������Ϣ����
 * \n
 *
 * @param NULL
 * @return -1:����ʧ�ܿ��ܵ�Զ�����ڴ����ʧ�ܡ�0:��������ɹ�
 * \n
 * @see
 */
int CreateBleInfoList(void)
{
	JSON_BLE_CMD_LINK_NODE *ble_link_haed;
	static BLE_LIST_HEAD ble_list_head;
	ble_link_haed=(JSON_BLE_CMD_LINK_NODE*)Ql_MEM_Alloc(sizeof(JSON_BLE_CMD_LINK_NODE));
	if(ble_link_haed == NULL)
		{
		return (-1);
		JSON_ANALYSIS_debug("\r\n Create Ble InfoList failed \r\n");
		}
	ble_link_haed->next= NULL;
	ble_link_haed->previous = NULL;
	ble_link_haed->ble_list = &ble_list_head;

	transfer_the_point(ble_link_haed); //��������ͷָ��
	return (0);

}

/*!
 * @brief ����������Ϣ�Ĳ���
 * \n
 *
 * @param pSN:�ռ�������Ϣָ������Ҫ�����SN���ָ��
 * @param pTimestamp:�ռ�������Ϣָ������Ҫ�����ʱ�����ָ��
 * @param pCycle:�ռ�������Ϣָ������Ҫ�����pCycle��ָ��
 * @param pNum:�ռ�������Ϣָ������Ҫ�����pNum��ָ�� 
 * @return -2:����ʧ��,���ܵ�ԭ������CreateBleInfoList֮ǰ�����˸ú�����0���óɹ�
 * \n
 * @see
 */
int SetBleInfoList(char *pSN , char *pTimestamp , char *pCycle, char *pNum)
{
	JSON_BLE_CMD_LINK_NODE * ble_link_haed_pro;

	ble_link_haed_pro=transfer_the_point(NULL);  //��ȡ����ͷָ��
	if(ble_link_haed_pro == NULL)
	{
		return (-2);
		JSON_ANALYSIS_debug("\r\n BleLink is NULL  \r\n");
	}

	Ql_strcpy( (ble_link_haed_pro->ble_list->sn) , pSN);
	
	Ql_strcpy( (ble_link_haed_pro->ble_list->timestamp) , pTimestamp);

	Ql_strcpy( (ble_link_haed_pro->ble_list->cycle) ,pCycle);
	
	Ql_strcpy( (ble_link_haed_pro->ble_list->num) , pNum);	

	return (0);
	
}



/*!
 * @brief ��������Ϣ�����һ��������Ϣ
 * \n
 *
 * @param name:��������ָ��
 * @param mac:������mac��ַ��ָ��
 * @param rssi:�����ź������ȵ�ָ��
 * @param RAWdata:����scan_rsp��������
 * @return -2:����ʧ��,���ܵ�ԭ������CreateBleInfoList֮ǰ�����˸ú�����0���óɹ�
 * \n
 * @see
 */
int AddBleInfoToList(char *name , char *mac , char *rssi, char *RAWdata )
{
	static JSON_BLE_CMD_LINK_NODE * ble_link_head;
	JSON_BLE_CMD_LINK_NODE * ble_link_haed_pro;
	char *temp;

	ble_link_haed_pro=transfer_the_point(NULL); //��ȡ����ͷָ��
	if(ble_link_haed_pro == NULL)
	{
		JSON_ANALYSIS_debug("\r\n BleLink is NULL  \r\n");
		return (-2);
	}

	

	
	ble_link_head=(JSON_BLE_CMD_LINK_NODE*)Ql_MEM_Alloc(sizeof(JSON_BLE_CMD_LINK_NODE));
	if(ble_link_head==NULL)
	{
		JSON_ANALYSIS_debug("\r\n Memory allocation failure  \r\n");
		return (-1);
	}
//	ble_link_head->Ble_info.name = (char*)Ql_MEM_Alloc(Ql_strlen(name)+1);
//	ble_link_head->Ble_info.mac = (char*)Ql_MEM_Alloc(Ql_strlen(mac)+1);
//	ble_link_head->Ble_info.mac = (char*)Ql_MEM_Alloc(Ql_strlen(mac)+1);

	while(ble_link_haed_pro->next != NULL)
	{
		ble_link_haed_pro = ble_link_haed_pro->next;
	}

	
	ble_link_haed_pro->next = ble_link_head;
	ble_link_head->next = NULL;
	ble_link_head->previous = ble_link_haed_pro;
	ble_link_head->ble_list = NULL;
	
	Ql_strcpy( (ble_link_head->Ble_info.name) ,name);
	
	Ql_strcpy( (ble_link_head->Ble_info.mac) ,mac);

	Ql_strcpy( (ble_link_head->Ble_info.rssi) ,rssi);
	
	Ql_strcpy( (ble_link_head->Ble_info.RAWdata) ,RAWdata);	

	return (0);
	
}




/*!
 * @brief ��������Ϣ��ɾ��һ��������Ϣ
 * \n
 *
 * @param mac:��Ҫɾ����������Ϣ�ж�Ӧ��mac��ַ
 * @return -2:����ʧ��,���ܵ�ԭ������CreateBleInfoList֮ǰ�����˸ú�����-1:û��ƥ�������; 0:ɾ���ɹ�
 * \n
 * @see
 */
int DeleteBleInfoP(void)
{
	JSON_BLE_CMD_LINK_NODE * ble_link_haed_pro;

	ble_link_haed_pro=transfer_the_point(NULL); //��ȡ����ͷָ��
	if(ble_link_haed_pro == NULL)
	{
		JSON_ANALYSIS_debug("\r\n BleLink is NULL  \r\n");
		return (-2);
	}
	
	Ql_MEM_Free(ble_link_haed_pro);

	return 0;
	
}

int DeleteBleInfoToList(const char * mac )
{
	JSON_BLE_CMD_LINK_NODE * ble_link_head;
	JSON_BLE_CMD_LINK_NODE * ble_link_haed_pro;

	ble_link_haed_pro=transfer_the_point(NULL); //��ȡ����ͷָ��
	if(ble_link_haed_pro == NULL)
	{
		JSON_ANALYSIS_debug("\r\n BleLink is NULL  \r\n");
		return (-2);
	}
	ble_link_haed_pro = ble_link_haed_pro->next;
	
	while( Ql_strcmp(ble_link_haed_pro->Ble_info.mac , mac) != 0 )
	{
		if(ble_link_haed_pro->next == NULL)
		{
			JSON_ANALYSIS_debug("\r\n Not found match mac  \r\n");
			return (-1);
		}
		ble_link_haed_pro = ble_link_haed_pro->next;
	}

	
	
	if(ble_link_haed_pro->next != NULL)
	{
		ble_link_haed_pro->previous->next = ble_link_haed_pro->next;
		ble_link_haed_pro->next->previous = ble_link_haed_pro->previous;
	}else
	{
		ble_link_haed_pro->previous->next = NULL;		
	}
    
	
	Ql_MEM_Free(ble_link_haed_pro);

	return 0;
	
}


/*!
 * @brief �����������е���Ϣת��Ϊjson��
 * \n
 *
 * @param NULL
 * @return ����ת�����json����ָ�룬�������ΪNULL�����ת��ʧ�ܣ����ܵ�ԭ������CreateBleInfoList֮ǰ�����˸ú���
 *		   �����json����ʹ����ɺ���Ҫ����FreeJsonString()���ͷ�json����ռ���ڴ�
 * \n
 * @see
 */
char * BleInfoToJson(void)
{
	char *json_ret;
	JSON_BLE_CMD_LINK_NODE * ble_link_head;
	JSON_BLE_CMD_LINK_NODE * ble_link_haed_pro;
	
	cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();
	cJSON *rows,*row;

	ble_link_haed_pro=transfer_the_point(NULL); //��ȡ����ͷָ��
	if(ble_link_haed_pro == NULL)
	{
		JSON_ANALYSIS_debug("\r\n BleLink is NULL  \r\n");
		return (NULL);
	}
			
	cJSON_AddItemToObject(root, "type", cJSON_CreateString("ble_list"));
	cJSON_AddItemToObject(root, "sn", cJSON_CreateString(ble_link_haed_pro->ble_list->sn ));
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(ble_link_haed_pro->ble_list->timestamp ));
	cJSON_AddItemToObject(root, "cycle", cJSON_CreateString(ble_link_haed_pro->ble_list->cycle ));
	cJSON_AddItemToObject(root, "num", cJSON_CreateString(ble_link_haed_pro->ble_list->num ));
	cJSON_AddItemToObject(root, "list", rows=cJSON_CreateArray());

	ble_link_head = ble_link_haed_pro->next;
	
	while(ble_link_head!= NULL)
	{
		cJSON_AddItemToArray(rows, row = cJSON_CreateObject());
		cJSON_AddItemToObject(row, "name", cJSON_CreateString(ble_link_head->Ble_info.name));  
		cJSON_AddItemToObject(row, "mac", cJSON_CreateString(ble_link_head->Ble_info.mac));  
		cJSON_AddItemToObject(row, "rssi", cJSON_CreateString(ble_link_head->Ble_info.rssi));
		//cJSON_AddItemToObject(row, "RAWdata", cJSON_CreateString(ble_link_head->Ble_info.RAWdata));		
		ble_link_head = ble_link_head->next;
		
	}
	
	json_ret=cJSON_Print(root);
	json_ret=(char *)Ql_MEM_Alloc(Ql_strlen(json_ret)+1);
	json_ret=Ql_strcpy(json_ret,cJSON_Print(root));
	cJSON_Delete(root);

	return (json_ret);
	
}


/*!
 * @brief ��������Ϣת��Ϊjson��
 * \n
 *
 * @param 		 mac :mac��ַ���ָ��
 * @param   timestamp:ʱ�����ָ��
 * @param bat_voltage:��ص�ѹ�ַ�����ָ��
 * @param bat_current:��ص����ַ�����ָ��
 * @param pwr_voltage:��Դ��ѹ�ַ�����ָ��
 * @param pwr_current:��Դ�����ַ�����ָ��
 * @return  �����json����ʹ����ɺ���Ҫ����FreeJsonString()���ͷ�json����ռ���ڴ�  
 * \n
 * @see
 */
char *BleHeartbeatToJson(char * mac,char *timestamp,char *bat_voltage,char *bat_current,char *pwr_voltage,char *pwr_current)
{
	cJSON * root =  cJSON_CreateObject();
	char *json_ret,*buf_String;
	short len;
	
	cJSON_AddItemToObject(root, "type", cJSON_CreateString("ble_heartbeat"));
	cJSON_AddItemToObject(root, "mac", cJSON_CreateString(mac));
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(timestamp));
	cJSON_AddItemToObject(root, "batteryVoltage", cJSON_CreateString(bat_voltage));
	cJSON_AddItemToObject(root, "batteryCurrent", cJSON_CreateString(bat_current));
	cJSON_AddItemToObject(root, "powerVoltage", cJSON_CreateString(pwr_voltage));
	cJSON_AddItemToObject(root, "powerCurrent", cJSON_CreateString(pwr_current));
	
	buf_String=cJSON_Print(root);
	len = Ql_strlen(buf_String);
	json_ret=(char*)Ql_MEM_Alloc(len+1);
	if(json_ret != NULL)
	{
			Ql_strcpy(json_ret,buf_String);
	}else
	{
		JSON_ANALYSIS_debug("\r\n Memory allocation failure  \r\n");
	}
	cJSON_Delete(root);

	
	return (json_ret);		
	
}


/*!
 * @brief �·�����������豸�ظ������������
 * \n
 *
 * @param type :�ͽ����յ���ָ���type���ݵ�ָ��
 * @param sn   :sn��ָ��
 * @return  �����json����ʹ����ɺ���Ҫ����FreeJsonString()���ͷ�json����ռ���ڴ�  
 * \n
 * @see
 */
char *AckForJsonCmd(char *type, char *sn)
{
	cJSON * root =  cJSON_CreateObject();
	char *json_ret;
	
	cJSON_AddItemToObject(root, "type", cJSON_CreateString(type));
	cJSON_AddItemToObject(root, "sn", cJSON_CreateString(sn));
	
	json_ret=cJSON_Print(root);
	json_ret=(char *)Ql_MEM_Alloc(Ql_strlen(json_ret)+1);
	json_ret=Ql_strcpy(json_ret,cJSON_Print(root));
	cJSON_Delete(root);
	
	return (json_ret);
	
}



/*!
 * @brief �ͷ�ת����json�ڴ�
 * \n
 *
 * @param pJsonStr :��Ҫɾ����json����ָ��
 * @return void
 * \n
 * @see
 */
void FreeJsonString(char *pJsonStr)
{
	if(pJsonStr != NULL)
	{
		Ql_MEM_Free(pJsonStr);
		JSON_ANALYSIS_debug("\r\n Del successful \r\n");
	}
	else
	{
		JSON_ANALYSIS_debug("\r\n Del Error \r\n");
	}
}

/*!
 * @brief ����json����������� SJON_CMD_TYPE����ָ�뷵��
 * \n
 * @param  Payload :��Ҫ������json����ָ��
 * @return SJON_CMD_TYPE :������� ���ݽ���������ָ������ȷ�������������Ǵ�	StringType��BleSleepType��BleMqttSetType
 *							�е���һ����á���������һ���ַ������	StringType�ж�ȡ��
 *							���ָ���ǡ���������ʱ�������� BleSleepType�ж�ȡ��
 *							���ָ���ǡ�1.15�豸mqttͨѶ�������á����BleMqttSetType�ж�ȡ
 * \n
 * @see
 */
SJON_CMD_TYPE*JsonCmdAnalysis(char *Payload)
{
	cJSON *root , *value , *type;
	
	unsigned short index=0;
	static SJON_CMD_TYPE RetResult;
	static char *cmd[11]={
					"ble_power",
					"ble_search_interval",
					"ble_search_window",
					"ble_report_interval",
					"ble_battery_voltage",
					"ble_battery_current",
					"ble_power_max_voltage",
					"ble_power_max_current",
					"ble_sleep",
					"ble_reset",
					"ble_mqtt"
					};


					
    root = cJSON_Parse(Payload);  
    if (!root)  
    {  
        JSON_ANALYSIS_debug("Error before: [%s]\n",cJSON_GetErrorPtr());
		RetResult.cmd=CMD_Error;
		goto END;
    }
	else
	{
		
		 type = cJSON_GetObjectItem( root , "type");
		 
		 if(type->type == cJSON_String)
		 {
		 	mprintf("%s\r\n",type->valuestring);
		 	while(0!=Ql_strcmp(type->valuestring, cmd[index]))
	 		{
	 			
	 			if(index >=11)
	 			{
	 				RetResult.cmd=CMD_Error;
					goto END;
	 			}
				
				index++;
			
	 		}
			RetResult.cmd = index;
			
			JSON_ANALYSIS_debug("The cmd is %d\r\n",index);
			
			switch(index)
			{
				case SET_BLE_PRW:
				case SET_BLE_SEARCH_INTERVAL:
				case SET_BLE_SEARCH_WINDOW:
				case SET_BLE_REPORT_INTERVAL:
				case SET_BLE_BATTERY_VOLTAGE:
				case SET_BLE_BATTERY_CURRENT:
				case SET_BLE_BATTERY_MAX_VOLTAGE:
				case SET_BLE_BATTERY_MAX_CURRENT:
					{
						type = cJSON_GetObjectItem( root , "value");
						if(type->type==cJSON_String)
						{
							RetResult.StringType = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.StringType,type->valuestring);
							JSON_ANALYSIS_debug(" value = %s\r\n",RetResult.StringType);
						}
						else
						{
							JSON_ANALYSIS_debug(" value Error %s\r\n");
							RetResult.cmd=CMD_Error;
							goto END;
						}
						
					}break;
					
				case SET_BLE_SLEEP_TIME:
					{
						type = cJSON_GetObjectItem( root , "sleep");
						if(type->type==cJSON_String)
						{
							RetResult.BleSleepType.sleep  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleSleepType.sleep ,type->valuestring);
							JSON_ANALYSIS_debug("\r\n sleep = %s \r\n",RetResult.BleSleepType.sleep);
						}
						else
						{
							JSON_ANALYSIS_debug("\r\n sleep Error\r\n");
							RetResult.cmd=CMD_Error;
							goto END;
						}						
						type = cJSON_GetObjectItem( root , "wakeup");
						if(type->type==cJSON_String)
						{
							RetResult.BleSleepType.wakeup  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleSleepType.wakeup ,type->valuestring);	
							JSON_ANALYSIS_debug("\r\n wakeup = %s\r\n",RetResult.BleSleepType.wakeup);
						}
						else
						{
							JSON_ANALYSIS_debug("\r\n wakeup Error\r\n");
							Ql_MEM_Free(RetResult.BleSleepType.sleep);
							RetResult.cmd=CMD_Error;
							goto END;
						}						
					}break;
						
				case SET_BLE_MQTT_PARAMETER:
					{
						type = cJSON_GetObjectItem( root , "sslBrokerUrl");
						if(type->type==cJSON_String)
						{
							RetResult.BleMqttSetType.sslBrokerUrl  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleMqttSetType.sslBrokerUrl,type->valuestring);
							JSON_ANALYSIS_debug("\r\n sslBrokerUrl = %s\r\n",RetResult.BleMqttSetType.sslBrokerUrl);
						}
						else
						{
							JSON_ANALYSIS_debug("\r\n sslBrokerUrl Error\r\n");
							RetResult.cmd=CMD_Error;
							goto END;
						}						
						
						type = cJSON_GetObjectItem( root , "Topic");
						if(type->type==cJSON_String)
						{
							RetResult.BleMqttSetType.Topic  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleMqttSetType.Topic,type->valuestring);
							JSON_ANALYSIS_debug("Topic=%s\r\n",RetResult.BleMqttSetType.Topic);
						}	
						else
						{
							JSON_ANALYSIS_debug("Topic Error\r\n");
							Ql_MEM_Free(RetResult.BleMqttSetType.sslBrokerUrl);
							RetResult.cmd=CMD_Error;
							goto END;
						}
						
						type = cJSON_GetObjectItem( root , "UserName");
						if(type->type==cJSON_String)
						{
							RetResult.BleMqttSetType.UserName  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleMqttSetType.UserName,type->valuestring);
							JSON_ANALYSIS_debug("UserName=%s\r\n",RetResult.BleMqttSetType.UserName);
						}
						else
						{
							JSON_ANALYSIS_debug("UserName Error\r\n");
							Ql_MEM_Free(RetResult.BleMqttSetType.sslBrokerUrl);
							Ql_MEM_Free(RetResult.BleMqttSetType.Topic);
							RetResult.cmd=CMD_Error;
							goto END;
						}	
						
						type = cJSON_GetObjectItem( root , "password");
						if(type->type==cJSON_String)
						{
							RetResult.BleMqttSetType.password  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleMqttSetType.password,type->valuestring);
							JSON_ANALYSIS_debug("\r\n password = %s\r\n",RetResult.BleMqttSetType.password);
						}
						else
						{
							JSON_ANALYSIS_debug("password Error\r\n");
							Ql_MEM_Free(RetResult.BleMqttSetType.sslBrokerUrl);
							Ql_MEM_Free(RetResult.BleMqttSetType.Topic);	
							Ql_MEM_Free(RetResult.BleMqttSetType.UserName);
							RetResult.cmd=CMD_Error;
							goto END;
						}
					}break;
				default :  RetResult.cmd=CMD_Error; JSON_ANALYSIS_debug("The cmd Error %d\r\n");break;
			}

			JSON_ANALYSIS_debug("The analysis end %d\r\n");

			
		 }
	}
END:
	cJSON_Delete(root);
	return (&RetResult);
}




/*!
 * @brief �ͷ�JsonCmdAnalysis()����ʱΪ���ص����ݿ����ڴ�
 * \n
 *
 * @param pMem :��Ҫ�ͷŵĽ����������Ӧ��ָ��
 * @return void
 * \n
 * @see
 */
void FreeAnalysisMem(SJON_CMD_TYPE *pMem)
{
	unsigned short index;

	index = pMem->cmd;
	switch(index)
	{
		case SET_BLE_PRW:
		case SET_BLE_SEARCH_INTERVAL:
		case SET_BLE_SEARCH_WINDOW:
		case SET_BLE_REPORT_INTERVAL:
		case SET_BLE_BATTERY_VOLTAGE:
		case SET_BLE_BATTERY_CURRENT:
		case SET_BLE_BATTERY_MAX_VOLTAGE:
		case SET_BLE_BATTERY_MAX_CURRENT:
			{
				Ql_MEM_Free(pMem->StringType);
				JSON_ANALYSIS_debug("\r\n Free StringType %d\r\n");
			}break;
			
		case SET_BLE_SLEEP_TIME:
			{
				
				Ql_MEM_Free(pMem->BleSleepType.sleep);
				Ql_MEM_Free(pMem->BleSleepType.wakeup);	
				JSON_ANALYSIS_debug("\r\n Free BleSleepType %d\r\n");
			}break;
				
		case SET_BLE_MQTT_PARAMETER:
			{
				Ql_MEM_Free(pMem->BleMqttSetType.password);					
				Ql_MEM_Free(pMem->BleMqttSetType.sslBrokerUrl);		
				Ql_MEM_Free(pMem->BleMqttSetType.Topic);					
				Ql_MEM_Free(pMem->BleMqttSetType.UserName);	
				JSON_ANALYSIS_debug("\r\n Free BleMqttSetType %d\r\n");
			}break;
		default :  break;
	}

	

	
}


/*!
 * @brief ģ����Ժ���
 * \n
 *
 * @param NULL 
 * @return NULL
 * \n
 * @see
 */
void testJsonAnalysis(void)
{

	SJON_CMD_TYPE* JSON_CMD;
	char *json="{\"type\":\"ble_mqtt\",\"sslBrokerUrl\":\"ssl://post-cn-v0h0dlzxa02.mqtt.aliyuncs.com:8883\",\"Topic\":\"xxtopic\",\"UserName\":\"LTAIbX60YQNt7UTf\",\"password\":\"3UyyQbjklF8U/QnQlBYxHNZTBiU=\"}";
	char *mmce;
	int temp;

	CreateBleInfoList();
	SetBleInfoList("12334567","2018-3/T09-02","123","1");//(char *pSN , char *pTimestamp , char *pCycle, char *pNum);
	AddBleInfoToList("Thomas", "01af5bc8d5e9", "-12", "�����һ������");
	AddBleInfoToList("Thomas1", "01af5bc8d5ea", "-18", "�����һ������1");
	AddBleInfoToList("Thomas2", "01af5bc8d5eb", "-20", "�����һ������2");
	mprintf("%s\r\n",mmce = BleInfoToJson());
	
	temp=DeleteBleInfoToList("01af5bc8d5e9");
	if(temp==0)
	{
		mprintf("ɾ�����");
	}else
	{
		mprintf("ɾ��ʧ�� ������ %d",temp);
	}
	temp=DeleteBleInfoToList("01a05bc8d5ea");
	if(temp==0)
	{
		mprintf("ɾ�����");
	}else
	{
		mprintf("ɾ��ʧ�� ������ %d",temp);
	}	
	
	mprintf("%s\r\n",mmce = BleInfoToJson());

	mprintf("%s\r\n",mmce = BleHeartbeatToJson("123abd43d","2018-3-8/T-16-21","4.100V","2.000A","15.000V","4.000A"));
	
	FreeJsonString(mmce);

	mprintf("%s\r\n",mmce=AckForJsonCmd("ble_heartbeat","12343455"));

	FreeJsonString(mmce);

	JSON_CMD = JsonCmdAnalysis(json);

	FreeAnalysisMem(JSON_CMD);	
}




