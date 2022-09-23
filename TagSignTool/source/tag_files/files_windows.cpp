#include "tag_files/files_windows.hpp"
#include "cseries/cseries.hpp"

#include <windows.h>
#include <assert.h>

bool file_create(s_file_reference* file_reference)
{
	assert(file_reference);

	if ((file_reference->flags & (1 << _file_reference_flag_is_file_name)) != 0)
	{
		HANDLE handle = CreateFileA(file_reference->path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (handle && handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
			return true;
		}
	}
	else if (CreateDirectoryA(file_reference->path, NULL))
		return true;

	return false;
}

bool file_exists(s_file_reference* file_reference)
{
	assert(file_reference);

	return GetFileAttributesA(file_reference->path) != INVALID_FILE_ATTRIBUTES;
}

bool file_open(s_file_reference* file_reference, unsigned long open_flags, long* error)
{
	assert(file_reference);
	assert(error);

	bool result = false;
	unsigned long desired_access = 0;
	unsigned long share_mode = 0;
	unsigned long flags_and_attributes = FILE_READ_ATTRIBUTES;

	*error = 0;

	if ((open_flags & (1 << _file_open_flag_desired_access_read)) != 0)
		desired_access = GENERIC_READ;
	if ((open_flags & (1 << _file_open_flag_desired_access_write)) != 0)
		desired_access |= GENERIC_WRITE;

	if ((open_flags & (1 << _file_open_flag_share_mode_read)) != 0)
		share_mode |= (1 << (FILE_SHARE_READ >> 1));
	if ((open_flags & (1 << _file_open_flag_desired_access_write)) != 0)
		share_mode |= (1 << (FILE_SHARE_WRITE >> 1));

	if ((open_flags & (1 << _file_open_flag_flags_and_attributes_write)) != 0)
		flags_and_attributes = FILE_WRITE_ATTRIBUTES;
	if ((open_flags & (1 << _file_open_flag_flags_and_attributes_delete_on_close)) != 0)
		flags_and_attributes = FILE_FLAG_DELETE_ON_CLOSE;
	if ((open_flags & (1 << _file_open_flag_flags_and_attributes_random_access)) != 0)
		flags_and_attributes = FILE_FLAG_RANDOM_ACCESS;
	if ((open_flags & (1 << _file_open_flag_flags_and_attributes_sequecial_scan)) != 0)
		flags_and_attributes = FILE_FLAG_SEQUENTIAL_SCAN;

	HANDLE handle = CreateFileA(file_reference->path, desired_access, share_mode, NULL, OPEN_EXISTING, flags_and_attributes, NULL);
	if (!handle || handle == INVALID_HANDLE_VALUE)
	{
		switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
			*error = 1;
			break;
		case ERROR_PATH_NOT_FOUND:
			*error = 3;
			break;
		case ERROR_ACCESS_DENIED:
			*error = 2;
			break;
		case ERROR_INVALID_DRIVE:
			*error = 4;
			break;
		case ERROR_SHARING_VIOLATION:
			*error = 5;
			break;
		default:
			*error = 6;
			break;
		}
	}
	else
	{
		result = true;

		file_reference->handle = handle;
		file_reference->position = 0;

		if ((open_flags & (1 << _file_open_flag_set_file_end_and_close)) != 0)
		{
			file_reference->position = SetFilePointer(file_reference->handle, 0, 0, FILE_END);
			if (file_reference->position == INVALID_SET_FILE_POINTER)
			{
				CloseHandle(file_reference->handle);
				file_reference->handle = INVALID_HANDLE_VALUE;
				file_reference->position = 0;

				result = false;
			}
		}
	}

	if ((open_flags & (1 << _file_open_flag_desired_access_write)) != 0)
		file_reference->flags &= ~(1 << _file_reference_flag_open_for_write);

	return result;
}

bool file_read(s_file_reference* file_reference, unsigned long size, void** buffer)
{
	assert(file_reference);
	assert(buffer);

	unsigned long bytes_read = 0;
	bool result = false;

	if (!size)
		return true;

	if (ReadFile(file_reference->handle, *buffer, size, &bytes_read, NULL))
	{
		if (size == bytes_read)
			result = true;
		else
			SetLastError(ERROR_HANDLE_EOF);
	}

	file_reference->position += bytes_read;

	return result;
}

bool file_write(s_file_reference* file_reference, unsigned long size, void const* buffer)
{
	assert(file_reference);
	assert(size);
	assert(buffer);

	unsigned long bytes_written = 0;
	bool result = false;

	if (!size)
		return true;

	if (WriteFile(file_reference->handle, buffer, size, &bytes_written, NULL))
		result = size == bytes_written;

	file_reference->position += bytes_written;

	return result;
}

bool file_close(s_file_reference* file_reference)
{
	if (CloseHandle(file_reference->handle))
	{
		file_reference->handle = INVALID_HANDLE_VALUE;
		file_reference->position = 0;

		return true;
	}

	return false;
}

bool file_get_size(s_file_reference* file_reference, unsigned long* out_file_size)
{
	assert(file_reference);
	assert(out_file_size);

	WIN32_FILE_ATTRIBUTE_DATA file_info{};
	if (GetFileAttributesExA(file_reference->path, GetFileExInfoStandard, &file_info))
	{
		*out_file_size = file_info.nFileSizeLow;
		return true;
	}

	return false;
}

unsigned long file_get_eof(s_file_reference* file_reference)
{
	assert(file_reference);

	return GetFileSize(file_reference->handle, 0);
}


void file_path_remove_name(char* path)
{
	long i = 0;

	for (i = strlen(path); i > 0; i--)
	{
		if (path[i] == '\\')
			break;
	}
	path[i] = 0;
}

void file_path_add_name(char* path, long name_size, char const* name)
{
	assert(path && name);
	assert(name_size > 0 && name_size <= 0x100000);

	unsigned long path_size = 0;
	char* buffer = nullptr;

	if (*name)
	{
		path_size = strlen(path);
		char* buffer = &path[path_size];
		if (buffer != path && *(buffer - 1) != '\\')
		{
			*buffer++ = '\\';
			*buffer = '\0';
			++path_size;
		}

		csstrnzcpy(buffer, name, name_size - path_size);
	}
}
