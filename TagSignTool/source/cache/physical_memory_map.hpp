#pragma once

#include "cseries/cseries.hpp"

void physical_memory_initialize();
void physical_memory_free();
void* physical_memory_malloc(dword size);

