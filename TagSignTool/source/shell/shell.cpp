#include "shell/shell.hpp"

#include "shell/shell_windows.hpp"

bool __cdecl shell_initialize(bool windowed)
{
	bool shell_initialized = false;
	if (shell_platform_initialize())
	{
		shell_initialized = true;
		shell_platform_verify();
	}
	return shell_initialized;
}

void __cdecl shell_dispose()
{
}

