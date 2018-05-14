
#include <string.h>
#include "aliot_platform_os.h"

#include "aliot_log.h"
#include "aliot_error.h"
#include "aliot_device.h"


static aliot_user_info_t aliot_user_info;
static aliot_device_info_t aliot_device_info;


int aliot_device_init(void)
{
    MEMSET(&aliot_device_info, 0x0, sizeof(aliot_device_info_t));
    MEMSET(&aliot_user_info, 0x0, sizeof(aliot_user_info_t));

    ALIOT_LOG_INFO("device init success!");
    return SUCCESS_RETURN;
}


int32_t aliot_set_device_info(
            const char *product_key,
            const char *device_name,
            const char *device_secret)
{
    int ret;
    ALIOT_LOG_DEBUG("start to set device info!");
    MEMSET(&aliot_device_info, 0x0, sizeof(aliot_device_info));

    strncpy(aliot_device_info.product_key, product_key, PRODUCT_KEY_LEN);
    strncpy(aliot_device_info.device_name, device_name, DEVICE_NAME_LEN);
    strncpy(aliot_device_info.device_secret, device_secret, DEVICE_SECRET_LEN);

    //construct device-id(@product_key+@device_name)
    ret = SNPRINTF(aliot_device_info.device_id, DEVICE_ID_LEN, "%s.%s", product_key, device_name);
    if ((ret < 0) || (ret >= DEVICE_ID_LEN)) {
        ALIOT_LOG_ERROR("set device info failed");
        return FAIL_RETURN;
    }

    ALIOT_LOG_DEBUG("set device info successfully!");
    return SUCCESS_RETURN;
}

aliot_device_info_pt aliot_get_device_info(void)
{
    return &aliot_device_info;
}


aliot_user_info_pt aliot_get_user_info(void)
{
    return &aliot_user_info;
}


