#pragma once

struct s_file_last_modification_date
{
	dword date_time_low;
	dword date_time_high;
};
static_assert(sizeof(s_file_last_modification_date) == 0x8);

union cache_file_tag_instance
{
	struct
	{
		dword checksum;
		dword total_size;
		short dependency_count;
		short data_fixup_count;
		short resource_fixup_count;
		short : 16;

		// offset from `base`
		dword offset;

		s_cache_file_tag_group tag_group;
	};

#pragma warning(push)
#pragma warning(disable : 4200)
	byte base[];
#pragma warning(pop)

	void sign_instance();

	void* get()
	{
		return base + offset;
	}

	template<typename t_type>
	t_type* cast_to()
	{
		return static_cast<t_type*>(get());
	}
};
static_assert(sizeof(cache_file_tag_instance) == 0x24);

union cache_file_tags_header
{
	struct
	{
		dword : 32;

		// offset from `base`
		dword tag_instance_offsets;

		long tag_instance_count;

		dword : 32;

		s_file_last_modification_date creation_date;

		dword : 32;
		dword : 32;
	};

#pragma warning(push)
#pragma warning(disable : 4200)
	byte base[];
#pragma warning(pop)

	cache_file_tag_instance* get_instance(long tag_instance_index);
	void sign_instances(double& elapsed_time);
	void modify_group(double& elapsed_time, tag group_tag, void(*callback)(cache_file_tag_instance*));
};
static_assert(sizeof(cache_file_tags_header) == 0x20);

