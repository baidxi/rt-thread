#include <rthw.h>
#include <rtdevice.h>
#include <rtconfig.h>

#include "sunxi_sdhci.h"
#include "sunxi_pinctrl.h"
#include "sunxi_device.h"

static struct sunxi_sdhci sdhci_list[] = {
#ifdef RT_USING_SDHCI0
    {
        .name = "SDC0",
        .hw_base = 0x04020000,
        .data_width = 4,
        .freq_min = 400 * 1000,
        .freq_max = 50 * 1000 * 1000,
    },
#endif
#ifdef RT_USING_SDHCI1
    {
        .name = "SDC1",
        .hw_base = 0x04021000,
        .data_width = 4,
    },
#endif
#ifdef RT_USING_SDHCI2
    {
        .name = "SDC2",
        .hw_base = 0x04022000,
        .data_width = 4,
    },
#endif
};

static void sunxi_sdhci_request(struct rt_mmcsd_host *host, struct rt_mmcsd_req *req)
{

}

static void sunxi_sdhci_set_iocfg(struct rt_mmcsd_host *host, struct rt_mmcsd_io_cfg *io_cfg)
{

}

static rt_int32_t sunxi_sdhci_get_card_status(struct rt_mmcsd_host *host)
{
    return 0;
}

static void sunxi_sdhci_enable_irq(struct rt_mmcsd_host *host, rt_int32_t en)
{

}

static const struct rt_mmcsd_host_ops ops = {
    .request = sunxi_sdhci_request,
    .set_iocfg = sunxi_sdhci_set_iocfg,
    .get_card_status = sunxi_sdhci_get_card_status,
    .enable_sdio_irq = sunxi_sdhci_enable_irq,
};

int sunxi_sdhci_init(void)
{
    int i = 0, j = 0;
    struct sunxi_sdhci *sdhci;
    struct rt_mmcsd_host *host;

    for (i = 0; i < ARRAY_SIZE(sdhci_list); i++) {
        sdhci = &sdhci_list[i];
        host = mmcsd_alloc_host();
        if (!host)
            break;

        host->private_data = sdhci;
        host->ops = &ops;

        host->valid_ocr = VDD_26_27 | VDD_27_28 | VDD_28_29 | VDD_29_30 | VDD_30_31 | VDD_31_32 |
        VDD_32_33 | VDD_33_34 | VDD_34_35 | VDD_35_36;
        host->flags = MMCSD_BUSWIDTH_4 | MMCSD_MUTBLKWRITE | MMCSD_SUP_SDIO_IRQ | MMCSD_SUP_HIGHSPEED;
        host->max_seg_size = 2048;
        host->max_dma_segs = 10;
        host->max_blk_size = 512;
        host->max_blk_count = 4096;

        host->freq_min = sdhci->freq_min;
        host->freq_max = sdhci->freq_max;

        sdhci->host = host;

        for (j = 0; j < ARRAY_SIZE(sdhci->pinctrl); j++) {
            const struct sunxi_pinctrl *pin = &sdhci->pinctrl[j];
            if (!pin->group)
                continue;

            sunxi_pinctrl_init(pin);
        }

        mmcsd_change(host);
    }

    return 0;
}

INIT_APP_EXPORT(sunxi_sdhci_init);