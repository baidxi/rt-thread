#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "sunxi_gpio.h"

struct gpio_device {
    struct rt_device parent;
    const struct gpio_ops *ops;
};

rt_err_t gpio_controller_register(const struct gpio_ops *ops, const char *name)
{
    struct gpio_device *dev;
    int ret;

    if (!ops->direction_input ||
        !ops->direction_output ||
        !ops->init ||
        !ops->release ||
        !ops->request)
        return -RT_EINVAL;

    dev = rt_malloc(sizeof(*dev));
    if (!dev)
        return -RT_ENOMEM;

    dev->parent.init = ops->init;

    ret = rt_device_init(&dev->parent);

    if (ret) {
        goto init_err;
    }

    dev->ops = ops;

    ret = rt_device_register(&dev->parent, name, 0);

    if (ret)
        goto init_err;

    return RT_EOK;
init_err:
    rt_free(dev);
    return ret;
}

struct gpio_desc *gpio_request(rt_device_t dev,enum gpio_group group, enum gpio_pin pin,  enum gpio_dir dir, const char *name)
{
    struct gpio_device *gpio = rt_container_of(dev, struct gpio_device, parent);
    struct gpio_desc *desc;
    int ret;

    desc = rt_malloc(sizeof(*desc));
    if (!desc)
        return RT_NULL;

    desc->group = group;
    desc->dir = dir;
    desc->pin = pin;
    desc->parent = dev;
    desc->dev.init = gpio->ops->request;

    ret = rt_device_init(&desc->dev);

    if (ret)
        goto err;

    ret = rt_device_register(&desc->dev, name, 0);

    if (ret)
        goto err;

    return desc;

err:
    rt_free(desc);
    return RT_NULL;
}

int gpio_direct_output(struct gpio_desc *desc, int val)
{
    struct gpio_device *gpio;

    if (!desc)
        return -RT_EINVAL;

    gpio = rt_container_of(desc->parent, struct gpio_device, parent);

    return gpio->ops->direction_output(&desc->dev, val);
}

int gpio_direct_input(struct gpio_desc *desc)
{
    struct gpio_device *gpio;

    if (!desc)
        return -RT_EINVAL;

    gpio = rt_container_of(desc->parent, struct gpio_device, parent);

    return gpio->ops->direction_input(&desc->dev);
}

void gpio_release(struct gpio_desc *desc)
{
    struct gpio_device *gpio;

    if (!desc)
        return;

    gpio = rt_container_of(desc->parent, struct gpio_device, parent);

    gpio->ops->release(&desc->dev);
    
    rt_device_unregister(&desc->dev);
    rt_free(desc);
}