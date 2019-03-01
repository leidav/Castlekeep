#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG_BUILD

#define DEBUG_ERROR(message) debug_error((message), __FILE__, __LINE__)
#define DEBUG_ASSERT(expr, message) \
	debug_assert((expr), (message), __FILE__, __LINE__)

[[noreturn]] void debug_error(const char* msg, const char* file,
                              const int line);

inline void debug_assert(bool expr, const char* message, const char* file,
                         const int line)
{
	if (!expr) {
		debug_error(message, file, line);
	}
}

#else

#define DEBUG_ERROR(a) ((void)0)
#define DEBUG_ASSERT(expr, message) ((void)0)

#endif

#endif
