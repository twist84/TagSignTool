#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct _object_definition
{
	static tag const k_group_tag = OBJECT_TAG;

	byte __data[0x120];
};
static_assert(sizeof(_object_definition) == 0x120);

