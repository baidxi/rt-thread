#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "gpio.h"
#include "device.h"


static rt_err_t sun8i_r528_gpio_request(rt_device_t dev)
{
    struct gpio_desc *desc = rt_container_of(dev, struct gpio_desc, dev);
    rt_uint32_t cfg_val;

    cfg_val = readl(GPIO_BASE + desc->group * 0x30);

    cfg_val &= ~(0xf << (desc->pin * 4));
    cfg_val |= desc->dir << (desc->pin * 4);

    writel(cfg_val, GPIO_BASE + desc->group * 0x30);

    return RT_EOK;
}

static rt_err_t sun8i_r528_gpio_direction_output(rt_device_t dev, int val)
{
    struct gpio_desc *desc = rt_container_of(dev, struct gpio_desc, dev);
    rt_uint32_t v;
    rt_uint32_t tv = !!val;

    v = readl(GPIO_BASE + desc->group * 0x30 + 0x10);

    v &= ~(1 << desc->pin);
    v |= tv << desc->pin;

    writel(v, GPIO_BASE + desc->group * 0x30 + 0x10);

    return RT_EOK;
}

static int sun8i_r528_gpio_direction_input(rt_device_t dev)
{
    struct gpio_desc *desc = rt_container_of(dev, struct gpio_desc, dev);
    rt_uint32_t val;

    val = readl(GPIO_BASE + desc->group * 0x30 + 0x10) & (1 << desc->pin);

    return val;
}

static void sun8i_r528_gpio_release(rt_device_t dev)
{
    struct gpio_desc *desc = rt_container_of(dev, struct gpio_desc, dev);
    rt_uint32_t val;

    val = readl(GPIO_BASE + desc->group * 0x30);

    val |= 0xf << (desc->pin * 4);

    writel(val, GPIO_BASE + desc->group * 0x30);
}

static rt_err_t sun8i_r528_gpio_controller_init(rt_device_t dev)
{
    return RT_EOK;
}

static const struct gpio_ops ops = {
    .request = sun8i_r528_gpio_request,
    .direction_output = sun8i_r528_gpio_direction_output,
    .direction_input = sun8i_r528_gpio_direction_input,
    .release = sun8i_r528_gpio_release,
    .init = sun8i_r528_gpio_controller_init,
};

static int sun8i_r528_gpio_subsys_init(void)
{
    return gpio_controller_register(&ops, "gpio device");
}

INIT_SUBSYS_EXPORT(sun8i_r528_gpio_subsys_init);
