#include "main/main.hpp"

#include "cache/cache_files.hpp"
#include "cache/physical_memory_map.hpp"
#include "cseries/cseries_strings.hpp"
#include "profiler/profiler.hpp"
#include "shell/shell_windows.hpp"
#include "tag_files/files.hpp"
#include "tag_files/files_windows.hpp"
#include "units/bipeds.hpp"

#include <stdio.h>
#include <windows.h>

char usage[] = R"(
Usage:
	Place TagSignTool.exe beside tags.dat or run TagSignTool.exe <tags.dat path>

Example:
	TagSignTool.exe "C:\Games\Halo Online (1.106708 2015-03-20)\maps\tags.dat"
)";

char const* tags_path = "tags.dat";

void main_loop()
{
	s_file_reference tags_file{};
	unsigned long tags_file_size = 0;
	void* tags_file_buffer = nullptr;
	cache_file_tags_header* tags_header = nullptr;
	s_windows_params* windows_params = get_windows_params();

	file_reference_create_from_path(tags_file, tags_path, false);
	if (!file_exists(tags_file))
	{
		if (!string_ends_with(windows_params->cmd_line, tags_path))
		{
			printf(usage);
			return;
		}

		tags_path = windows_params->cmd_line;
	}

	file_reference_create_from_path(tags_file, tags_path, false);
	if (!file_exists(tags_file))
		return;

	file_get_size(tags_file, tags_file_size);
	tags_file_buffer = physical_memory_malloc(tags_file_size);
	if (!file_read_into_buffer(tags_file, tags_file_buffer, tags_file_size))
		return;

	tags_header = static_cast<cache_file_tags_header*>(tags_file_buffer);

	PROFILE_FUNCTION2(modify_group, tags_header->modify_group, BIPED_TAG, modify_biped_instance_callback);
	PROFILE_FUNCTION2(sign_instances, tags_header->sign_instances);

	file_reference_set_name(tags_file, "tags_signed.dat");
	file_create(tags_file);

	if (!file_write_from_buffer(tags_file, tags_file_size, tags_header))
		return;
}

