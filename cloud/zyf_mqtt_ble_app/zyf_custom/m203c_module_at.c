/******************************************************************************

                  ��Ȩ���� (C) 2017, ��ͨ�Ƽ�

 ******************************************************************************
  �ļ�   : quectel_module_at.c
  �汾   : ����
  ����   : zhoujie
  ����   : 2017��11��29��
  �޸�   :
  ����   : at ָ��

  �޸ļ�¼:
  ����   : 2017��11��29��
    ����   : zhoujie
    ����   : �����ļ�

******************************************************************************/

#include <stddef.h>

const char* const AT_commands[] =
{
    NULL, // for URCs
    "AT+QINISTAT",
    "AT",
    "ATV",
    "ATE0",
    "AT+CMEE=1",
    "AT+IPR=9600",
    "ATI",
    "AT+GSN",
    "AT+CPIN?",
    "AT+CIMI",
    "AT+QCCID",
    "AT+CSQ",
    "AT+CREG?",
    "AT+CGREG?",
    "AT+COPS?",
    "AT+CGATT?",
    "AT+QIHEAD=1",
    "AT+QIDNSIP?",
    "AT+QIDNSIP=0",  // ip
    "AT+QIDNSIP=1",  // dns
    "AT+QISHOWRA=0", // ���ý�������ʱ�Ƿ���ʾ���ͷ��� IP  ��ַ�Ͷ˿ں�
    "AT+QISHOWPT=0", /* �ڽ��յ�������֮ǰ���Ӵ�����Э�����ͣ�TCP����UDP�� */
    "AT+QISHOWPT=1", /* �ڽ��յ�������֮ǰ���Ӵ�����Э�����ͣ�TCP����UDP�� */
    "AT+QIMODE?",    // ���� TCPIP  Ӧ��ģʽΪ͸��ģʽ, 0: ��͸��ģʽ,1 ͸��ģʽ.
    "AT+QIMODE=0",   // ���� TCPIP  Ӧ��ģʽΪ͸��ģʽ, 0: ��͸��ģʽ,1 ͸��ģʽ.
    "AT+QIMODE=1",   // ���� TCPIP  Ӧ��ģʽΪ͸��ģʽ, 0: ��͸��ģʽ,1 ͸��ģʽ.
    "AT+QIMUX=0",
    "AT+QIMUX=1",
    "AT+QIMUX?",
    "AT+QINDI=2",   // ����ģʽ 2
    "AT+QINDI=1",   // ����ģʽ 1
    "AT+QINDI=0",   // ����ģʽ 0
    "AT+QINDI?",    // ����ģʽ ��ѯ
    "AT+QISDE=0",   // ���������ݻ���
    "AT+QIFGCNT=0", //ǰ�ó���0
    "AT+QIFGCNT=2", //ǰ�ó���2
    "AT+QICSGP=1,\"CMNET\"",
    "AT+QISEND=%d",
    "AT+QSSLSEND=0,%d", // SSL send data<ssid[,]length]>
    "AT+QIREGAPP",
    "AT+QIACT",
    "AT+QILOCIP",
    "AT+QIOPEN=\"TCP\",\"%s\",\"%d\"",
    "AT+QISACK",
    "AT+QICLOSE",
    "AT+QIDEACT",
    "AT+QISTAT",
    "AT+QSSLSTATE",
    "AT+QIRD=0,1,0,%d",    // AT+QIRD=<id>,<sc>,<sid>,<len>
    "AT+QSSLRECV=0,0,%d",  // at+qsslrecv=cid,sid,len
    "AT&F",                // �ָ���������
    "ATZ",                 // �ָ�Ĭ������
    "AT+CFUN?",            // 15s
    "AT+CFUN=0",           // 15s
    "AT+CFUN=1",           // 15s
    "AT+QPOWD=2",          // ��������ģ��
    "AT+QITCFG=3,2,512,1", // ����͸������ģʽ
    "AT&D1",               //   ���� DTR  ����ģʽ DTR �� ON �� OFF��TA �ڱ��ֵ�ǰ����ͨ����ͬʱ���л�������ģʽ
    "AT+QCELLLOC=1",       //Get Current Location
    "AT+QNSTATUS",         //��ѯ GSM  ����״̬
    "AT+CTZU=3",           //����ʱ��ͬ���Լ����� RTC NITZ ʱ�Զ����±���ʱ���� RTC��ʱ�������� NVROM
    "AT+CTZR=0",           // ����ʱ��ͬ��ʱ�ϱ�ʱ����Ϣ�ĸı� ��+CTZE: <tz>,<dst>,[<time>]��ʽ�ϱ�
    "AT+QNITZ=1",          //ͬ������ʱ�� ����ͬ������ʱ��, ����ͬ������ʱ�书�ܣ���ֱ���ϱ���+QNITZ:<time>,<ds>��
    "AT+CCLK?",            //ʵʱʱ��
    "AT+QLEDMODE=0",       //����ָʾ������:��������ʱ����ָʾ�ƿ�����˸
    "AT+QSCLK?",           //��ʱ������
    "AT+QSCLK=2",          //��ʱ������
    "AT&W",                //�����û����ò���
    "AT+QHTTPURL",         //Set HTTP Server URL
    "AT+QHTTPPOST",        //Send HTTP POST Request
    "AT+QHTTPREAD",        //Read HTTP Server Response
    "AT+QHTTPGET",         //Send HTTP GET Request
    "AT+QHTTPDL",          //Send HTTP download request
    "AT+QFLDS=\"RAM\"",    //Get data storage size
    "AT+QFLST=\"RAM:*\"",  // List files
    "AT+QFDWL",            // Download file from storage
    "AT+QFDEL=\"RAM:*\"",  // Delete file in storage
    "AT+QFOPEN=",          // Open file
    "AT+QFREAD=",          // Read file
    "AT+QFCLOSE=",         // Close file
    "ATS0=0&W",            // ATS0 and save
    "AT+QNTP=",            // ͨ������ʱ�������ͬ������ʱ��
    "AT+QGPCLASS",
    "AT+CDETXPW", // reduce TX power.
    "AT+QENG=2,3",
    "AT+QAUDCH=",
    "AT+CLVL=",
    "AT+QWTTS=0,7,2,",
    "AT+QFTPCFG=4,\"/RAM/%s\"", // AT+QFTPCFG=4,"/RAM/fm0.3.bin"
    "AT+QFTPUSER=\"%s\"",       //AT+QFTPUSER="test"
    "AT+QFTPPASS=\"%s\"",       //AT+QFTPPASS="test"
    "AT+QFTPOPEN=\"%s\",%s",    //AT+QFTPOPEN="114.215.196.51",21
    "AT+QFTPSTAT",              //AT+QFTPSTAT
    "AT+QFTPPATH=\"%s\"",       //AT+QFTPPATH? AT+QFTPPATH="/firmware/" ע������'/'����
    "AT+QFTPGET=\"%s\"",        //AT+QFTPGET="0.3"
    "AT+QFTPCLOSE",
    "AT+QSIMSTAT=1",
    "AT+QSIMDET=1,0,1",
    "AT+QGNSSC?",        // ��ѯGNSSģ���Դ״̬
    "AT+QGNSSC=1",       // =1 ����GNSSģ���Դ
    "AT+QGNSSC=0",       // =1 ����GNSSģ���Դ
    "AT+QGNSSRD=%s",     // ��ȡ��λ����
    "AT+QGNSSCMD=0,",    // ��GNSSģ�鷢������
    "AT+QGNSSEPO=1",     // ����epo
    "AT+QGNSSEPO?",      // ��ѯepo
    "AT+QGEPOAID",       // ����epo
    "AT+QGNSSTS?",       // ��ѯʱ��ͬ��״̬
    "AT+QGREFLOC=%s,%s", // γ��,���� Ϊ�붨�������� Reference-location ��Ϣ
    "AT+CBC",            // ��ȡ��ص�ѹ
    "AT&V",
    "AT+QSSLOPEN=0,0,\"%s\",%d,%d,75",
    "AT+QSSLCFG=\"sslversion\",0,3",           // 3: TLS1.2
    "AT+QSSLCFG=\"ciphersuite\",0,\"0XFFFF\"", // All support
    "AT+QSSLCFG=\"seclevel\",0,0",             // 0,0: no authentication
    "AT+QSECREAD=\"RAM:cacert.pem\"",
    "AT+QSECREAD=\"RAM:clientcert.pem\"",
    "AT+QSECREAD=\"RAM:clientkey.pem\"",
    "AT+QSECWRITE=\"RAM:cacert.pem\",%d,100",
    "AT+QSECWRITE=\"RAM:clientcert.pem\",%d,100",
    "AT+QSECWRITE=\"RAM:clientkey.pem\",%d,100",
    "AT+QSSLCFG=\"cacert\",0,\"RAM:cacert.pem\"",
    "AT+QSSLCFG=\"clientcert\",0,\"RAM:clientcert.pem\"",
    "AT+QSSLCFG=\"clientkey\",0,\"RAM:clientkey.pem\"",
    "AT+QSECDEL=\"RAM:cacert.pem\"",
    "AT+QSECDEL=\"RAM:clientcert.pem\"",
    "AT+QSECDEL=\"RAM:clientkey.pem\"",
    "AT+QSSLCLOSE=0", //Close socket index 0
    NULL,             //AT_CMD_DUMMY
};
