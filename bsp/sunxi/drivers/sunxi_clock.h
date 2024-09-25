#ifndef _DRV_CLK_H_
#define _DRV_CLK_H_

#include <rtthread.h>

#include "rtconfig.h"

enum sunxi_ccu_bus {
    SUNXI_CCU_BUS_DE,
    SUNXI_CCU_BUS_DI,
    SUNXI_CCU_BUS_G2D,
    SUNXI_CCU_BUS_CE,
    SUNXI_CCU_BUS_VE,
    SUNXI_CCU_BUS_DMA,
    SUNXI_CCU_BUS_MSGBOX,
    SUNXI_CCU_BUS_SPINLOCK,
    SUNXI_CCU_BUS_HSTIMER,
    SUNXI_CCU_BUS_DBGSYS,
    SUNXI_CCU_BUS_PWM,
    SUNXI_CCU_BUS_IOMMU,
    SUNXI_CCU_BUS_MBUS,
    SUNXI_CCU_BUS_DRAM,
    SUNXI_CCU_BUS_SDHCI,
    SUNXI_CCU_BUS_UART,
    SUNXI_CCU_BUS_TWI,
    SUNXI_CCU_BUS_SPI,
    SUNXI_CCU_BUS_EMAC,
    SUNXI_CCU_BUS_IRTX,
    SUNXI_CCU_BUS_GPADC,
    SUNXI_CCU_BUS_THS,
    SUNXI_CCU_BUS_I2S,
    SUNXI_CCU_BUS_OWA,
    SUNXI_CCU_BUS_DMIC,
    SUNXI_CCU_BUS_AUDIO,
    SUNXI_CCU_BUS_USB,
    SUNXI_CCU_BUS_DPSS_TOP,
    SUNXI_CCU_BUS_DSI,
    SUNXI_CCU_BUS_TCONLCD,
    SUNXI_CCU_BUS_TCONTV,
    SUNXI_CCU_BUS_TVE,
    SUNXI_CCU_BUS_LEDC,
    SUNXI_CCU_BUS_CSI,
    SUNXI_CCU_BUS_TPADC,
    SUNXI_CCU_BUS_DSP,
};

void clock_reset(rt_uint8_t clock);
void clock_enable(rt_uint8_t clock);
void clock_disable(rt_uint8_t clock);
int clock_init(void);

#ifdef SOC_ALLWINNER_R528
#include "sun8i_r528_clock.h"
#endif

#endif