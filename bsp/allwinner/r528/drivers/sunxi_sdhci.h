#ifndef _SUNXI_SDHCI_H_
#define _SUNXI_SDHCI_H_

#include <rtdevice.h>

#include "rtdef.h"
#include "sunxi_pinctrl.h"

#define SUNXI_SDC_CLK   0
#define SUNXI_SDC_CMD   1
#define SUNXI_SDC_DAT0  2
#define SUNXI_SDC_DAT1  3
#define SUNXI_SDC_DAT2  4
#define SUNXI_SDC_DAT3  5
struct sunxi_sdhci {
    struct rt_mmcsd_host *host;
    unsigned long hw_base;
    unsigned long voltage;
    rt_uint8_t data_width;
    rt_uint32_t freq_min;
    rt_uint32_t freq_max;
    rt_bool_t removeable;
    const struct sunxi_pinctrl pinctrl[6];
    const char name[8];
    #ifdef CONFIG_MMC_USE_DMA
    ALIGN(32) static rt_uint8_t dma_buffer[64 * 1024];
    #endif
    int irq;
};

#endif
