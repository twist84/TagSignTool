#include "stdafx.h"

char usage[] = R"(
Usage:
	TagSignTool.exe <tags.dat path>

Example:
	TagSignTool.exe "C:\Games\Halo Online (1.106708 2015-03-20)\maps\tags.dat"
)";

int main(int argc, char* argv[])
{
	physical_memory_initialize();

	char const* tags_path = "tags.dat";

	if (argc < 2)
	{
		s_file_reference tags_file{};
		file_reference_create_from_path(tags_file, tags_path, false);
		if (!file_exists(tags_file))
		{
			printf(usage);

			return 1;
		}
		else
		{

		}
	}

	int result = main_body(tags_path);

	physical_memory_free();

	return result;
}

void modify_biped_instance_callback(cache_file_tag_instance* instance)
{
	if (!instance)
		return;

	if (instance->tag_group != BIPED_TAG)
		return;

	_biped_definition* biped = instance->cast_to<_biped_definition>();
	if (!biped)
		return;

	// "edge drop" fix
	biped->physics.ground_physics.scale_ground_adhesion_velocity = 30.0f / 60;
}

int main_body(char const* tags_path)
{
	s_file_reference tags_file{};
	unsigned long tags_file_size = 0;
	void* tags_file_buffer = nullptr;
	cache_file_tags_header* tags_header = nullptr;
	double elapsed_time = 0.0;

	if (!string_ends_with(tags_path, "tags.dat"))
		return 2;

	file_reference_create_from_path(tags_file, tags_path, false);
	if (!file_exists(tags_file))
		return 3;

	file_get_size(tags_file, tags_file_size);
	tags_file_buffer = physical_memory_malloc(tags_file_size);
	if (!file_read_into_buffer(tags_file, tags_file_buffer, tags_file_size))
		return 4;

	tags_header = static_cast<cache_file_tags_header*>(tags_file_buffer);
	tags_header->modify_group(elapsed_time, BIPED_TAG, modify_biped_instance_callback);
	printf("Modifing group instances took %.2lf seconds.\n", elapsed_time);

	elapsed_time = 0;
	tags_header->sign_instances(elapsed_time);
	printf("Signing all instances took %.2lf seconds.\n", elapsed_time);

	file_reference_set_name(tags_file, "tags_signed.dat");
	file_create(tags_file);

	if (!file_write_from_buffer(tags_file, tags_file_size, tags_header))
		return 5;

	return 0;
}

