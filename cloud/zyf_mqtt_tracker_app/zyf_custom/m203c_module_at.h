/******************************************************************************

                  ��Ȩ���� (C) 2017, ��ͨ�Ƽ�

 ******************************************************************************
  �ļ�   : quectel_module_at.h
  �汾   : ����
  ����   : zhoujie
  ����   : 2017��11��29��
  �޸�   :
  ����   : quectel_module_at.c ͷ�ļ�

  �޸ļ�¼:
  ����   : 2017��11��29��
    ����   : zhoujie
    ����   : �����ļ�

******************************************************************************/

#ifndef __QUECTEL_MODULE_AT_H__
#define __QUECTEL_MODULE_AT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
extern const char* const AT_commands[];
enum AT_CMD_INDEX
{
    AT_URC = 0,
    AT_QINISTAT,
    AT,       // at sync
    ATV,      // ��Ӧ���ظ�ʽ
    ATE0,     // ����
    AT_CMEE2, //�ϱ���������
    AT_IPR,   // baudrate
    ATI,      // module info
    AT_GSN,   // gsn/imei
    AT_CPIN,  // ��ѯsim��״̬
    AT_CIMI,  // sim imsi
    AT_QCCID, // sim ccid
    AT_CSQ,
    AT_CREG,
    AT_CGREG,
    AT_COPS,
    AT_CGATT,
    AT_QIHEAD1,
    AT_QIDNSIP,
    AT_QIDNSIP_ip,
    AT_QIDNSIP_domain,
    AT_QISHOWRA0,
    AT_QISHOWPT0,
    AT_QISHOWPT1,
    AT_QIMODE,  // ͸��ģʽ���͸��ģʽ��ѯ
    AT_QIMODE0, // ��͸��ģʽ����
    AT_QIMODE1, // ͸��ģʽ
    AT_QIMUX_0,
    AT_QIMUX_1,
    AT_QIMUX_status,
    AT_QINDI2,
    AT_QINDI1,
    AT_QINDI0,
    AT_QINDIstatus,
    AT_QISDE0,
    AT_QIFGCNT0, // ����PDP Context0
    AT_QIFGCNT2, // ����PDP Context2
    AT_QICSGP,
    AT_QISEND,
    AT_QSSLSEND,
    AT_QIREGAPP,
    AT_QIACT,
    AT_QILOCIP,
    AT_QIOPEN,
    AT_QISACK,
    AT_QICLOSE,
    AT_QIDEACT,
    AT_QISTAT,
    AT_QSSLSTATE,
    AT_QIRD,
    AT_QSSLRECV,   // ��ȡ ssl ����
    ATF,           // �ָ�ȱʡ����
    ATZ,           // �ָ���������
    AT_CFUNstatus, // ����ģ�鹦�� 15s��������״̬Ӱ��
    AT_CFUN0,      // ����ģ�鹦�� 15s��������״̬Ӱ��
    AT_CFUN1,      // ����ģ�鹦�� 15s��������״̬Ӱ��
    AT_QPOWD,      // �ػ�
    AT_QITCFG,     // ����͸������ģʽ
    AT_DTR,        //���� DTR  ����ģʽ DTR �� ON �� OFF��TA �ڱ��ֵ�ǰ����ͨ����ͬʱ���л�������ģʽ
    AT_QCELLLOC,   //Get Current Location
    AT_QNSTATUS,   //��ѯ GSM  ����״̬
    AT_CTZU,       //����ʱ��ͬ���Լ����� RTC ���豣�棬�����Զ�д�� Flash
    AT_CTZR,       //����ʱ��ͬ���ϱ�
    AT_QNITZ,      //ͬ������ʱ��
    AT_CCLK,       //ʵʱʱ��
    AT_QLEDMODE0,  //����ָʾ������
    AT_QSCLKstate, //��ʱ������
    AT_QSCLK2,     //��ʱ������
    ATW,           //�����û����ò���
    AT_QHTTPURL,   //Set HTTP Server URL
    AT_QHTTPPOST,  //Send HTTP POST Request
    AT_QHTTPREAD,  //Read HTTP Server Response
    AT_QHTTPGET,   //Send HTTP GET Request
    AT_QHTTPDL,    //Send HTTP file download
    AT_QFLDS,      //Get data storage size
    AT_QFLST,      // List files
    AT_QFDWL,      // Download file from storage
    AT_QFDEL,      // Delete file in storage
    AT_QFOPEN,     // Open file
    AT_QFREAD,     // Read file
    AT_QFCLOSE,    // Close file
    ATS0,          // ATS0 and save
    AT_QNTP,       // ͨ������ʱ�������ͬ������ʱ��
    AT_QGPCLASS,   // ���� GPRS    ��ʱ϶����
    AT_CDETXPW,    // reduce TX power,The configuration can be stored in NVRAM automatically.
    AT_QENG,
    AT_QAUDCH,
    AT_CLVL,
    AT_QWTTS,
    AT_QFTPCFG,  // AT+QFTPCFG=4,"/RAM/fm0.3.bin"
    AT_QFTPUSER, //AT+QFTPUSER="test"
    AT_QFTPPASS, //AT+QFTPPASS="test"
    AT_QFTPOPEN, //AT+QFTPOPEN="114.215.196.51",21
    AT_QFTPSTAT, //AT+QFTPSTAT
    AT_QFTPPATH, //AT+QFTPPATH? AT+QFTPPATH="/firmware/"
    AT_QFTPGET,  // AT+QFTPGET="0.3"
    AT_QFTPCLOSE,
    AT_QSIMSTAT,       // SIM�����״̬�ϱ�
    AT_QSIMDET,        // ���� SIM ����⹦��
    AT_QGNSSC,         // ��ѯGNSSģ���Դ״̬
    AT_QGNSSC1,        // ����GNSSģ���Դ   gprs_atcmd_ex(AT_QGNSSC, NULL);
    AT_QGNSSC0,        // �ر�GNSSģ���Դ   gprs_atcmd_ex(AT_QGNSSC, NULL);
    AT_QGNSSRD,        // ��ȡ��λ����
    AT_QGNSSCMD,       // ��GNSSģ�鷢������ gprs_atcmd_ex(AT_QGNSSCMD, "\"ָ���ַ���\"");
    AT_QGNSSEPO1,      // ʹ��EPO����        gprs_atcmd_ex(AT_QGNSSEPO, NULL);
    AT_QGNSSEPOstatus, // ��ѯepo�Ƿ�ʹ��
    AT_QGEPOAID,       // ����EPO����
    AT_QGNSSTS,        // ��ѯʱ��ͬ��״̬
    AT_QGREFLOC,       // Ϊ�붨��������Reference location��Ϣ
    AT_CBC,            // ��ѯ��ص����ͳ��״̬
    AT_PROFILE,        // Display Current Configuration
    AT_QSSLOPEN,
    AT_QSSLCFG_sslversion,
    AT_QSSLCFG_ciphersuite,
    AT_QSSLCFG_seclevel,
    AT_QSECREAD_cacert,
    AT_QSECREAD_clientcert,
    AT_QSECREAD_clientkey,
    AT_QSECWRITE_cacert,
    AT_QSECWRITE_clientcert,
    AT_QSECWRITE_clientkey,
    AT_QSSLCFG_cacert,
    AT_QSSLCFG_clientcert,
    AT_QSSLCFG_clientkey,
    AT_QSECDEL_cacert,
    AT_QSECDEL_clientcert,
    AT_QSECDEL_clientkey,
    AT_QSSLCLOSE_idx0,
    AT_CMD_DUMMY, // DUMMY
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __QUECTEL_MODULE_AT_H__ */
