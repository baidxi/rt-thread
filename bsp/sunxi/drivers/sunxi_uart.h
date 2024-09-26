#ifndef _UART_H_
#define _UART_H_

#include <drivers/dev_serial.h>

#include "sunxi_pinctrl.h"

struct serial_device {
    struct rt_serial_device serial_device;
    char name[RT_NAME_MAX];
    unsigned long base;
    int irq;
    const struct sunxi_pinctrl pinctrl[2];
    int id;
    rt_bool_t initialized;
};


#endif
