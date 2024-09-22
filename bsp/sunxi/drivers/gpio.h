#ifndef _GPIO_H_
#define _GPIO_H_

#include <rtconfig.h>
#include <rtthread.h>

enum gpio_dir {
    GPIO_OUTPUT,
    GPIO_INPUT,
};

enum gpio_group {
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH,
    GPIOI,
    GPIOJ,
    GPIOK,
};

enum gpio_pin {
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
};

struct gpio_desc {
    enum gpio_pin pin;
    enum gpio_group group;
    enum gpio_dir dir;
    rt_device_t parent;
    struct rt_device dev;
};

struct gpio_ops {
    rt_err_t (*request)(rt_device_t dev);
    rt_err_t (*direction_output)(rt_device_t dev, int val);
    int (*direction_input)(rt_device_t dev);
    void (*release)(rt_device_t dev);
    rt_err_t (*init)(struct rt_device *dev);
};

rt_err_t gpio_controller_register(const struct gpio_ops *ops, const char *name);

struct gpio_desc *gpio_request(rt_device_t dev,enum gpio_group group, enum gpio_pin pin,  enum gpio_dir dir, const char *name);
int gpio_direct_output(struct gpio_desc *desc, int val);
int gpio_direct_input(struct gpio_desc *desc);
void gpio_release(struct gpio_desc *desc);

#ifdef SOC_ALLWINNER_R528
#include "sun8i_r528_gpio.h"
#endif

#endif
