#include "stdafx.h"

bool file_create_parent_directories_if_not_present(s_file_reference const& file_reference)
{
	char temp[MAX_PATH]{};
	csstrnzcpy(temp, file_reference.path, NUMBEROF(temp));

	char path[MAX_PATH]{};

	char* part0 = temp;
	while (char* part1 = strchr(part0, '\\'))
	{
		*part1 = 0;

		csstrnzcat(path, part0, NUMBEROF(path));
		csstrnzcat(path, "\\", NUMBEROF(path));
		part0 = part1 + 1;

		s_file_reference directory{};
		file_reference_create_from_path(directory, path, true);
		if (!file_exists(directory))
		{
			bool file_create_result = file_create(directory);
			if (!file_create_result)
				return file_create_result;
		}
	}

	return true;
}

bool file_create(s_file_reference& file_reference)
{
	//assert(file_reference);

	if (TEST_BIT(file_reference.flags, _file_reference_flag_is_file_name))
	{
		HANDLE handle = CreateFileA(file_reference.path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (handle && handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
			return true;
		}
	}
	else if (CreateDirectoryA(file_reference.path, NULL))
	{
		return true;
	}

	return false;
}

bool file_exists(s_file_reference& file_reference)
{
	//assert(file_reference);

	return GetFileAttributesA(file_reference.path) != INVALID_FILE_ATTRIBUTES;
}

bool file_open(s_file_reference& file_reference, dword open_flags, long* error)
{
	//assert(file_reference);
	assert(error);

	bool result = false;
	dword desired_access = 0;
	dword share_mode = 0;
	dword flags_and_attributes = FILE_READ_ATTRIBUTES;

	*error = 0;

	if (TEST_BIT(open_flags, _file_open_flag_desired_access_read))
		desired_access = GENERIC_READ;
	if (TEST_BIT(open_flags, _file_open_flag_desired_access_write))
		desired_access |= GENERIC_WRITE;

	if (TEST_BIT(open_flags, _file_open_flag_share_mode_read))
		share_mode |= FLAG((FILE_SHARE_READ >> 1));
	if (TEST_BIT(open_flags, _file_open_flag_desired_access_write))
		share_mode |= FLAG((FILE_SHARE_WRITE >> 1));

	if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_write))
		flags_and_attributes = FILE_WRITE_ATTRIBUTES;
	if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_delete_on_close))
		flags_and_attributes = FILE_FLAG_DELETE_ON_CLOSE;
	if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_random_access))
		flags_and_attributes = FILE_FLAG_RANDOM_ACCESS;
	if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_sequecial_scan))
		flags_and_attributes = FILE_FLAG_SEQUENTIAL_SCAN;

	HANDLE handle = CreateFileA(file_reference.path, desired_access, share_mode, NULL, OPEN_EXISTING, flags_and_attributes, NULL);
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

		file_reference.handle = handle;
		file_reference.position = 0;

		if (TEST_BIT(open_flags, _file_open_flag_set_file_end_and_close))
		{
			file_reference.position = SetFilePointer(file_reference.handle, 0, 0, FILE_END);
			if (file_reference.position == INVALID_SET_FILE_POINTER)
			{
				CloseHandle(file_reference.handle);
				file_reference.handle = INVALID_HANDLE_VALUE;
				file_reference.position = 0;

				result = false;
			}
		}
	}

	if (TEST_BIT(open_flags, _file_open_flag_desired_access_write))
		file_reference.flags &= ~FLAG(_file_reference_flag_open_for_write);

	return result;
}

bool file_read(s_file_reference& file_reference, dword size, void* buffer)
{
	//assert(file_reference);
	assert(buffer);

	dword bytes_read = 0;
	bool result = false;

	if (!size)
		return true;

	if (ReadFile(file_reference.handle, buffer, size, &bytes_read, NULL))
	{
		if (size == bytes_read)
			result = true;
		else
			SetLastError(ERROR_HANDLE_EOF);
	}

	file_reference.position += bytes_read;

	return result;
}

bool file_write(s_file_reference& file_reference, dword size, void const* buffer)
{
	//assert(file_reference);
	assert(size);
	assert(buffer);

	dword bytes_written = 0;
	bool result = false;

	if (!size)
		return true;

	if (WriteFile(file_reference.handle, buffer, size, &bytes_written, NULL))
		result = size == bytes_written;

	file_reference.position += bytes_written;

	return result;
}

bool file_close(s_file_reference& file_reference)
{
	if (CloseHandle(file_reference.handle))
	{
		file_reference.handle = INVALID_HANDLE_VALUE;
		file_reference.position = 0;

		return true;
	}

	return false;
}

bool file_get_size(s_file_reference& file_reference, dword& out_file_size)
{
	//assert(file_reference);
	//assert(out_file_size);

	WIN32_FILE_ATTRIBUTE_DATA file_info{};
	if (GetFileAttributesExA(file_reference.path, GetFileExInfoStandard, &file_info))
	{
		out_file_size = file_info.nFileSizeLow;
		return true;
	}

	return false;
}

dword file_get_eof(s_file_reference& file_reference)
{
	//assert(file_reference);

	return GetFileSize(file_reference.handle, 0);
}

bool file_set_position(s_file_reference& file_reference, dword offset)
{
	if (file_reference.position == offset)
		return true;

	LONG DistanceToMoveHigh = 0;
	file_reference.position = SetFilePointer(file_reference.handle, offset, &DistanceToMoveHigh, FILE_BEGIN);

	return file_reference.position != INVALID_SET_FILE_POINTER;
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

	if (*name)
	{
		dword path_len = strlen(path);
		char* buffer = &path[path_len];
		if (buffer != path && *(buffer - 1) != '\\')
		{
			*buffer++ = '\\';
			*buffer = '\0';
			++path_len;
		}

		csstrnzcpy(buffer, name, name_size - path_len);
	}
}

void file_path_add_extension(char* path, long extension_size, char const* extension)
{
	assert(path && extension);
	assert(extension_size > 0 && extension_size <= 0x100000);

	if (*extension)
	{
		dword path_len = strlen(path);
		char* buffer = &path[path_len];
		if (buffer != path)
		{
			*buffer++ = '.';
			*buffer = '\0';
			++path_len;
		}

		csstrnzcpy(buffer, extension, extension_size - path_len);
	}
}

