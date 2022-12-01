/**
 * @file portcan.c
 * @author T0n0T (kamitixe@gmail.com)
 * @brief Add to freeModbus
 * @version 0.1
 * @date 2022-12-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "rtdevice.h"
#include "board.h"

/* ----------------------- Static variables ---------------------------------*/
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t serial_soft_trans_irq_stack[512];

static struct rt_thread thread_can_soft_trans_irq;
static struct rt_event event_can;
/* modbus slave can device */
static struct rt_can_device *can;

/* can transmit event */
#define EVENT_CAN_TRANS_START    (1<<0)

/* ----------------------- static functions ---------------------------------*/
static void prvvCANxReadyISR(void);
static void prvvCANxISR(void);
static rt_err_t can_rx_ind(rt_device_t dev, rt_size_t size);
static void can_soft_trans_irq(void* parameter);

int mb_port_can_init(const char* can_name, void* baudrate)
{
    rt_device_t dev = RT_NULL;
    dev = rt_device_find(can_name);
    if (dev == RT_NULL) {
        return FALSE; 
    } else {
        can = (struct rt_can_device*)dev;
    }

    rt_device_control(&can->parent, RT_CAN_CMD_SET_BAUD, baudrate);

    if (!rt_device_open(&can->parent, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX))
    {
        rt_device_set_rx_indicate(&can->parent, can_rx_ind);
    } else {
        return FALSE;
    }

    rt_event_init(&event_can, "slave event", RT_IPC_FLAG_PRIO);
    rt_thread_init(&thread_can_soft_trans_irq,
                   "slave trans",
                   can_soft_trans_irq,
                   RT_NULL,
                   serial_soft_trans_irq_stack,
                   sizeof(serial_soft_trans_irq_stack),
                   10, 5);
    rt_thread_startup(&thread_can_soft_trans_irq);

    return TRUE;      
}

void mb_port_can_enable(BOOL xRxEnable, BOOL xTxEnable)
{
    rt_uint32_t recved_event;
    if (xRxEnable)
    {
        can->ops->control(can, RT_DEVICE_CTRL_SET_INT, (void *)RT_DEVICE_FLAG_INT_RX);
    }
    else
    {
        can->ops->control(can, RT_DEVICE_CTRL_CLR_INT, (void *)RT_DEVICE_FLAG_INT_RX);
    }
    if (xTxEnable)
    {
        /* start serial transmit */
        rt_event_send(&event_can, EVENT_CAN_TRANS_START);
    }
    else
    {
        /* stop serial transmit */
        rt_event_recv(&event_can, EVENT_CAN_TRANS_START,
                RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, 0,
                &recved_event);
    }
}

void mb_port_can_close(void)
{
    can->parent.close(&(can->parent));
}

BOOL xMBPortCanPutByte(CHAR ucByte)
{
    can->parent.write(&(can->parent), 0, &ucByte, 1);
    return TRUE;
}

BOOL xMBPortCanGetByte(CHAR * pucByte)
{
    can->parent.read(&(can->parent), 0, pucByte, 1);
    return TRUE;
}

void prvvCANxReadyISR(void)
{
    pxMBFrameCBTransmitterEmpty();
}

void prvvCANxISR(void)
{
    pxMBFrameCBByteReceived();
}

static void can_soft_trans_irq(void* parameter) {
    rt_uint32_t recved_event;
    while (1)
    {
        /* waiting for serial transmit start */
        rt_event_recv(&event_can, EVENT_CAN_TRANS_START, RT_EVENT_FLAG_OR,
                RT_WAITING_FOREVER, &recved_event);
        /* execute modbus callback */
        prvvCANxReadyISR();
    }
}

static rt_err_t can_rx_ind(rt_device_t dev, rt_size_t size) {
    while(size--)
        prvvCANxISR();
    return RT_EOK;
}