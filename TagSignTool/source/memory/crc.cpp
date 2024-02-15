#include "memory/crc.hpp"

dword adler_new()
{
	return adler32(0, 0, 0);
}

dword adler32(dword adler, const byte* buf, dword len)
{
	dword sum2 = (adler >> 16) & 0xFFFF;
	adler &= 0xFFFF;

	if (len == 1ul)
	{
		adler += buf[0];

		if (adler >= 0xFFF1ul)
			adler -= 0xFFF1ul;

		sum2 += adler;

		if (sum2 >= 0xFFF1ul)
			sum2 -= 0xFFF1ul;

		return adler | (sum2 << 16);
	}

	if (!buf)
		return 1ul;

	if (len < 16)
	{
		while (len--)
		{
			adler += *buf++;
			sum2 += adler;
		}

		if (adler >= 0xFFF1ul)
			adler -= 0xFFF1ul;

		sum2 %= 0xFFF1ul;

		return adler | (sum2 << 16);
	}

	while (len >= 0x15B0)
	{
		len -= 0x15B0;
		dword n = 0x15B0 / 16;

		do
		{
			adler += (buf)[0];  sum2 += adler;
			adler += (buf)[1];  sum2 += adler;
			adler += (buf)[2];  sum2 += adler;
			adler += (buf)[3];  sum2 += adler;
			adler += (buf)[4];  sum2 += adler;
			adler += (buf)[5];  sum2 += adler;
			adler += (buf)[6];  sum2 += adler;
			adler += (buf)[7];  sum2 += adler;
			adler += (buf)[8];  sum2 += adler;
			adler += (buf)[9];  sum2 += adler;
			adler += (buf)[10]; sum2 += adler;
			adler += (buf)[11]; sum2 += adler;
			adler += (buf)[12]; sum2 += adler;
			adler += (buf)[13]; sum2 += adler;
			adler += (buf)[14]; sum2 += adler;
			adler += (buf)[15]; sum2 += adler;

			buf += 16;
		} while (--n);

		adler %= 0xFFF1ul;
		sum2 %= 0xFFF1ul;
	}

	if (len)
	{
		while (len >= 16)
		{
			len -= 16;

			adler += (buf)[0];  sum2 += adler;
			adler += (buf)[1];  sum2 += adler;
			adler += (buf)[2];  sum2 += adler;
			adler += (buf)[3];  sum2 += adler;
			adler += (buf)[4];  sum2 += adler;
			adler += (buf)[5];  sum2 += adler;
			adler += (buf)[6];  sum2 += adler;
			adler += (buf)[7];  sum2 += adler;
			adler += (buf)[8];  sum2 += adler;
			adler += (buf)[9];  sum2 += adler;
			adler += (buf)[10]; sum2 += adler;
			adler += (buf)[11]; sum2 += adler;
			adler += (buf)[12]; sum2 += adler;
			adler += (buf)[13]; sum2 += adler;
			adler += (buf)[14]; sum2 += adler;
			adler += (buf)[15]; sum2 += adler;

			buf += 16;
		}

		while (len--)
		{
			adler += *buf++;
			sum2 += adler;
		}

		adler %= 0xFFF1ul;
		sum2 %= 0xFFF1ul;
	}

	return adler | (sum2 << 16);
}

