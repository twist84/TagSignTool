#include "cseries/cseries.hpp"

#include <assert.h>
#include <string.h>

long csstricmp(char const* s1, char const* s2)
{
	return _stricmp(s1, s2);
}

long csstrcmp(char const* s1, char const* s2)
{
	return strcmp(s1, s2);
}

long csstrnicmp(char const* s1, char const* s2, dword max_count)
{
	return _strnicmp(s1, s2, max_count);
}

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

char* csstrnzcat(char* s1, char const* s2, dword size)
{
	assert(s1 && s2);
	assert(size > 0 && size <= 0x100000);

	dword len = csstrnlen(s1, size);
	return csstrnzcpy(s1 + len, s2, size - len);
}

dword csstrnlen(char const* s, dword size)
{
	assert(s);
	assert(size > 0 && size <= 0x100000);

	return strnlen(s, size);
}

