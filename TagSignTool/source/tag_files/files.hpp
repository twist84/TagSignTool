#pragma once

#define FILE_REFERENCE_SIGNATURE 'filo'
#define NUMBER_OF_FILE_REFERENCE_LOCATIONS 2

enum e_file_reference_flags
{
	_file_reference_flag_is_file_name = 0,
	_file_reference_flag_open_for_write,
};

struct file_reference_info
{
	unsigned long signature;
	unsigned short flags;
	short location;
	char path[256];
};
static_assert(sizeof(file_reference_info) == 0x108);

struct s_file_reference : file_reference_info
{
	void* handle;
	long position;
};
static_assert(sizeof(s_file_reference) == 0x110);

void file_reference_agnostic_create(s_file_reference* file_reference);
void file_reference_create(s_file_reference* file_reference);
void file_reference_set_name(s_file_reference* file_reference, char const* path);
void file_reference_create_from_path(s_file_reference* file_reference, char const* path);

bool file_read_into_buffer(s_file_reference* file_reference, void* buffer, unsigned long size);
bool file_write_from_buffer(s_file_reference* file_reference, unsigned long size, void const* buffer);