#include "stdafx.h"

void file_reference_agnostic_create(s_file_reference& file_reference)
{
	//assert(file_reference);

	memset(&file_reference, 0, sizeof(s_file_reference));
	file_reference.signature = 'filo';
}

void file_reference_create(s_file_reference& file_reference)
{
	//assert(file_reference);

	file_reference_agnostic_create(file_reference);
	file_reference.handle = INVALID_HANDLE_VALUE;
}

void file_reference_set_name(s_file_reference& file_reference, char const* name)
{
	//assert(file_reference);
	assert(name);

	if (TEST_BIT(file_reference.flags, _file_reference_flag_is_file_name))
		file_path_remove_name(file_reference.path);

	file_path_add_name(file_reference.path, sizeof(file_reference.path), name);

	file_reference.flags |= FLAG(_file_reference_flag_is_file_name);
}

void file_reference_create_from_path(s_file_reference& file_reference, char const* path, bool add_directory)
{
	//assert(file_reference);
	assert(path);

	file_reference_create(file_reference);

	if (add_directory)
		file_path_add_name(file_reference.path, NUMBEROF(file_reference.path), path);
	else
		file_reference_set_name(file_reference, path);
}

bool file_read_into_buffer(s_file_reference& file_reference, void* buffer, dword size)
{
	//assert(file_reference);
	assert(buffer);

	bool result = 0;
	long error = 0;

	if (file_open(file_reference, FLAG(_file_open_flag_desired_access_read), &error))
	{
		dword file_size = file_get_eof(file_reference);
		if (file_size <= size)
			result = file_read(file_reference, file_size, buffer);

		file_close(file_reference);
	}
	return result;
}

bool file_write_from_buffer(s_file_reference& file_reference, dword size, void const* buffer)
{
	//assert(file_reference);
	assert(size);
	assert(buffer);

	bool result = 0;
	long error = 0;
	if (file_open(file_reference, FLAG(_file_open_flag_desired_access_write), &error))
	{
		result = file_write(file_reference, size, buffer);
		file_close(file_reference);
	}

	return result;
}

void* file_read_into_memory_with_null_padding(s_file_reference& file_reference, dword& size, dword pad_size)
{
	long error = 0;
	if (!file_open(file_reference, FLAG(_file_open_flag_desired_access_read), &error))
	{
		size = -1;
		return nullptr;
	}

	dword file_size = file_get_eof(file_reference);
	dword memory_size = file_size + pad_size;

	size = memory_size;

	if (!memory_size)
	{
		file_close(file_reference);
		return nullptr;
	}

	void* memory = system_malloc(memory_size);
	if (memory)
	{
		if (file_read(file_reference, file_size, memory))
		{
			if (pad_size)
			{
				memset(static_cast<byte*>(memory) + file_size, 0, pad_size);
				file_close(file_reference);
				return memory;
			}
		}
		else
		{
			system_free(memory);
			memory = 0;
		}
	}

	file_close(file_reference);
	return memory;
}

void* file_read_into_memory(s_file_reference& file_reference, dword& size)
{
	return file_read_into_memory_with_null_padding(file_reference, size, 0);
}

