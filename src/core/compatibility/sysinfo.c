#include <stdint.h>
#include <stdbool.h>
#include "core/compatibility/sysinfo.h"

char cpu_name[15];
char cpu_model[50];
uint16_t cpu_cores;
uint16_t cpu_threads;

void cpuid(uint32_t eax, uint32_t ecx, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
    __asm__ volatile (
        "cpuid"
        : "=a" (*a), "=b" (*b), "=c" (*c), "=d" (*d)
        : "a" (eax), "c" (ecx)
    );
}

char* get_info() {
    return &cpu_name[0];
}

void get_cpu_info() {
    uint32_t eax, ebx, ecx, edx;

    // Get CPU name using CPUID functions 0x80000002 to 0x80000004
    char name_buffer[49] = {0};

    cpuid(0x80000002, 0, &eax, &ebx, &ecx, &edx);
    *((uint32_t*)&name_buffer[0]) = eax;
    *((uint32_t*)&name_buffer[4]) = ebx;
    *((uint32_t*)&name_buffer[8]) = ecx;
    *((uint32_t*)&name_buffer[12]) = edx;

    cpuid(0x80000003, 0, &eax, &ebx, &ecx, &edx);
    *((uint32_t*)&name_buffer[16]) = eax;
    *((uint32_t*)&name_buffer[20]) = ebx;
    *((uint32_t*)&name_buffer[24]) = ecx;
    *((uint32_t*)&name_buffer[28]) = edx;

    cpuid(0x80000004, 0, &eax, &ebx, &ecx, &edx);
    *((uint32_t*)&name_buffer[32]) = eax;
    *((uint32_t*)&name_buffer[36]) = ebx;
    *((uint32_t*)&name_buffer[40]) = ecx;
    *((uint32_t*)&name_buffer[44]) = edx;

    // Copy to the cpu_name array (truncate to fit)
    for (int i = 0; i < 15 && name_buffer[i] != '\0'; i++) {
        cpu_name[i] = name_buffer[i];
    }

    // For demonstration, model is assumed same as name (can adapt if model parsing logic differs)
    for (int i = 0; i < 50 && name_buffer[i] != '\0'; i++) {
        cpu_model[i] = name_buffer[i];
    }

    // Get number of threads and cores
    cpuid(1, 0, &eax, &ebx, &ecx, &edx);

    cpu_threads = (ebx >> 16) & 0xFF; // Logical processors
    bool hyper_threading = (edx >> 28) & 1;

    // Use CPUID leaf 0xB (extended topology) if available
    cpuid(0xB, 0, &eax, &ebx, &ecx, &edx);
    if (ebx != 0) {
        cpu_cores = ebx & 0xFFFF; // Number of cores in the first subleaf
    } else {
        cpu_cores = hyper_threading ? cpu_threads / 2 : cpu_threads;
    }
}
