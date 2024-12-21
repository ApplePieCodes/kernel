#include "core/gdt.h"
#include "core/panic.h"
#include "core/memory.h"
#include "core/limineutils.h"

void kmain() {
    init_gdt();
    
    while (1);
}