#include <rthw.h>
#include <rtthread.h>

#include "board.h"

#include <mmu.h>

struct mem_desc platform_mem_desc[] = {
    {0x00000000, 0xFFFFFFFF, 0x00000000, DEVICE_MEM},
    {0x00000000, 0x000FFFFF, 0x00000000, NORMAL_MEM},
    {0x40000000, 0x47FFFFFF, 0x40000000, NORMAL_MEM},
};

const rt_uint32_t platform_mem_desc_size = sizeof(platform_mem_desc)/sizeof(platform_mem_desc[0]);

#define SYS_CTRL                        __REG32(REALVIEW_SCTL_BASE)

extern void rt_hw_ipi_handler_install(int ipi_vector, rt_isr_handler_t ipi_isr_handler);

void idle_wfi(void)
{
    asm volatile ("wfi");
}

void rt_hw_board_init(void)
{
    /* initialize hardware interrupt */
    rt_hw_interrupt_init();
    /* initialize system heap */
    rt_system_heap_init(HEAP_BEGIN, HEAP_END);

    rt_components_board_init();
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);

    rt_thread_idle_sethook(idle_wfi);

#ifdef RT_USING_SMP
    /* install IPI handle */
    rt_hw_ipi_handler_install(RT_SCHEDULE_IPI, rt_scheduler_ipi_handler);
#endif
}