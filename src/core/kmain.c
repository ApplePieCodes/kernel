#include "core/gdt.h"
#include "core/compatibility/sysinfo.h"

void kmain() {
    init_gdt();
    get_cpu_info();
    *(volatile int*)0xFFFFFF0E = 0xFE;
    while (1);
}