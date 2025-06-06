#ifndef _SUN8I_R528_GPIO_H_
#define _SUN8I_R528_GPIO_H_

#define GPIO_BASE       0x02000000
#define GPIO_CFG(_grp, _x)    (0x30 * _grp + 0x4 * _x)

#define GPIO_DAT(_x)    (0x30 * _x + 0x10)
#define GPIO_PUPD(_x)   (0x30 * _x + 0x24)

#endif
