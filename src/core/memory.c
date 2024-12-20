#include "limine.h"
#include "core/limineutils.h"
#include "memory.h"
#include <stddef.h>

#define PAGE_SIZE 4096

memory_map map;

int memcmp(const void* aptr, const void* bptr, size_t size) {
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for (size_t i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

void* memmove(void* dstptr, const void* srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if (dst < src) {
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (size_t i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}

void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}

void setup_page_table() {
	struct limine_memmap_response *response = get_memory_map();

	struct limine_memmap_entry **entries = response->entries;

	map.memory_size = 0;

	for (uint64_t x = 0; x < response->entry_count; i++) {
		map.memory_size += entries[x]->length;
	}

	for (uint64_t i = 0; i < response->entry_count; i++) {
		if (entries[i]->length < PAGE_SIZE || entries[i]->type != LIMINE_MEMMAP_USABLE) {
			continue;
		}

		int pagesfromsegment = (entries[i]->length - (entries[i]->length % 4096)) / 4096;

		
	}
}
