#include <stdint.h>

struct gdtr {
    uint16_t limite;
    uint32_t base;
} __attribute__ ((packed));

struct gdtdesc {
    uint16_t lim0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t acces;
    uint8_t lim16_19:4;
    uint8_t other:4;
    uint8_t base24_31;
} __attribute__ ((packed));

struct gdtdesc kgdt[3];
struct gdtr kgdtr;

void init_gdt_desc(uint32_t base, uint32_t limite, uint8_t acces, uint8_t other, struct gdtdesc *desc)
{
    desc->lim0_15 = (limite & 0xffff);
    desc->base0_15 = (base & 0xffff);
    desc->base16_23 = (base & 0xff0000) >> 16;
    desc->acces = acces;
    desc->lim16_19 = (limite & 0xf0000) >> 16;
    desc->other = (other & 0xf);
    desc->base24_31 = (base & 0xff000000) >> 24;
    return;
}

extern void load_gdt();

void init_gdt(void)
{
    init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
    init_gdt_desc(0x0, 0xFFFFF, 0x9B, 0x0D, &kgdt[1]);    /* code */
    init_gdt_desc(0x0, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);    /* data */

    /* initialize the gdtr structure */
    kgdtr.limite = sizeof(kgdt);
    kgdtr.base = (uint32_t)&kgdt;

    load_gdt();
}