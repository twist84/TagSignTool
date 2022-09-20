#include "cseries/cseries.hpp"

#include <assert.h>
#include <string.h>

char* csstrnzcpy(char* s1, char const* s2, unsigned long s1_size)
{
	assert(s1 && s2);
	assert(s1_size > 0 && s1_size <= 0x100000);

	unsigned long s2_size = strnlen(s2, s1_size);
	strncpy_s(s1, s1_size, s2, 0xFFFFFFFF);

	if (s2_size >= s1_size)
		s1[s1_size - 1] = 0;
	else
		memset(&s1[s2_size], 0, s1_size - s2_size);

	return s1;
}