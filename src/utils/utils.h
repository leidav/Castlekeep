#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace utils
{
template <typename T>
static constexpr bool isPowerOfTwo(T a)
{
	static_assert(std::is_unsigned<T>::value,
	              "isPowerOfTwo requires an unsigned integer parameter");
	return (a & (a - 1)) == 0;
}

template <typename T>
static constexpr T nextPowerOfTwo(T a)
{
	static_assert(std::is_unsigned<T>::value,
	              "nextPowerOfTwo requires an unsigned integer parameter");
	a--;
	a = a | (a >> 1);
	a = a | (a >> 2);
	a = a | (a >> 4);
	if constexpr (sizeof(T) == 1) {
		return a + 1;
	}
	a = a | (a >> 8);
	if constexpr (sizeof(T) == 2) {
		return a + 1;
	}
	a = a | (a >> 16);
	if constexpr (sizeof(T) == 4) {
		return a + 1;
	}
	a = a | (a >> 32);
	return a + 1;
}

static size_t alignmentOffset(void *address, size_t alignment)
{
	std::byte *addr = reinterpret_cast<std::byte *>(address);
	size_t misalignment =
	    (reinterpret_cast<size_t>(addr - 1) & (alignment - 1));
	return alignment - misalignment - 1;
}
static constexpr size_t roundDownToMultipleOfAlignment(size_t size,
                                                       size_t alignment)
{
	return size & ~(alignment - 1);
}

static constexpr size_t roundUpToMultipleOfAlignment(size_t size,
                                                     size_t alignment)
{
	return ((size - 1) & ~(alignment - 1)) + alignment;
}
}  // namespace utils

#endif
