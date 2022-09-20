#include "cache/cache_files.hpp"
#include "memory/crc.hpp"

#include <time.h>

void cache_file_tag_instance::sign_instance()
{
	checksum = adler32(adler_new(), base + 4, total_size - 4);
}

cache_file_tag_instance* cache_file_tags_header::get_instance(long tag_instance_index)
{
	if (tag_instance_index < 0 || tag_instance_index > tag_instance_count)
		return nullptr;

	unsigned long tag_offset = reinterpret_cast<unsigned long*>(base + tag_instance_offsets)[tag_instance_index];
	if (tag_offset == 0xFFFFFFFF || tag_offset < sizeof(cache_file_tags_header))
		return nullptr;

	return reinterpret_cast<cache_file_tag_instance*>(base + tag_offset);
}

void cache_file_tags_header::sign_instances(double* out_elapsed_time)
{
	time_t start;
	time_t end;

	if (out_elapsed_time)
		time(&start);

	for (long tag_instance_index = 0; tag_instance_index < tag_instance_count; tag_instance_index++)
	{
		cache_file_tag_instance* tag_instance = get_instance(tag_instance_index);
		if (tag_instance == nullptr)
			continue;

		tag_instance->sign_instance();
	}

	if (out_elapsed_time)
		time(&end);

	if (out_elapsed_time)
		*out_elapsed_time = difftime(end, start);
}
