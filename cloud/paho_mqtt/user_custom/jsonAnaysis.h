



#ifndef JSON_ANAYSIS_H_
#define JSON_ANAYSIS_H_


typedef enum
{
	SET_BLE_PRW=0,				//�����������书������
	SET_BLE_SEARCH_INTERVAL,	//�������������������
	SET_BLE_SEARCH_WINDOW,		//����������������ʱ������
	SET_BLE_REPORT_INTERVAL,	//����������Ϣ�ϱ��������
	SET_BLE_BATTERY_VOLTAGE,	//���õ�ص�ѹ����
	SET_BLE_BATTERY_CURRENT,	//���õ�ص�������
	SET_BLE_BATTERY_MAX_VOLTAGE,//������ӵ�Դ��ѹ��������
	SET_BLE_BATTERY_MAX_CURRENT,//������ӵ�Դ������������
	SET_BLE_SLEEP_TIME,			//��������ʱ������
	SET_BLE_RESET,				//�豸��λ����
	SET_BLE_MQTT_PARAMETER,		//�豸mqttͨѶ��������
	CMD_Error,					//ָ�����
}E_S2C_CMD;//���ܵ�ָ����
	


typedef enum
{
	TRANSMIT_BLE_LIST=0,
	TRANSMIT_BLE_HEARTBEAT,
	TRANSMIT_ACK,
}E_C2S_CMD;//���͵�ָ����

typedef struct
{
	char name[20];
	char mac[20];
	char rssi[5];
	unsigned char RAWdata[2];

}BLE_INFO_TYPE;//������Ϣ�ṹ

typedef struct
{
	char *sleep;
	char *wakeup;
}
BLE_SLEEP_TIME;

typedef struct
{
	char *sslBrokerUrl;
	char *Topic;
	char *UserName;
	char *password;
}
BLE_MQTT_SET;	//mqttͨ�����õ�����




typedef struct
{
	E_C2S_CMD cmd;
	char *StringType;
	BLE_SLEEP_TIME BleSleepType;
	BLE_MQTT_SET   BleMqttSetType;
}SJON_CMD_TYPE;	//�������jsonָ��







int CreateBleInfoList(void);

int AddBleInfoToList(char *name , char *mac , char *rssi, char *RAWdata );

int DeleteBleInfoToList(const char * mac );

char *BleInfoToJson(void);

int SetBleInfoList(char *pSN , char *pTimestamp , char *pCycle, char *pNum);

char *BleHeartbeatToJson(char * mac,char *timestamp,char *bat_voltage,char *bat_current,char *pwr_voltage,char *pwr_current);

char *AckForJsonCmd(char *type, char *sn);

void FreeJsonString(char *pJsonStr);

SJON_CMD_TYPE*JsonCmdAnalysis(char *Payload);

void FreeAnalysisMem(SJON_CMD_TYPE *pMem);

void testJsonAnalysis(void);
int DeleteBleInfoP(void);








#endif


