#include "stdafx.h"

bool string_ends_with(char const* s1, char const* s2)
{
	assert(s1 && s2);

	dword s1_size = strlen(s1);
	dword s2_size = strlen(s2);

	return strcmp(s1 + (s1_size - s2_size), s2) == 0;
}

