#pragma once

#include "cseries/cseries.hpp"
#include "objects/object_definitions.hpp"

struct _unit_definition
{
	_object_definition object;

	byte __data[0x2C8];
};
static_assert(sizeof(_unit_definition) == sizeof(_object_definition) + 0x2C8);

