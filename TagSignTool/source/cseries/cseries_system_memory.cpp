#include "cseries/cseries_system_memory.hpp"

#include <windows.h>

void* system_malloc(unsigned int size)
{
	return HeapAlloc(GetProcessHeap(), 0, size);
}

void system_free(void* memory)
{
    if (memory)
    {
        HeapFree(GetProcessHeap(), 0, memory);
    }
}

