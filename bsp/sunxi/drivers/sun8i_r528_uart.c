#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <rtconfig.h>

#include "sunxi_device.h"
#include "sunxi_clock.h"

#include "sunxi_uart.h"

#define UART_THR    0x0000
#define UART_RBR    0x0000
#define UART_IER    0x0004
#define UART_USR    0x007c

struct serial_device serial_ports[] = {
#ifdef RT_USING_UART0
    {
        .name = "uart0",
        .base = 0x02500000,
        .id = 0,
        .irq = 34,
        .pinctrl[0] = {
            .pin = 2,
            .func = 6,
            .group = 4,
            .pull = 0x1,
        },
        .pinctrl[1] = {
            .pin = 3,
            .func = 6,
            .group = 4,
        },
    },
#endif
#ifdef RT_USING_UART1
    {
        .name = "uart1",
        .base = 0x02500400,
        .id = 1,
        .irq = 35,
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
        .irq = 36,
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
        .irq = 37,
        .pinctrl[0] = {
            .pin = 6,
            .func = 7,
            .group = 1,
        },
        .pinctrl[1] = {
            .pin = 7,
            .func = 7,
            .group = 1,
            .pull = 0x1,
        },
    },
#endif
#ifdef RT_USING_UART4
    {
        .name = "uart4",
        .base = 0x02501000,
        .id = 4,
        .irq = 38,
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
        .irq = 39,
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
    rt_uint32_t val;

    clock_enable(ser_dev->id);

    clock_reset(ser_dev->id);

    write32(ser_dev->base + 0x04, 0x0);
    write32(ser_dev->base + 0x08, 0xf7);
    write32(ser_dev->base + 0x10, 0x0);

    val = readl(ser_dev->base + 0x0c);
    val |= (1 << 7);

    write32(ser_dev->base + 0x0c, val);
    write32(ser_dev->base + 0x00, 0xd & 0xff);
    write32(ser_dev->base + 0x04, (0xd >> 8) & 0xff);

    val = readl(ser_dev->base + 0x0c);
    val &= ~(1 << 7);

    write32(ser_dev->base + 0x0c, val);
    val = readl(ser_dev->base + 0x0c);
    val &= ~0x1f;
    val |= (0x3 << 0) | (0x0 << 2) | (0x0 << 3);

    write32(ser_dev->base + 0x0c, val);

    return 0;
}

static rt_err_t serial_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct serial_device *ser_dev;
    int ret;

    ser_dev = serial->parent.user_data;

    serial->config = *cfg;

    ret = serial_device_init(ser_dev, cfg);

    if (ret) {
        rt_kprintf("config %s err:%d\n", ser_dev->name, ret);
        return ret;
    }

    return RT_EOK;
}

static void serial_irq_handler(int irq, void *param)
{
    rt_uint32_t val;
    struct rt_serial_device *serial = param;
    struct serial_device *ser_dev = serial->parent.user_data;

    val = readl(ser_dev->base + 0x08) & 0x0f;

    if (val & 0x4)
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);

}

static rt_err_t serial_ctrl(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct serial_device *ser_dev = serial->parent.user_data;

    RT_ASSERT(serial != RT_NULL);

    switch(cmd) {
        case RT_DEVICE_CTRL_CLR_INT:
            rt_hw_interrupt_mask(ser_dev->irq);
            writel(0x00, ser_dev->base + UART_IER);
            break;
        case RT_DEVICE_CTRL_SET_INT:
            rt_hw_interrupt_install(ser_dev->irq, serial_irq_handler, serial, ser_dev->name);
            rt_hw_interrupt_umask(ser_dev->irq);
            writel(0x01, ser_dev->base + UART_IER);
            break;
    }

    return RT_EOK;
}

static int serial_put_c(struct rt_serial_device *serial, char c)
{
    struct serial_device *ser_dev;
    volatile rt_uint32_t *sbuf;
    volatile rt_uint32_t *sta;

    RT_ASSERT(serial != RT_NULL);

    ser_dev = serial->parent.user_data;

    if (!ser_dev->initialized)
        return 0;

    sbuf = (rt_uint32_t *)(ser_dev->base + UART_THR);
    sta = (rt_uint32_t *)(ser_dev->base + UART_USR);

    while(!(*sta & 0x02));
    *sbuf = c;

    return 1;
}

static int serial_get_c(struct rt_serial_device *serial)
{
    int ch = -1;
    volatile rt_uint32_t *rbuf;
    volatile rt_uint32_t *sta;
    struct serial_device *ser_dev;

    RT_ASSERT(serial != RT_NULL);

    ser_dev = serial->parent.user_data;

    if (!ser_dev->initialized)
        return 0;

    rbuf = (rt_uint32_t *)(ser_dev->base + UART_RBR);
    sta = (rt_uint32_t *)(ser_dev->base + UART_USR);

    while(*sta & 0x08)
    {
        ch = *rbuf & 0xff;
    }

    return ch;
}

static rt_size_t serial_dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction)
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
        ser_dev->initialized = RT_TRUE;
    }

    return RT_EOK;
}

INIT_BOARD_EXPORT(rt_hw_uart_init);
