#include <rthw.h>
#include <rtdevice.h>
#include <rtconfig.h>

#include "drivers/mmcsd_host.h"
#include "sunxi_sdhci.h"
#include "sunxi_pinctrl.h"
#include "sunxi_device.h"
#include "sunxi_gpio.h"

struct sunxi_sdhci_device {
    struct rt_device parent;
    struct sunxi_sdhci *sdhci;
};

struct sunxi_sdhci_reg {
    rt_uint32_t ctrl;
    rt_uint32_t clk_div;
    rt_uint32_t time_out;
    rt_uint32_t bus_with;
    rt_uint32_t blk_size;
    rt_uint32_t byte_count;
    rt_uint32_t cmd;
    rt_uint32_t cmd_arg;
    rt_uint32_t resp0;
    rt_uint32_t resp1;
    rt_uint32_t resp2;
    rt_uint32_t resp3;
    rt_uint32_t int_mask;
    rt_uint32_t masked_int_status;
    rt_uint32_t raw_int_status;
    rt_uint32_t status;
    rt_uint32_t fifo_water_level;
    rt_uint32_t fifo_func;
    rt_uint32_t tcbcnt;
    rt_uint32_t tbbcnt;
    rt_uint32_t dbgc;
    rt_uint32_t csdc;
    rt_uint32_t a12a;
    rt_uint32_t ntsc;   /* 0x005c*/
    rt_uint8_t res0[0x18];
    rt_uint32_t hwrst;
    rt_uint32_t idmac;
    rt_uint32_t dlba;
    rt_uint32_t idst;
    rt_uint32_t idie;
    rt_uint8_t res1[0x70];
    rt_uint32_t thld;
    rt_uint32_t sfc;
    rt_uint32_t a23a;
    rt_uint32_t ddr_sbit_det;
    rt_uint8_t res2[0x28];
    rt_uint32_t ext_cmd;
    rt_uint32_t ext_resp;
    rt_uint32_t drv_dl;
    rt_uint32_t smap_dl;
    rt_uint32_t ds_dl;
    rt_uint32_t hs400_dl;
    rt_uint8_t res3[0xb0];
    rt_uint32_t fifo;
};

static struct sunxi_sdhci sdhci_list[] = {
#ifdef RT_USING_SDHCI0
    {
        .name = "SDC0",
        .hw_base = 0x04020000,
        .data_width = 4,
        .freq_min = 400 * 1000,
        .freq_max = 50 * 1000 * 1000,
        .pinctrl[SUNXI_SDC_DAT1] = {
            .group = GPIOF,
            .pin = GPIO_PIN_0,
            .func = 2,
        },
        .pinctrl[SUNXI_SDC_DAT0] = {
            .group = GPIOF,
            .pin = GPIO_PIN_1,
            .func = 2,
        },
        .pinctrl[SUNXI_SDC_CLK] = {
            .group = GPIOF,
            .pin = GPIO_PIN_2,
            .func = 2,
        },
        .pinctrl[SUNXI_SDC_CMD] = {
            .group = GPIOF,
            .pin = GPIO_PIN_3,
            .func = 2,
        },
        .pinctrl[SUNXI_SDC_DAT3] = {
            .group = GPIOF,
            .pin = GPIO_PIN_4,
            .func = 2,
        },
        .pinctrl[SUNXI_SDC_DAT2] = {
            .group = GPIOF,
            .pin = GPIO_PIN_5,
            .func = 2,
        }
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

static rt_err_t sunxi_sdhci_device_init(rt_device_t dev)
{
    struct sunxi_sdhci_device *sdhci_dev = rt_container_of(dev, struct sunxi_sdhci_device, parent);
    struct sunxi_sdhci *sdhci = sdhci_dev->sdhci;
    struct rt_mmcsd_host *host;
    int i;

    host = mmcsd_alloc_host();

    if (!host)
        return -RT_ENOMEM;

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

    for (i = 0; i < ARRAY_SIZE(sdhci->pinctrl); i++) {
        const struct sunxi_pinctrl *pin = &sdhci->pinctrl[i];
        if (!pin->group)
            return -RT_EINVAL;

        sunxi_pinctrl_init(pin);
    }

    return RT_EOK;
}

static rt_err_t sunxi_sdhci_device_open(rt_device_t dev, rt_uint16_t oflag)
{
    struct sunxi_sdhci_device *sdhci_device = rt_container_of(dev, struct sunxi_sdhci_device, parent);
    struct sunxi_sdhci *sdhci = sdhci_device->sdhci;
    struct rt_mmcsd_host *host = sdhci->host;

    mmcsd_change(host);

    return RT_EOK;
}

static rt_err_t sunxi_sdhci_device_close(rt_device_t dev)
{
    struct sunxi_sdhci_device *sdhci_device = rt_container_of(dev, struct sunxi_sdhci_device, parent);
    struct sunxi_sdhci *sdhci = sdhci_device->sdhci;
    struct rt_mmcsd_host *host = sdhci->host;

    mmcsd_free_host(host);

    return RT_EOK;
}

static int sunxi_sdhci_init(void)
{
    int i = 0;
    struct sunxi_sdhci_device *dev;

    char name[8];
    rt_err_t ret;

    for (i = 0; i < ARRAY_SIZE(sdhci_list); i++) {
        dev = rt_malloc(sizeof(*dev));
        if (!dev)
            return -RT_ENOMEM;

        dev->parent.init = sunxi_sdhci_device_init;
        dev->parent.open = sunxi_sdhci_device_open;
        dev->parent.close = sunxi_sdhci_device_close;

        dev->parent.type = RT_Device_Class_SDIO;
        dev->sdhci = &sdhci_list[i];

        rt_snprintf(name, sizeof(name), "sdhci%d", i);
        ret = rt_device_register(&dev->parent, name, 0);
        if (ret) {
            rt_free(dev);
            break;
        }
    }
    return 0;
}

INIT_BOARD_EXPORT(sunxi_sdhci_init);
