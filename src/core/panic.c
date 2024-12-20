#include <stdint.h>
#include "core/panic.h"

void panic() {
    asm volatile("cli");  // Disable interrupts

    // Load a null IDT
    struct {
        uint16_t limit;
        uint64_t base;
    } __attribute__((packed)) null_idt = {0, 0};

    asm volatile("lidt %0" : : "m"(null_idt));

    // Trigger an interrupt (this will fail due to the invalid IDT)
    asm volatile("int $0x03");

    asm volatile("cli");
    for (;;) asm volatile("hlt");
}