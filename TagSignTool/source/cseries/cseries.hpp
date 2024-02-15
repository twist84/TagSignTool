#pragma once

#include "math/integer_math.hpp"
#include "math/real_math.hpp"

//// 32-character ascii string
//using string = char[32];
//static_assert(sizeof(string) == 0x20);
//
//// 256-character ascii string
//using long_string = char[256];
//static_assert(sizeof(long_string) == 0x100);

// a 32-bit string identifier
using string_id = long;
static_assert(sizeof(string_id) == 0x4);

// 8-bit unsigned integer ranging from 0 to 255
using byte = unsigned char;
static_assert(sizeof(byte) == 0x1);

// 16-bit unsigned integer ranging from 0 to 65,535
using word = unsigned short;
static_assert(sizeof(word) == 0x2);

// 32-bit unsigned integer ranging from 0 to 4,294,967,295
using dword = unsigned long;
static_assert(sizeof(dword) == 0x4);

// 64-bit unsigned integer ranging from 0 to 18,446,744,073,709,551,615
using qword = unsigned long long;
static_assert(sizeof(qword) == 0x8);

// 8-bit enumerator value
using char_enum = char;
static_assert(sizeof(char_enum) == 0x1);

// 16-bit enumerator value
using short_enum = short;
static_assert(sizeof(short_enum) == 0x2);

// 32-bit enumerator value
using long_enum = long;
static_assert(sizeof(long_enum) == 0x4);

// 8-bit flags container
using byte_flags = byte;
static_assert(sizeof(byte_flags) == 0x1);

// 16-bit flags container
using word_flags = word;
static_assert(sizeof(word_flags) == 0x2);

// 32-bit flags container
using dword_flags = dword;
static_assert(sizeof(dword_flags) == 0x4);

// 64-bit flags container
using qword_flags = qword;
static_assert(sizeof(qword_flags) == 0x8);

// 32-bit floating-point number ranging from 1.175494351e-38F to 3.402823466e+38F
using real = float;
static_assert(sizeof(real) == 0x4);

// 4-character tag group identifier
typedef dword tag;
static_assert(sizeof(tag) == 0x4);

enum : tag
{
	_tag_none = 0xFFFFFFFF
};

#define _STRCONCAT(x, y) x ## y
#define STRCONCAT(x, y) _STRCONCAT(x, y)

#define OFFSETOF(s,m) __builtin_offsetof(s,m)
#define NUMBEROF(_array) (sizeof(_array) / sizeof(_array[0]))
#define IN_RANGE_INCLUSIVE(value, begin, end) (((value) >= (begin)) && ((value) <= (end)))
#define VALID_INDEX(index, count) ((index) >= 0 && (index) < (count))
#define VALID_COUNT(index, count) ((index) >= 0 && (index) <= (count))

#define SIZEOF_BITS(value) 8 * sizeof(value)

const long CHAR_BITS = SIZEOF_BITS(char);
const long SHORT_BITS = SIZEOF_BITS(short);
const long LONG_BITS = SIZEOF_BITS(long);

#define FLAG(bit) (1 << (bit))
#define MASK(bit) ((1 << (bit)) - 1)
#define TEST_BIT(flags, bit) (((flags) & (1 << (bit))) != 0)
#define ALIGN(value, bit) (((value) & ~((1 << (bit)) - 1)) + (1 << (bit)))

#define NONE -1

long csstricmp(char const* s1, char const* s2);
long csstrcmp(char const* s1, char const* s2);
long csstrnicmp(char const* s1, char const* s2, dword max_count);
char* csstrnzcpy(char* s1, char const* s2, unsigned long s1_size);
char* csstrnzcat(char* s1, char const* s2, dword size);
dword csstrnlen(char const* s, dword size);

struct c_string_id
{
	string_id m_value;
};
static_assert(sizeof(c_string_id) == sizeof(string_id));

//template<typename t_type, typename t_storage_type, t_type k_count>
template<typename t_type, typename t_storage_type, long k_count>
struct c_flags //: public c_flags_no_init<t_type, t_storage_type, k_count>
{
	t_storage_type m_storage;
};

