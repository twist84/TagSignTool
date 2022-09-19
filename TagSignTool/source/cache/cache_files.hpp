#pragma once

struct s_file_last_modification_date
{
	unsigned long date_time_low;
	unsigned long date_time_high;
};
static_assert(sizeof(s_file_last_modification_date) == 0x8);

union cache_file_tag_instance
{
	struct
	{
		unsigned long checksum;
		unsigned long total_size;
		short dependency_count;
		short data_fixup_count;
		short resource_fixup_count;
		short : 16;

		// offset from `base`
		unsigned long offset;

		unsigned long group_tags[3];
		unsigned long group_name;
	};

#pragma warning(push)
#pragma warning(disable : 4200)
	unsigned char base[];
#pragma warning(pop)
};
static_assert(sizeof(cache_file_tag_instance) == 0x24);

extern void recalculate_checksum(cache_file_tag_instance* tag_instance);

union cache_file_tags_header
{
	struct
	{
		unsigned long : 32;

		// offset from `base`
		unsigned long tag_instance_offsets;

		long tag_instance_count;

		unsigned long : 32;

		s_file_last_modification_date creation_date;

		unsigned long : 32;
		unsigned long : 32;
	};

#pragma warning(push)
#pragma warning(disable : 4200)
	unsigned char base[];
#pragma warning(pop)

};
static_assert(sizeof(cache_file_tags_header) == 0x20);

extern cache_file_tag_instance* tag_instance_get(cache_file_tags_header* tags_header, long tag_instance_index);
extern void recalculate_checksums(cache_file_tags_header* tags_header);