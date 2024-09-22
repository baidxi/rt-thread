#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "device.h"
#include "clock.h"
#include "uart.h"

struct serial_device serial_ports[] = {
#ifdef RT_USING_UART0
    {
        .name = "uart0",
        .base = 0x02500000,
        .id = 0,
        .pinctrl[0] = {
            .pin = 2,
            .func = 3,
            .group = 6,
            .pull = 0x1,
        },
        .pinctrl[1] = {
            .pin = 4,
            .func = 3,
            .group = 6,
        },
    },
#endif
#ifdef RT_USING_UART1
    {
        .name = "uart1",
        .base = 0x02500400,
        .id = 1,
        .pinctrl[0] = {
            .pin = 10,
            .func = 3,
            .group = 5,
            .pull = 0x1,
        },
        .pinctrl[1] = {
            .pin = 11,
            .func = 3,
            .group = 5,
        },
    },
#endif
#ifdef RT_USING_UART2
    {
        .name = "uart2",
        .base = 0x02500800,
        .id = 2,
        .pinctrl[0] = {
            .pin = 2,
            .func = 3,
            .group = 5,
            .pull = 0x1,
        },
        .pinctrl[1] = {
            .pin = 3,
            .func = 3,
            .group = 5,
        },
    },
#endif
#ifdef RT_USING_UART3
    {
        .name = "uart3",
        .base = 0x02500c00,
        .id = 3,
        .pinctrl[0] = {
            .pin = 6,
            .func = 7,
            .group = 1,
            .pull = 0x1,
        },
        .pinctrl[1] = {
            .pin = 7,
            .func = 7,
            .group = 1,
        },
    },
#endif
#ifdef RT_USING_UART4
    {
        .name = "uart4",
        .base = 0x02501000,
        .id = 4,
        .pinctrl[0] = {
            .pin = 4,
            .func = 3,
            .group = 5,
            .pull = 0x1,
        },
        .pinctrl[1] = {
            .pin = 5,
            .func = 3,
            .group = 5,
        },
    },
#endif
#ifdef RT_USING_UART5
    {
        .name = "uart5",
        .base = 0x02501400,
        .id = 5,
        .pinctrl[0] = {
            .pin = 6,
            .func = 3,
            .group = 5,
            .pull = 0x1,
        },
        .pinctrl[1] = {
            .pin = 7,
            .func = 3,
            .group = 5,
        },
    }
#endif
};

static int serial_device_init(struct serial_device *ser_dev, struct serial_configure *cfg)
{
    clock_enable(ser_dev->id);

    clock_reset(ser_dev->id);

    

    return 0;
}

static rt_err_t serial_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct serial_device *ser_dev;
    int ret;

    ser_dev = serial->parent.user_data;

    ret = serial_device_init(ser_dev, cfg);

    if (ret) {
        rt_kprintf("config %s err:%d\n", ser_dev->name, ret);
        return ret;
    }

    return RT_EOK;
}

static rt_err_t serial_ctrl(struct rt_serial_device *serial, int cmd, void *arg)
{
    return 0;
}

static int serial_put_c(struct rt_serial_device *serial, char c)
{
    return 0;
}

static int serial_get_c(struct rt_serial_device *serial)
{
    return 0;
}

static rt_ssize_t serial_dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction)
{
    return 0;
}

const struct rt_uart_ops serial_ops = {
    .configure = serial_configure,
    .control = serial_ctrl,
    .putc = serial_put_c,
    .getc = serial_get_c,
    .dma_transmit = serial_dma_transmit,
};

int rt_hw_uart_init(void)
{
    struct serial_device *ser_dev;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    int idx = 0;
    int ret;

    for (idx = 0; idx < ARRAY_SIZE(serial_ports); idx++)
    {
        ser_dev = &serial_ports[idx];
        ser_dev->serial_device.ops = &serial_ops;
        ser_dev->serial_device.config = config;
        ser_dev->serial_device.config = config;
        ser_dev->serial_device.config.baud_rate = 115200;

        sunxi_pinctrl_init(&ser_dev->pinctrl[0]);
        sunxi_pinctrl_init(&ser_dev->pinctrl[1]);

        ret = rt_hw_serial_register(&ser_dev->serial_device, ser_dev->name, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, ser_dev);
        if (ret) {
            return ret;
        }
    }

    return RT_EOK;
}

INIT_BOARD_EXPORT(rt_hw_uart_init);
