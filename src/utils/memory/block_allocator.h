#ifndef BLOCK_ALLOCATOR
#define BLOCK_ALLOCATOR

#include "allocator.h"
#include "debug.h"
#include "utils.h"

#include <cstddef>
#include <cstdint>

namespace memory
{
template <size_t block_size>
class BlockAllocator
{
public:
	BlockAllocator(void *memory, size_t size, size_t alignment = 16)
	    : m_num_blocks(utils::roundDownToMultipleOfAlignment(size, alignment) /
	                   block_size),
	      m_allocated(0),
	      m_memory(reinterpret_cast<std::byte *>(memory) +
	               utils::alignmentOffset(memory, alignment))
	{
		static_assert(block_size > sizeof(BlockHeader *),
		              "The size of a block has to be at least as larg as the "
		              "size of a pointer");
		m_free = reinterpret_cast<BlockHeader *>(memory);
		m_free->next = nullptr;
	}
	BlockAllocator(const Arena &region, size_t alignment = 16)
	    : BlockAllocator(region.memory, region.size, alignment)
	{
	}

	~BlockAllocator() {}

	void *allocate()
	{
		void *block = nullptr;
		if (m_allocated <= m_num_blocks) {
			block = m_free;
			if (m_free->next == nullptr) {
				m_free = m_free + block_size;
				m_free->next = nullptr;
			} else {
				m_free = m_free->next;
			}
			m_allocated++;
		}
		DEBUG_ASSERT(block != nullptr, "Not enough memory!");
		return block;
	}
	void *allocate(size_t size) { return allocate(); }
	void *allocate_aligned(size_t size, size_t alignment) { return allocate(); }

	void deallocate(void *address)
	{
		if (address != nullptr) {
			BlockHeader *tmp = m_free;
			m_free = reinterpret_cast<BlockHeader *>(address);
			m_free->next = tmp;
			m_allocated--;
		}
	}

private:
	struct BlockHeader {
		BlockHeader *next;
	};

	size_t m_num_blocks;
	size_t m_allocated;
	std::byte *m_memory;
	BlockHeader *m_free;
};

}  // namespace memory
#endif
