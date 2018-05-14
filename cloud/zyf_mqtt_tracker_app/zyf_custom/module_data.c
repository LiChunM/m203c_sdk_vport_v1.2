/******************************************************************************

                  ��Ȩ���� (C) 2018, �촴��ũ�Ƽ�

 ******************************************************************************
  �ļ�   : module_data.c
  �汾   : ����
  ����   : LiCM
  ����   : 2018��03��8��
  ����   : �����ļ�
  ����   : ��ȡģ���ڲ�������Ϣ

  �޸ļ�¼:
  ����   : 2018��03��8��
    ����   : LiCM
    ����   : �����ļ�

******************************************************************************/

#include "ril.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "module_data.h"
#include "uart.h"



#ifdef MODULE_DATA_DEBUG
    #define module_data_debug(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
    #define module_data_error(format, ...) mprintf( "[error]%s() %d " format "\r\n", /*__FILE__,*/ __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
   	#define module_data_debug(format, ...)
    #define module_data_error(format, ...)
#endif



Module_Data_T module_data;		//ģ���ڲ���Ϣ�ϼ�

/*!
 * @brief ��ȡģ������ע����Ϣ
 * \n
 *
 * @param creg ע����Ϣ cgreg������Ϣ
 * @return ret Ϊ0��ʾ��ȡ�ɹ�
 * \n
 * @see
 */


s32 GetModuleRegData(s32 *creg,s32 *cgreg)
{
	 s32 ret;
	 ret=RIL_NW_GetGSMState(creg);
	 if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_NW_GetGSMState Error");
	 	}
     ret=RIL_NW_GetGPRSState(cgreg);
	  if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_NW_GetGPRSState Error");
	 	}
	  return ret;
}


/*!
 * @brief ��ȡģ����SIM�������Ϣ
 * \n
 *
 * @param imei ccid imsi 
 * @return ret Ϊ0��ʾ��ȡ�ɹ�
 * \n
 * @see
 */

s32 GetModuleSIMData(void)
{
	u8 tempdata[30];
	s32 ret;
	ret =RIL_GetIMEI(tempdata);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_GetIMEI Error");
	 	}
	module_data_debug("module_data.imei=%s",tempdata);
	ret =RIL_SIM_GetCCID(tempdata);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_SIM_GetCCID Error");
	 	}
	module_data_debug("module_data.ccid=%s",tempdata);
	ret =RIL_SIM_GetIMSI(tempdata);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_SIM_GetIMSI Error");
	 	}
	module_data_debug("module_data.imsi=%s",tempdata);
	return ret;
}

/*!
 * @brief ��ȡģ���ڲ�������Ϣ
 * \n
 *
 * @param void
 * @return NULL
 * \n
 * @see
 */

s32 GetModuleData(void)
{
	s32 t1,t2;
	s32 ret,other;
	ret=GetModuleRegData(&t1,&t2);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("GetModuleRegData Error");
	 	}
	module_data_debug("creg=%d",t1);
	module_data_debug("cgreg=%d",t2);
	ret = RIL_GetPowerSupply(&t1, &t2);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_GetPowerSupply Error");
	 	}
	module_data_debug("capacity=%d",t1);
	module_data_debug("voltage=%d",t2);
	module_data.mvol=t2;
	module_data.bvol=t1;
	ret =RIL_NW_GetSignalQuality(&t1,&t2);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_NW_GetSignalQuality Error");
	 	}
	module_data_debug("csq=%d",t1);
	module_data_debug("ber=%d",t2);
	module_data.csq=t1;
	return ret;
}







