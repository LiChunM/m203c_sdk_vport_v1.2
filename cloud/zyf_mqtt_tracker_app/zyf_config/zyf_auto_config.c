#include "zyf_auto_config.h"
ZYF_AUTO_CONFIG_T ZYFAutoconfig=
{
    "b8b71aabdb39",
	 1883,
	 "www.hizyf.com",
	 "652541",
	 "admin",
	 "zyfadmin",
	 "",
};
/**
*0F82:����ģʽ
*1685:�𶯼���
*1792:LBS(γ��)
*087B:LBS(����)
*1480:GPS(γ��)
*00EA:GPS(���ȣ�
*1E9D:�����ٷֱ�
*023A:�ź�ǿ��
*2557:����ʱ��
*66AA:LED��
*22BC:������
*/
u8 *AutomaticRD[READONLYNUM]={"1792","087B","1480","00EA","1E9D","023A","2557"};
u8 *AutomaticWR[READWRITE]={"0F82","1685","66AA","22BC","150A"};
