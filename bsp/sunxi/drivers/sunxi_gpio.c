#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "sunxi_gpio.h"
#include "sunxi_device.h"

static void sunxi_gpio_pin_mode(struct rt_device *device, rt_base_t pin, rt_base_t mode)
{
    struct sunxi_gpio *gpio = device->user_data;
    rt_uint32_t val;
    rt_uint32_t bank = pin / 32;
    rt_uint32_t p = pin % 32;

    switch(mode) {
        case PIN_MODE_OUTPUT:
            val = readl(gpio->hw_base + bank * 0x30);
            val &= ~(0xf << (p * 0x4));
            val |= 1 << (p * 0x4);
            writel(val, gpio->hw_base + bank * 0x30);
            break;
        case PIN_MODE_INPUT:
            val = readl(gpio->hw_base + bank * 0x30);
            val &= ~(0xf << (p * 0x4));
            writel(val, gpio->hw_base + bank * 0x30);
            break;
        case PIN_MODE_INPUT_PULLUP:
            val = readl(gpio->hw_base + bank * 0x30 + 0x24);
            val &= ~(0x3 << (p * 0x2));
            val |= 1 << (p * 0x2);
            writel(val, gpio->hw_base + bank * 0x30 + 0x24);
            break;
        case PIN_MODE_INPUT_PULLDOWN:
            val = readl(gpio->hw_base + bank * 0x30 + 0x24);
            val &= ~(0x3 << (p * 0x2));
            val |= 2 << (p * 0x2);
            writel(val, gpio->hw_base + bank * 0x30 + 0x24);
            break;
    }
}

static void sunxi_gpio_pin_write(struct rt_device *device, rt_base_t pin, rt_base_t value)
{
    struct sunxi_gpio *gpio = device->user_data;
    rt_uint32_t val;
    rt_uint32_t bank = pin / 32;
    rt_uint32_t p = pin % 32;

    val = readl(gpio->hw_base + bank * 0x30 + 0x10);
    val &= ~(1 << p);
    val |= 1 << p;

    writel(val, gpio->hw_base + bank * 0x30 + 0x10);
}

static int sunxi_gpio_pin_read(struct rt_device *device, rt_base_t pin)
{
    struct sunxi_gpio *gpio = device->user_data;
    rt_uint32_t val;
    rt_uint32_t bank = pin / 32;
    rt_uint32_t p = pin % 32;

    val = readl(gpio->hw_base + bank * 0x30 + 0x10);
    return (val & 1 << p);
}

static rt_err_t sunxi_gpio_pin_attach_irq(struct rt_device *device, rt_int32_t pin,
                      rt_uint32_t mode, void (*hdr)(void *args), void *args)
{
    return RT_EOK;
}

static rt_err_t sunxi_gpio_pin_detach_irq(struct rt_device *device, rt_int32_t pin)
{
    return RT_EOK;
}

static rt_err_t sunxi_gpio_pin_irq_enable(struct rt_device *device, rt_base_t pin, rt_uint32_t enabled)
{
    return RT_EOK;
}

static rt_base_t sunxi_gpio_pin_get(struct rt_device *device, const char *name)
{
    struct sunxi_gpio *gpio = device->user_data;

    char *str_pin = rt_strstr(name, ".") + 1;

    gpio->pin = str_pin[0] - 0x30;
    gpio->bank = name[1] - 0x41;

    return gpio->invalid_pin(gpio) ? (gpio->bank * 32 + gpio->pin) : -1;
}

static const struct rt_pin_ops sunxi_gpio_ops = {
    .pin_mode = sunxi_gpio_pin_mode,
    .pin_write = sunxi_gpio_pin_write,
    .pin_read = sunxi_gpio_pin_read,
    .pin_attach_irq = sunxi_gpio_pin_attach_irq,
    .pin_detach_irq = sunxi_gpio_pin_detach_irq,
    .pin_irq_enable = sunxi_gpio_pin_irq_enable,
    .pin_get = sunxi_gpio_pin_get,
};

int sunxi_gpio_init(struct sunxi_gpio *gpio)
{
    return rt_device_pin_register("gpio0", &sunxi_gpio_ops, gpio);
}
