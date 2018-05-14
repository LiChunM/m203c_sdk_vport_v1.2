
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "ql_system.h"
#include "ql_uart.h"
#include "aliot_platform.h"
#include "aliot_platform_os.h"
#include "sys.h"


#ifdef DEBUG
    #define PLATFORM_OS_PERROR(format, ...) aliot_platform_printf(NRF_LOG_COLOR_YELLOW "[OS]%u %s " format "\n", __LINE__, __FUNCTION__, ##__VA_ARGS__)
    #define PLATFORM_OS_PDEBUG(format, ...) aliot_platform_printf(NRF_LOG_COLOR_YELLOW "[OS]%s " format "\n", __FUNCTION__, ##__VA_ARGS__)
#else
    #define PLATFORM_OS_PERROR(format, ...)
    #define PLATFORM_OS_PDEBUG(format, ...)
#endif
/*
�������������������OS��ƽ̨������ʵ�ָýӿ�
���ٻ��������������OS��ƽ̨������ʵ�ָýӿ�
��סָ�����������������OS��ƽ̨������ʵ�ָýӿ�
�ͷ�ָ�����������������OS��ƽ̨������ʵ�ָýӿ�
*/
void* aliot_platform_mutex_create(void)
{
    return NULL;
}
void aliot_platform_mutex_destroy(_IN_ void* mutex)
{
    return;
}
void aliot_platform_mutex_lock(_IN_ void* mutex)
{
    return;
}
void aliot_platform_mutex_unlock(_IN_ void* mutex)
{
    return;
}
/*
�����ڴ��
�ͷ��ڴ��
�Ѹ�Ϊ��̬�����ڴ棬����ʵ�ָýӿ�
*/
void* aliot_platform_malloc(_IN_ uint32_t size)
{
    void* ptr = Ql_MEM_Alloc(size);
    PLATFORM_OS_PDEBUG("%#x %d bytes, used=%d.", ptr, size, lite_mem_used());
    return ptr;
}
void aliot_platform_free(_IN_ void* ptr)
{
    PLATFORM_OS_PDEBUG("%#X, used=%d.", ptr, lite_mem_used());
    Ql_MEM_Free(ptr);
    return;
}
/*
����OTA�������ݲ�֧��OTA���ܣ��ýӿ�������ʵ��
дOTA�̼��������ݲ�֧��OTA���ܣ��ýӿ�������ʵ��
���OTA�������ݲ�֧��OTA���ܣ��ýӿ�������ʵ��
*/
int aliot_platform_ota_start(const char* md5, uint32_t file_size)
{
    return -1;
}
int aliot_platform_ota_write(_IN_ char* buffer, _IN_ uint32_t length)
{
    return -1;
}
int aliot_platform_ota_finalize(_IN_ int stat)
{
    return -1;
}
/* ��ȡϵͳʱ�䣨��λ��ms����������� */
uint32_t aliot_platform_time_get_ms(void)
{
    // TODO
    return systemset.SysTime*1000;
}
/*
˯��ָ��ʱ�䣬�������OS��ƽ̨��������ʵ��Ϊ��ʱָ��ʱ�伴��
*/
void aliot_platform_msleep(_IN_ uint32_t ms)
{
    Ql_Sleep(ms);
}

/*
hex��ʽ��ӡ u8 pdata[i] ��ǰn��Ԫ��
*/
void aliot_platform_printhex(const int8_t* title, uint8_t* pdata, uint32_t count)
{
    uint32_t i;
    if (count == 0)
        return;
    if (title != NULL)
        aliot_platform_printf("%s: ", title);
    for (i = 0; i < count; i++)
        aliot_platform_printf("%02X ", *(pdata + i));
    aliot_platform_printf("(%dB)\r\n", count);
}
char* aliot_platform_module_get_pid(char pid_str[])
{
    return NULL;
}
