#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>
#include <cstdint>
#include <memory>

namespace memory
{
namespace literals
{
constexpr size_t operator"" _kib(unsigned long long int size)
{
	return static_cast<size_t>(size) * 1024;
}

constexpr size_t operator"" _mib(unsigned long long int size)
{
	return static_cast<size_t>(size) * 1024 * 1024;
}

constexpr size_t operator"" _gib(unsigned long long int size)
{
	return static_cast<size_t>(size) * 1024 * 1024 * 1024;
}
}  // namespace literals
struct MemoryArena {
	MemoryArena(size_t s, void *mem) : size(s), memory(mem) {}
	size_t size;
	void *memory;
};

class HeapRegion
{
public:
	HeapRegion(size_t size) : m_size(size) { m_memory = new std::byte[size]; }
	HeapRegion(const HeapRegion &other) = delete;
	HeapRegion(HeapRegion &&other) noexcept
	    : m_size(other.m_size), m_memory(other.m_memory)
	{
		other.m_memory = nullptr;
	}
	~HeapRegion() { delete[] m_memory; }
	size_t size() const { return m_size; }

	void *start() { return m_memory; }
	void *end() { return m_memory + m_size; }

	MemoryArena memory() { return {m_size, m_memory}; }

private:
	size_t m_size;
	std::byte *m_memory;
};

template <typename T, typename Allocator, typename... Args>
T *allocateObject(Allocator &allocator, Args &&... args)
{
	return new (allocator.allocate_aligned(sizeof(T), alignof(T)))
	    T(std::forward<Args>(args)...);
}

template <typename T, typename Allocator>
void deleteObject(T *object, Allocator &allocator)
{
	object->~T();
	allocator.deallocate(object);
}

template <typename T, typename Allocator>
class Deleter
{
public:
	Deleter(Allocator &allocator) : m_allocator(allocator) {}
	void operator()(T *object)
	{
		object->~T();
		m_allocator.deallocate(object);
	}

private:
	Allocator &m_allocator;
};

template <typename T, typename Allocator, typename... Args>
auto allocateUniquePtrObject(Allocator &allocator, Args &&... args)
{
	T *ptr =
	    allocateObject<T, Allocator>(allocator, std::forward<Args>(args)...);

	return std::unique_ptr<T, Deleter<T, Allocator>>(
	    ptr, Deleter<T, Allocator>(allocator));
}

}  // namespace memory
#endif
