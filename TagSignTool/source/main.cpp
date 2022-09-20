#include "cache/cache_files.hpp"
#include "cseries/cseries_strings.hpp"
#include "tag_files/files.hpp"
#include "tag_files/files_windows.hpp"

// 256 MB max tags file size
const unsigned long tags_file_buffer_size = 256 * 1024 * 1024;
unsigned char tags_file_buffer[tags_file_buffer_size]{};

char usage[] = R"(
Usage:
	TagSignTool.exe <tags.dat path>

Example:
	TagSignTool.exe "C:\Games\Halo Online (1.106708 2015-03-20)\maps\tags.dat"
)";

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		puts(usage);

		return 1;
	}

	char const* tags_path = argv[1];
	s_file_reference tags_file{};
	unsigned long tags_file_size = 0;
	cache_file_tags_header* tags_header = nullptr;

	if (!string_ends_with(tags_path, "tags.dat"))
		return 2;

	file_reference_create_from_path(&tags_file, tags_path);
	if (!file_exists(&tags_file))
		return 3;

	file_get_size(&tags_file, &tags_file_size);
	if (!file_read_into_buffer(&tags_file, (void**)&tags_file_buffer, tags_file_buffer_size))
		return 4;

	tags_header = reinterpret_cast<cache_file_tags_header*>(tags_file_buffer);
	recalculate_checksums(tags_header);

	file_reference_set_name(&tags_file, "tags_signed.dat");
	file_create(&tags_file);

	if (!file_write_from_buffer(&tags_file, tags_file_size, tags_file_buffer))
		return 5;

	return 0;
}
