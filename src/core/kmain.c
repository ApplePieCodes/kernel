#include "core/gdt.h"
#include "core/panic.h"
#include "core/memory.h"

void kmain() {
    init_gdt();
    setup_page_table();
    while (1);
}