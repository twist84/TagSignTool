#pragma once

#include "cseries/cseries.hpp"

dword adler_new();
dword adler32(dword adler, const byte* buf, dword len);

