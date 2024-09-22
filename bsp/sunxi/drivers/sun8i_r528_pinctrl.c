#include <rthw.h>
#include <rtthread.h>

#include "device.h"
#include "pinctrl.h"
#include "gpio.h"

void sunxi_pinctrl_init(const struct pinctrl *pinctrl)
{
    rt_uint32_t cfg_val;
    rt_uint32_t pull_val;

    cfg_val = readl(GPIO_BASE + GPIO_CFG(pinctrl->group, 0));
    pull_val = readl(GPIO_BASE + GPIO_PUPD(pinctrl->group));

    cfg_val &= ~(0xf << (pinctrl->pin * 4));
    cfg_val |= (pinctrl->func << (pinctrl->pin * 4));

    writel(cfg_val, GPIO_BASE + GPIO_CFG(pinctrl->group, 0));

    if (pinctrl->pull) {
        pull_val &= ~(0x3 << (pinctrl->pin * 2));
        pull_val |= pinctrl->pull << (pinctrl->pull * 2);
        writel(pull_val, GPIO_BASE + GPIO_PUPD(pinctrl->group));
    }
}