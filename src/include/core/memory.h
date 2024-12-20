#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    bool used;
    void* start_address;
    void* end_address;
} memory_page;

typedef struct
{
    uint64_t memory_size;
    memory_page* pages;
    size_t page_count;
} memory_map;


int memcmp(const void* aptr, const void* bptr, size_t size);
void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size);
void* memmove(void* dstptr, const void* srcptr, size_t size);
void* memset(void* bufptr, int value, size_t size);
void* malloc(size_t size);
void free(void* ptr);
void setup_page_table();