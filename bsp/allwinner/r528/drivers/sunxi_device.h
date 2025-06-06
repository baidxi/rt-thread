#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <rtconfig.h>
#include <rtdevice.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)    (sizeof(x) / sizeof(x[0]))
#endif

#define readl(addr)           (*(volatile unsigned int *)(addr))
#define writel(value,addr)    (*(volatile unsigned int *)(addr) = (value))
#define write32(addr, value)    (*(volatile unsigned int *)(addr) = (value))

#endif