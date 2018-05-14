
#ifndef _PLATFORM_ALIOT_PLATFORM_H_
#define _PLATFORM_ALIOT_PLATFORM_H_

// ѡ���豸ͨ��TCP��TLS��MQTT����˽�������
// 1 -- TCP
// 0 -- SSL
#define ALIOT_MQTT_TCP 1

#if defined(_WIN32)
#define ALIOT_AUTH_SKIP	0	// �Ƿ�������֤(������;)
#else
#define ALIOT_AUTH_SKIP	0	// �Ƿ�������֤(������;)
#endif

#define MYDEVICE_NOT_SUPPORT_TIME	1	// �豸�Ƿ�֧�� time()

#include "aliot_platform_datatype.h"
#include "os/aliot_platform_os.h"
#include "network/aliot_platform_network.h"
#include "ssl/aliot_platform_ssl.h"


#endif /* _PLATFORM_ALIOT_PLATFORM_H_ */
