#ifndef _UART_H_
#define _UART_H_

#include <drivers/dev_serial.h>

#include "pinctrl.h"

struct serial_device {
    struct rt_serial_device serial_device;
    char name[RT_NAME_MAX];
    rt_uint32_t base;
    int irq;
    const struct pinctrl pinctrl[2];
    int id;
};


#endif
