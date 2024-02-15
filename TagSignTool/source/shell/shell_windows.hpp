#pragma once

struct s_windows_params
{
	void* instance;
	void* created_window_handle;
	void* window_handle;
	int cmd_show;
	int(__stdcall* window_proc)(void*, unsigned int, unsigned int, int);
	char class_name[64];
	char window_name[64];
	char* cmd_line;
};
static_assert(sizeof(s_windows_params));

s_windows_params* get_windows_params();

bool __cdecl shell_platform_initialize();
void __cdecl shell_platform_verify();

