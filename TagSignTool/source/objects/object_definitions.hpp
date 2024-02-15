#pragma once

#include "cseries/cseries.hpp"

struct _object_definition
{
	byte __data[0x120];
};
static_assert(sizeof(_object_definition) == 0x120);

