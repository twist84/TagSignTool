#include "cache/physical_memory_map.hpp"

#include <windows.h>

unsigned char* physical_memory_base = nullptr;

void physical_memory_initialize()
{
	physical_memory_base = (unsigned char*)VirtualAlloc(0, 0x2F900000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void physical_memory_free()
{
	VirtualFree(physical_memory_base, 0, 0x2F900000);
}

void* physical_memory_malloc(unsigned long size)
{
	static unsigned long total_used = 0;

	void* allocation = physical_memory_base + total_used;
	total_used += (size + 0xFFFF) & 0xFFFF0000;

	return allocation;
}
