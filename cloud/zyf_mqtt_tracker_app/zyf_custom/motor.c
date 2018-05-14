#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "motor.h"

motor_ctrl_t motor_ctrl;

void init_motor_gpio(void)
{
	 Ql_GPIO_Init(PINNAME_SIM2_CLK, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_SIM2_DATA, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_SIM2_RST, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
}

void sleep_motor(void)
{
	MOTOR_NSLEEP_OFF;
	MOTOR_REVERSE_OFF;
	MOTOR_FORWARD_OFF;
}

void forward_motor(void)
{
	MOTOR_NSLEEP_ON;
	MOTOR_REVERSE_OFF;
	MOTOR_FORWARD_ON;
}

void reverse_motor(void)
{
	MOTOR_NSLEEP_ON;
	MOTOR_REVERSE_ON;
	MOTOR_FORWARD_OFF;
}


void init_infrared_sensor()
{
	Ql_GPIO_Init(PINNAME_PCM_CLK, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_PCM_IN, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_PCM_OUT, PINDIRECTION_IN, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_PCM_SYNC, PINDIRECTION_IN, 0, PINPULLSEL_PULLUP);

	GPIO_OUT_UNLOCKED_SENSOR_OFF;
	GPIO_OUT_LOCKED_SENSOR_OFF;
}

void motor_init_lockio()
{
    init_motor_gpio();
    sleep_motor();
    init_infrared_sensor();
}


/*!
 * ��ȡ��״̬
 * \n
 *
 * @param cnt ��ȡ��״̬�Ĵ���,ÿ�λ���ʱ10ms
 * @return �ɹ��������߼�״̬(��0)�����򷵻�0
 *	ȫ��ס��11������0,1������1,0���������Ӧ���ǣ�00����11
 * \n
 * @see LOCK_STATUS_t
 */
u8 motor_get_lock_status_10ms(u8 cnt)
{
    u8 i = 0;

    // ��������
    GPIO_OUT_UNLOCKED_SENSOR_ON;
	GPIO_OUT_LOCKED_SENSOR_ON;
    // ��ʱһ���ʱ��
    // ��ȡ��������ƽ
    for (i = 0; i < cnt; i++)
    {
        GPIO_IN_UNLOCKED_SENSOR;
		GPIO_IN_LOCKED_SENSOR;
        Ql_Sleep(10);
    }
    // ���������߼�״̬
    if (GPIO_IN_LOCKED_SENSOR==0&&GPIO_IN_UNLOCKED_SENSOR==1)
    {
        motor_ctrl.cStatus = LOCK_STATUS_LOCKED;
        return LOCK_STATUS_LOCKED;
    }
    else if (GPIO_IN_UNLOCKED_SENSOR==0&&GPIO_IN_LOCKED_SENSOR==1)
    {
        motor_ctrl.cStatus = LOCK_STATUS_UNLOCKED;
        return LOCK_STATUS_UNLOCKED;
    }
    else if ((GPIO_IN_UNLOCKED_SENSOR==0&&GPIO_IN_LOCKED_SENSOR==0)||(GPIO_IN_UNLOCKED_SENSOR==1&&GPIO_IN_LOCKED_SENSOR==1))
    {
        motor_ctrl.cStatus = LOCK_STATUS_LOCKABLE;
        return LOCK_STATUS_LOCKABLE;
    }

	  // �رպ���
    GPIO_OUT_UNLOCKED_SENSOR_OFF;
	GPIO_OUT_LOCKED_SENSOR_OFF;
	
    return 0;
}

/*!
 * ��������
 * \n
 *
 * @param cnt ���ת��ʱ�䣬��10msΪ��λ
 * @return �ɹ�����0
 * \n
 * @see
 */
u8 motor_unlock_10ms(u8 motor_run_time)
{
    u8 i;
    u8 lock_status;
    motor_ctrl.cCmd = LOCK_CTRL_CMD_UNLOCK;

    // �����״̬ �����ʾ�Ѿ���������������
    if (LOCK_STATUS_UNLOCKED == motor_get_lock_status_10ms(10))
        return 0;

    // �������
    forward_motor();
    // ���n�μ�鴫�����Ƿ���״̬1(�ѽ���)
    for (i = 0; i < motor_run_time; i++)
    {
        if (LOCK_STATUS_UNLOCKED == motor_get_lock_status_10ms(10))
            break;
        Ql_Sleep(10);
    }
    // ���ͣת
    sleep_motor();

    // ���ؽ��
    lock_status=motor_get_lock_status_10ms(10);
    if (LOCK_STATUS_UNLOCKED ==lock_status )
        return 0;
    else if(lock_status==LOCK_STATUS_LOCKED)
    return 1;
}


/*!
 * ʹ�ܹ���
 * \n
 *
 * @param cnt ���ת��ʱ�䣬��10msΪ��λ
 * @return �ɹ�����0
 * \n
 * @see
 */
u8 motor_lockable_10ms(u8 motor_run_time)
{
    u8 i;

    motor_ctrl.cCmd = LOCK_CTRL_CMD_LOCKABLE;
    // ��������
    // �����״̬ �����ʾ����/��������������
    if (LOCK_STATUS_LOCKED == motor_get_lock_status_10ms(10))
        return 0;
    
    if (LOCK_STATUS_LOCKABLE == motor_get_lock_status_10ms(10))
        return 0;

    // �������
    reverse_motor();
    // ���n�μ�鴫�����Ƿ���״̬2(������)
    for (i = 0; i < motor_run_time; i++)
    {
        if (LOCK_STATUS_LOCKABLE == motor_get_lock_status_10ms(10))
            break;
        Ql_Sleep(10);
    }

    Ql_Sleep(400);
    // ���ͣת
    sleep_motor();

    // ���ؽ��
    if (LOCK_STATUS_LOCKABLE == motor_get_lock_status_10ms(10))
        return 0;
    return 1;
}


/*!
 * �ȴ�����
 * \n
 *
 * @param cnt �ȴ�������ÿ�� 1s
 * @return �ɹ�����0�����򷵻ط�0
 * \n
 * @see
 */
u8 motor_wait_locked_1s(u8 cnt)
{
    u8 i;
    if (LOCK_STATUS_LOCKED == motor_get_lock_status_10ms(10))
    {
       
        reverse_motor();
        Ql_Sleep(100);
        sleep_motor();

        return 0;
    }
    // ÿ��1s���һ���Ƿ��Ѿ�����
    for (i = 0; i < cnt; i++)
    {
        if (LOCK_STATUS_LOCKED == motor_get_lock_status_10ms(10))
        {
            break;
        }
        Ql_Sleep(100);
    }

    // ���ؽ��
    if (LOCK_STATUS_LOCKED == motor_get_lock_status_10ms(10))
    {
        reverse_motor();
        Ql_Sleep(100);
        sleep_motor();
        return 0;
    }
    return 1;
}







