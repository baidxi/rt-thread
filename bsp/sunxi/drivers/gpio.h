#ifndef _GPIO_H_
#define _GPIO_H_

#include <rtconfig.h>
#include <rtthread.h>

struct gpio_desc {
    char name[32];
};

struct gpio_ops {
    const struct gpio_desc *(*request)(rt_uint32_t pin, const char *name);
    int (*direct_output)(const struct gpio_desc *desc, int val);
    int (*direct_input)(const struct gpio_desc *desc);
    void (*release)(const struct gpio_desc *desc);
};

int gpio_controller_register(const struct gpio_ops *ops, const char *name);

const struct gpio_desc *gpio_request(rt_uint32_t pin, const char *name);
int gpio_direct_output(const struct gpio_desc *desc, int val);
int gpio_direct_input(const struct gpio_desc *desc);
void gpio_release(const struct gpio_desc *desc);

#ifdef SOC_ALLWINNER_R528
#include "sun8i_r528_gpio.h"
#endif

#endif
