#ifndef _PINCTRL_H_
#define _PINCTRL_H_

#include <rtthread.h>

struct sunxi_pinctrl {
    rt_uint32_t pin;
    rt_uint32_t func;
    rt_uint32_t pull;
    char group;
};

void sunxi_pinctrl_init(const struct sunxi_pinctrl *pinctrl);

#endif