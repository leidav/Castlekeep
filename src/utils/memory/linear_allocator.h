#ifndef LINEAR_ALLOCATOR
#define LINEAR_ALLOCATOR

#include <cstddef>
#include <cstdint>

#include "allocator.h"
#include "utils.h"

namespace memory
{
class LinearAllocator
{
public:
	using Marker = ptrdiff_t;
	class MarkerGuard
	{
		friend class LinearAllocator;

	public:
		~MarkerGuard() { m_allocator.reset(m_marker); }

		MarkerGuard(const MarkerGuard &other) = delete;
		MarkerGuard(MarkerGuard &&other) = delete;

	private:
		MarkerGuard(LinearAllocator &allocator, Marker marker)
		    : m_allocator(allocator), m_marker(marker)
		{
		}
		LinearAllocator &m_allocator;
		Marker m_marker;
	};

	LinearAllocator(void *memory, size_t size)
	    : m_memory(reinterpret_cast<std::byte *>(memory)),
	      m_free(m_memory),
	      m_end(m_memory + size)
	{
	}
	LinearAllocator(const MemoryArena &region)
	    : LinearAllocator(region.memory, region.size)
	{
	}

	~LinearAllocator() {}

	void *allocate(size_t size) { return allocate_aligned(size, 1); }
	void *allocate_aligned(size_t size, size_t alignment)
	{
		std::byte *address = nullptr;
		size_t offset = utils::alignmentOffset(m_free, alignment);

		auto next_free = m_free + size + offset;
		if (next_free < m_end) {
			address = m_free + offset;
			m_free = next_free;
		}
		return address;
	}

	void deallocate(void *address) { return; }

	void reset(Marker marker = 0) { m_free = m_memory + marker; }

	Marker mark() const { return m_free - m_memory; }
	MarkerGuard markGuarded() { return MarkerGuard(*this, m_free - m_memory); }

private:
	std::byte *m_memory;
	std::byte *m_free;
	std::byte *m_end;
};

}  // namespace memory
#endif
