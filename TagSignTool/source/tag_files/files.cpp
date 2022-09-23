#include "tag_files/files.hpp"
#include "tag_files/files_windows.hpp"

#include <windows.h>
#include <assert.h>

void file_reference_agnostic_create(s_file_reference* file_reference)
{
	assert(file_reference);

	memset(file_reference, 0, sizeof(s_file_reference));
	file_reference->signature = 'filo';
}

void file_reference_create(s_file_reference* file_reference)
{
	assert(file_reference);

	file_reference_agnostic_create(file_reference);
	file_reference->handle = INVALID_HANDLE_VALUE;
}

void file_reference_set_name(s_file_reference* file_reference, char const* name)
{
	assert(file_reference);
	assert(name);

	if ((file_reference->flags & (1 << _file_reference_flag_is_file_name)) != 0)
		file_path_remove_name(file_reference->path);
	file_path_add_name(file_reference->path, 256, name);

	file_reference->flags |= (1 << _file_reference_flag_is_file_name);
}

void file_reference_create_from_path(s_file_reference* file_reference, char const* path)
{
	assert(file_reference);
	assert(path);

	file_reference_create(file_reference);
	file_reference_set_name(file_reference, path);
}

bool file_read_into_buffer(s_file_reference* file_reference, void* buffer, unsigned long size)
{
	assert(file_reference);
	assert(buffer);

	bool result = 0;
	long error = 0;
	unsigned long file_size = 0;

	if (file_open(file_reference, (1 << _file_open_flag_desired_access_read), &error))
	{
		file_size = file_get_eof(file_reference);
		if (file_size <= size)
			result = file_read(file_reference, file_size, buffer);

		file_close(file_reference);
	}
	return result;
}

bool file_write_from_buffer(s_file_reference* file_reference, unsigned long size, void const* buffer)
{
	assert(file_reference);
	assert(size);
	assert(buffer);

	bool result = 0;
	long error = 0;
	if (file_open(file_reference, (1 << _file_open_flag_desired_access_write), &error))
	{
		result = file_write(file_reference, size, buffer);
		file_close(file_reference);
	}

	return result;
}
