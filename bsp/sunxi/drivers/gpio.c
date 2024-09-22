#include <rtthread.h>

#include "gpio.h"

int gpio_controller_register(const struct gpio_ops *ops, const char *name)
{
    return 0;
}

const struct gpio_desc *gpio_request(rt_uint32_t pin, const char *name)
{
    return RT_NULL;
}

int gpio_direct_output(const struct gpio_desc *desc, int val)
{
    return 0;
}

int gpio_direct_input(const struct gpio_desc *desc)
{
    return 0;
}

void gpio_release(const struct gpio_desc *desc)
{

}