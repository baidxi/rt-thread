#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "gpio.h"


static rt_err_t sun8i_r528_gpio_request(rt_device_t dev)
{
    return RT_EOK;
}

static rt_err_t sun8i_r528_gpio_direction_output(rt_device_t dev, int val)
{
    return RT_EOK;
}

static int sun8i_r528_gpio_direction_input(rt_device_t dev)
{
    return 0;
}

static void sun8i_r528_gpio_release(rt_device_t dev)
{

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
