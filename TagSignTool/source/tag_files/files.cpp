#include "tag_files/files.hpp"

#include <windows.h>
#include <assert.h>
#include <stdio.h>

bool file_exists(char const* path)
{
	return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}

bool file_read_into_buffer(char const* path, void** out_buffer, long* out_buffer_size)
{
	assert(out_buffer);
	assert(out_buffer_size);

	FILE* file = nullptr;
	if (fopen_s(&file, path, "rb") == 0 && file)
	{
		fseek(file, 0, SEEK_END);
		long buffer_size = ftell(file);
		fseek(file, 0, SEEK_SET);

		void* buffer = malloc(buffer_size);
		assert(buffer);
		memset(buffer, 0, buffer_size);

		if (fread(buffer, sizeof(unsigned char), buffer_size, file))
		{
			*out_buffer = buffer;
			*out_buffer_size = buffer_size;
		}

		fclose(file);

		return true;
	}

	return false;
}

bool file_write_from_buffer(char const* path, void* buffer, long buffer_size)
{
	assert(buffer);
	assert(buffer_size);

	FILE* file = nullptr;
	if (fopen_s(&file, path, "wb") == 0 && file)
	{
		fwrite(buffer, sizeof(unsigned char), buffer_size, file);
		fclose(file);

		free(buffer);

		return true;
	}

	return false;
}
