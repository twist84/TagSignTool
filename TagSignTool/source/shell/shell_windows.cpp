#include "shell/shell_windows.hpp"

#include "cache/physical_memory_map.hpp"
#include "cseries/cseries_strings.hpp"
#include "main/main.hpp"
#include "profiler/profiler.hpp"
#include "shell/shell.hpp"
#include "tag_files/files.hpp"
#include "tag_files/files_windows.hpp"

#include <stdio.h>
#include <windows.h>

FILE* console_file = NULL;

s_windows_params g_windows_params = {};
s_windows_params* get_windows_params()
{
	return &g_windows_params;
}

bool __cdecl shell_platform_initialize()
{
	if (AllocConsole() != TRUE)
		return false;

	if (SetConsoleTitleA(g_windows_params.window_name) != TRUE)
		return false;

	freopen_s(&console_file, "CONIN$", "r", stdin);
	freopen_s(&console_file, "CONOUT$", "w", stderr);
	freopen_s(&console_file, "CONOUT$", "w", stdout);

	return true;
}

void __cdecl shell_platform_dispose()
{
	if (GetConsoleWindow() != NULL)
		FreeConsole();
}

void __cdecl shell_platform_verify()
{
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	bool wait_for_exit = GetAsyncKeyState(VK_SHIFT) & 0x8000; // if shift is held wait for input on process exit

	SetLastError(NO_ERROR);
	SetProcessDPIAware();

	g_windows_params.cmd_line = lpCmdLine;
	g_windows_params.instance = hInstance;
	g_windows_params.cmd_show = nCmdShow;
	//g_windows_params.window_proc = WndProc;
	g_windows_params.window_handle = NULL;

	char const* name = "HaloOnline TagSignTool";
	csstrnzcpy(g_windows_params.class_name, name, sizeof(g_windows_params.class_name));
	csstrnzcpy(g_windows_params.window_name, name, sizeof(g_windows_params.window_name));

	physical_memory_initialize();

	static bool shell_initialized = false;
	if (shell_initialize(false))
	{
		PROFILE_FUNCTION(main_loop);

		shell_initialized = true;
		shell_dispose();
	}

	if (!shell_initialized)
		shell_dispose();

	physical_memory_free();

	if (wait_for_exit)
		(void)getc(stdin);

	return 0;
}

