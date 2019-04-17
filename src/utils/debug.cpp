#include "debug.h"

#ifdef DEBUG_BUILD

#include <cstdio>
#include <cstdlib>

void callstack();

#ifdef __linux__
#include <cxxabi.h>
#include <execinfo.h>

#define NUM_ADDRESSES 1024
static void* addresses[NUM_ADDRESSES];

const char* demangle(char* symbol, char* malloced_buffer, size_t* length)
{
	int pos = 0;
	char c = 0;
	char* name;
	while (((c = symbol[pos]) != 0) && (c != '(')) {
		pos++;
	}
	if (c == '(') {
		pos++;
	}
	name = &symbol[pos];
	while (((c = symbol[pos]) != 0) && (c != '+')) {
		pos++;
	}
	symbol[pos] = 0;

	const char* demangled =
	    abi::__cxa_demangle(name, malloced_buffer, length, nullptr);
	if (demangled == nullptr) {
		demangled = name;
	}

	return demangled;
}

#endif

[[noreturn]] void debug_error(const char* msg, const char* file, const int line)
{
	std::fprintf(stderr, "Error: \"%s\" in file \"%s\" on line %d", msg, file,
	             line);
#ifdef __linux__
	fprintf(stderr,
	        " during execution of the following functions:\n--------\n");
	char** symbols;
	int num = backtrace(addresses, NUM_ADDRESSES);
	symbols = backtrace_symbols(addresses, num);
	size_t buffer_length = 1024;
	char* buffer = reinterpret_cast<char*>(std::malloc(buffer_length));
	for (int i = 1; i < num; i++) {
		const char* demangled = demangle(symbols[i], buffer, &buffer_length);
		if (demangled != nullptr) {
			std::fprintf(stderr, "%d: %s\n--------\n", i, demangled);
		}
	}
	std::free(buffer);
#elif
	std::fprintf(stderr, "\n");
#endif
	std::abort();
}
#endif
