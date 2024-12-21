#include "core/gdt.h"
#include "core/panic.h"
#include "core/memory.h"
#include "core/limineutils.h"

void kmain() {
    init_gdt();
    *(int*)(thing() + 0xFF82) = 0x82;
    while (1);
}