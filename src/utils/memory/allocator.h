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

struct Arena {
	template <typename Allocator>
	Arena(Allocator &allocator, size_t size)
	    : size(size), memory(allocator.allocate(size))
	{
	}
	Arena(size_t s, void *mem) : size(s), memory(mem) {}
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

	Arena memory() { return Arena{m_size, m_memory}; }

private:
	size_t m_size;
	std::byte *m_memory;
};

template <typename T, typename Allocator, typename... Args>
T *createObject(Allocator &allocator, Args &&... args)
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
	Deleter(Allocator &allocator) : m_allocator(&allocator) {}
	Deleter(const Deleter &deleter) : m_allocator(deleter.m_allocator) {}
	void operator()(T *object)
	{
		if (object != nullptr) {
			object->~T();
			m_allocator->deallocate(object);
		}
	}

private:
	Allocator *m_allocator;
};

template <typename T, typename Allocator>
using UniquePtr = std::unique_ptr<T, Deleter<T, Allocator>>;

template <typename T, typename Allocator>
auto makeUnique(T *ptr, Allocator &allocator)
{
	return UniquePtr<T, Allocator>(ptr, Deleter<T, Allocator>(allocator));
}

template <typename T, typename Allocator, typename... Args>
auto createUniquePtrObject(Allocator &allocator, Args &&... args)
{
	T *ptr = createObject<T, Allocator>(allocator, std::forward<Args>(args)...);

	return UniquePtr<T, Allocator>(ptr, Deleter<T, Allocator>(allocator));
}

template <typename T, typename Allocator>
class ContainerAllocatorAdapter
{
public:
	using value_type = T;
	ContainerAllocatorAdapter(Allocator &a) : m_allocator(a) {}
	T *allocate(size_t size)
	{
		return reinterpret_cast<T *>(
		    m_allocator.allocate_aligned(sizeof(T) * size, alignof(T)));
	}
	void deallocate(T *p, std::size_t) { return m_allocator.deallocate(p); }

private:
	Allocator &m_allocator;
};

}  // namespace memory
#endif
