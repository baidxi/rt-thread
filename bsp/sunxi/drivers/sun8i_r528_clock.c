#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "sunxi_device.h"
#include "sunxi_clock.h"


void clock_reset(rt_uint8_t clock)
{
    rt_uint32_t val = readl(CCU_BASE + UART_BGR_REG);
    val |= 1 << (clock + UART_RESET_BASE);
    writel(val, CCU_BASE + UART_BGR_REG);
}

void clock_enable(rt_uint8_t clock)
{
    rt_uint32_t val = readl(CCU_BASE + UART_BGR_REG);
    val |= 1 << clock;
    writel(val, CCU_BASE + UART_BGR_REG);
}

void clock_disable(rt_uint8_t clock)
{
    rt_uint32_t val = readl(CCU_BASE + UART_BGR_REG);
    val &= ~(1 << clock);
    writel(val, CCU_BASE + UART_BGR_REG);
}

int clock_init(void)
{
    return 0;
}