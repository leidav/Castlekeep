#include "debug.h"

#include <cstdio>
#include <cstdlib>

#ifdef DEBUG_BUILD
[[noreturn]] void debug_error(const char* msg, const char* file, const int line)
{
	std::fprintf(stderr, "Error: \"%s\" in file \"%s\" on line %d\n", msg, file,
	             line);
	std::abort();
}
#endif
