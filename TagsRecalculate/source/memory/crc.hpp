#pragma once

unsigned long adler_new();
unsigned long adler32(unsigned long adler, const unsigned char* buf, unsigned long len);