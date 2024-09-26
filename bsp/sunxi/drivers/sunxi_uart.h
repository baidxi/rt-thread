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

struct sunxi_serial_reg {
    union {
        uint32_t rbr;
        uint32_t thr;
        uint32_t dll;
    };
    uint32_t dlh;
    uint32_t ier;
    union {
        uint32_t iir;
        uint32_t fcr;
    };
    uint32_t lcr;
    uint32_t mcr;
    uint32_t lsr;
    uint32_t msr;
    uint32_t sch;
    uint8_t res0[0x5c];
    uint32_t usr;
    uint32_t tfl;
    uint32_t rfl;
    uint32_t hsk;
    uint32_t dma_req_en;
    uint8_t res1[0x14];
    uint32_t halt;
    uint8_t res2[0x8];
    uint32_t dbg_dll;
    uint32_t dbg_dlh;
    uint8_t res3[0x38];
    uint32_t fcc;
    uint8_t res4[0xc];
    uint32_t rxdma_ctrl;
    uint32_t rxdma_str;
    uint32_t rxdma_sta;
    uint32_t rxdma_lmt;
    uint32_t rxdma_saddrl;
    uint32_t rxdma_saddrh;
    uint32_t rxdma_bl;
    uint32_t rxdma_ie;
    uint32_t rxdma_is;
    uint32_t rxdma_waddrl;
    uint32_t rxdma_waddrh;
    uint32_t rxdma_raddrl;
    uint32_t rxdma_raddrh;
    uint32_t rxdma_dcnt;
};

#endif
