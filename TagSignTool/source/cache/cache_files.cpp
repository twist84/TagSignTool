#include "cache/cache_files.hpp"

#include "memory/crc.hpp"

#include <stdio.h>
#include <time.h>

void cache_file_tag_instance::sign_instance()
{
	checksum = adler32(adler_new(), base + 4, total_size - 4);
}

cache_file_tag_instance* cache_file_tags_header::get_instance(long tag_instance_index)
{
	if (tag_instance_index < 0 || tag_instance_index > tag_instance_count)
		return nullptr;

	dword tag_offset = reinterpret_cast<unsigned long*>(base + tag_instance_offsets)[tag_instance_index];
	if (tag_offset == NONE || tag_offset < sizeof(cache_file_tags_header))
		return nullptr;

	return reinterpret_cast<cache_file_tag_instance*>(base + tag_offset);
}

void cache_file_tags_header::sign_instances()
{
	for (long tag_instance_index = 0; tag_instance_index < tag_instance_count; tag_instance_index++)
	{
		cache_file_tag_instance* tag_instance = get_instance(tag_instance_index);
		if (!tag_instance)
			continue;

		tag_instance->sign_instance();
	}
}

void cache_file_tags_header::modify_group(tag group_tag, void(*callback)(cache_file_tag_instance*))
{
	for (long tag_instance_index = 0; tag_instance_index < tag_instance_count; tag_instance_index++)
	{
		cache_file_tag_instance* tag_instance = get_instance(tag_instance_index);
		if (!tag_instance)
			continue;

		if (!tag_instance->tag_group.is_group(group_tag))
			continue;

		printf("Modifing group instance 0x%08X\n", tag_instance_index);

		callback(tag_instance);
	}
}

