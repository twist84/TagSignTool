#pragma once

bool file_exists(char const* path);
bool file_read_into_buffer(char const* path, void** out_buffer, long* out_buffer_size);
bool file_write_from_buffer(char const* path, void* buffer, long buffer_size);