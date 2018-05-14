#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "sys.h"
#include "ql_socket.h"
#include "tiem.h"
#include "uart.h"
#include "pro.h"
#include "new_socket.h"
#include "lcd.h"
#include "ql_adc.h"
#include "flash.h"
#include "PRD.h"
#include "gps.h"
#include "ql_pwm.h"
#include "ql_memory.h"



/*!
 * @brief �������ݴ��,���������ļ�
 * \n
 *
 * @param Coredata  �ɼ���������,��ʽ����ʽд��Vֵ
 * \n
 * @see
 */


void Ch2Protocol(Coredata data)
{
	Ql_sprintf(AutomaticRDTlv[0].value,"%d.%06d",data.lbsw/1000000,data.lbsw%1000000);
	Ql_sprintf(AutomaticRDTlv[1].value,"%d.%06d",data.lbsj/1000000,data.lbsj%1000000);
	Ql_sprintf(AutomaticRDTlv[2].value,"%d.%06d",data.gpsw/1000000,data.gpsw%1000000);
	Ql_sprintf(AutomaticRDTlv[3].value,"%d.%06d",data.gpsj/1000000,data.gpsj%1000000);
	Ql_sprintf(AutomaticRDTlv[4].value,"%04d",data.vbai);
	Ql_sprintf(AutomaticRDTlv[5].value,"%d",data.dbm);
	Ql_sprintf(AutomaticRDTlv[6].value,"%d",systemset.SysTime);

}


/*!
 * @brief �������ݴ���,���������ļ�
 * \n
 *
 * @param index ����ָ�����
 *		  *dowhat ����������ݺ���Ҫ���ݵ���Ϣ
 * \n
 * @see
 */
 
u8 SendMsg2HalDoHd(u8 index,u8 *dowhat)
{
	u8 *p=NULL;
	u8 temstr[20]={0};
	*dowhat=0;
	if(ProNumberInstruct.tlvs[index].type==AutomaticWRTlv[0].type)
		{
			if(ProNumberInstruct.tlvs[index].value[0]==0x31)
				{
					LED2_H;
				}
			if(ProNumberInstruct.tlvs[index].value[0]==0x30)
				{
					LED2_L;
				}
		}
	else if(ProNumberInstruct.tlvs[index].type==AutomaticWRTlv[1].type)
		{
			Ql_sprintf(temstr,"%s",ProNumberInstruct.tlvs[index].value);
			systemset.Interval=strtol((const char*)temstr,NULL,10);
			SaveFlashParamsNew(&systemset);
			jishis=0;
			jishit=0;
		}
	else if(ProNumberInstruct.tlvs[index].type==AutomaticWRTlv[2].type)
		{
			Ql_sprintf(systemset.SN,"%s",ProNumberInstruct.tlvs[index].value);
			SaveFlashParamsNew(&systemset);
			InternetsetSN_AnalysisFor808(systemset.SN);
			mprintf("+SN %s\r\n",(u8*)systemset.SN);
			IsSnHavaSet();
		}
	return 0;
}



