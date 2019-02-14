#ifndef FREELIST_ALLOCATOR
#define FREELIST_ALLOCATOR

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include "allocator.h"

namespace memory
{
class FreeListAllocator
{
public:
	FreeListAllocator(void *memory, size_t size);
	FreeListAllocator(const MemoryArena &region);
	~FreeListAllocator();

	void *allocate(size_t size);
	void *allocate_aligned(size_t size, size_t alignment);

	void deallocate(void *address);

	void *allocate_aligned_old(size_t size, size_t alignment);
	void deallocate_old(void *address);

private:
	struct ChunkHeader {
		ChunkHeader *next;
		uint32_t size;
		uint16_t is_free;
		uint16_t padding;
	};

	struct ChunkTrailer {
		uint32_t size;
		uint32_t is_free;
	};
	struct FreeChunkTrailer {
		ChunkHeader *prev;
		uint32_t size;
		uint32_t is_free;
	};

	static ChunkHeader *header(std::byte *ptr)
	{
		return reinterpret_cast<ChunkHeader *>(ptr);
	}

	static ChunkTrailer *trailer(std::byte *ptr)
	{
		return reinterpret_cast<ChunkTrailer *>(ptr);
	}
	static FreeChunkTrailer *freeTrailer(void *ptr)
	{
		return reinterpret_cast<FreeChunkTrailer *>(ptr);
	}

	static std::byte *bytePtr(void *ptr)
	{
		return reinterpret_cast<std::byte *>(ptr);
	}

	static constexpr size_t minimal_split_size =
	    std::max(static_cast<size_t>(16), sizeof(ChunkHeader *));
	static constexpr size_t allocation_size_alignment =
	    std::max(static_cast<size_t>(16), alignof(ChunkHeader));

	std::byte *m_memory;
	ChunkHeader *m_free_list;
	size_t m_size;
};

}  // namespace memory
#endif
