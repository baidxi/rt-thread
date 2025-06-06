#ifndef _BOARD_H_
#define _BOARD_H_

#define MAX_HANDLERS    223
#define GIC_ACK_INTID_MASK  0x000003ff
#define GIC_IRQ_START   0

#define ARM_GIC_NR_IRQS     MAX_HANDLERS

#define ARM_GIC_MAX_NR      1

#define __REG32(x)  (*((volatile unsigned int *)(x)))
#define __REG16(x)  (*((volatile unsigned short *)(x)))

rt_inline rt_uint32_t platform_get_gic_dist_base(void)
{
    return 0x03021000;
}

rt_inline rt_uint32_t platform_get_gic_cpu_base(void)
{
    return 0x03022000;
}

#if defined(__CC_ARM)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN      ((void*)&Image$$RW_IRAM1$$ZI$$Limit)
#elif defined(__GNUC__)
extern int __bss_end;
#define HEAP_BEGIN      ((void*)&__bss_end)
#endif

#define HEAP_END        (void*)(0x40000000 + 128 * 1024 * 1024)

void rt_hw_board_init(void);

#endif