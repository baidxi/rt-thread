#ifndef _DRV_CLK_H_
#define _DRV_CLK_H_

#include <rtthread.h>

#include "rtconfig.h"

void clock_reset(rt_uint8_t clock);
void clock_enable(rt_uint8_t clock);
void clock_disable(rt_uint8_t clock);
int clock_init(void);

#ifdef SOC_ALLWINNER_R528
#include "sun8i_r528_clock.h"
#endif

#endif