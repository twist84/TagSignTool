#include "cache/cache_files.hpp"

#include "memory/crc.hpp"

inline void recalculate_checksum(cache_file_tag_instance* tag_instance)
{
	tag_instance->checksum = adler32(adler_new(), tag_instance->base + 4, tag_instance->total_size - 4);
}

inline cache_file_tag_instance* tag_instance_get(cache_file_tags_header* tags_header, long tag_instance_index)
{
	if (tag_instance_index < 0 || tag_instance_index > tags_header->tag_instance_count)
		return nullptr;

	unsigned long tag_offset = reinterpret_cast<unsigned long*>(tags_header->base + tags_header->tag_instance_offsets)[tag_instance_index];
	if (tag_offset == 0xFFFFFFFF || tag_offset > sizeof(cache_file_tags_header))
		return nullptr;

	return reinterpret_cast<cache_file_tag_instance*>(tags_header->base + tag_offset);
}

inline void recalculate_checksums(cache_file_tags_header* tags_header)
{
	for (long tag_instance_index = 0; tag_instance_index < tags_header->tag_instance_count; tag_instance_index++)
	{
		cache_file_tag_instance* tag_instance = tag_instance_get(tags_header, tag_instance_index);
		if (tag_instance == nullptr)
			continue;

		recalculate_checksum(tag_instance);
	}
}