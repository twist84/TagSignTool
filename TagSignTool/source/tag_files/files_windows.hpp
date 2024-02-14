#pragma once

enum e_file_open_flags
{
	_file_open_flag_desired_access_read = 0,
	_file_open_flag_desired_access_write,
	_file_open_flag_set_file_end_and_close,
	_file_open_flag_share_mode_read,
	_file_open_flag_print_error,
	_file_open_flag_flags_and_attributes_write,
	_file_open_flag_flags_and_attributes_delete_on_close,
	_file_open_flag_flags_and_attributes_random_access,
	_file_open_flag_flags_and_attributes_sequecial_scan,
};

bool file_create_parent_directories_if_not_present(s_file_reference const& file_reference);
bool file_create(s_file_reference& file_reference);
bool file_exists(s_file_reference& file_reference);
bool file_open(s_file_reference& file_reference, dword open_flags, long* error);
bool file_read(s_file_reference& file_reference, dword size, void* buffer);
bool file_write(s_file_reference& file_reference, dword size, void const* buffer);
bool file_close(s_file_reference& file_reference);
bool file_get_size(s_file_reference& file_reference, dword& out_file_size);
dword file_get_eof(s_file_reference& file_reference);
bool file_set_position(s_file_reference& file_reference, dword offset);

void file_path_remove_name(char* path);
void file_path_add_name(char* path, long name_size, char const* name);
void file_path_add_extension(char* path, long extension_size, char const* extension);

