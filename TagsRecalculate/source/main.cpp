#include "cache/cache_files.hpp"
#include "tag_files/files.hpp"

#include <string.h>

int main(int argc, char* argv[])
{
	if (argc < 2)
		return 1;

	char const* tags_path = argv[1];

	if (strstr(tags_path, "tags.dat") == 0)
		return 2;

	if (file_exists(tags_path) == false)
		return 3;

	cache_file_tags_header* tags_header = nullptr;
	long total_tags_size = 0;
	if (file_read_into_buffer(tags_path, reinterpret_cast<void**>(&tags_header), &total_tags_size) == false)
		return 4;

	recalculate_checksums(tags_header);

	if (file_write_from_buffer(tags_path, tags_header, total_tags_size) == false)
		return 5;

	return 0;
}
