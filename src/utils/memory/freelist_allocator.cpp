#include "freelist_allocator.h"
#include "utils.h"

#include <algorithm>

namespace memory
{
FreeListAllocator::FreeListAllocator(void *memory, size_t size)
    : m_memory(reinterpret_cast<std::byte *>(memory)), m_size(size)
{
	m_free_list = reinterpret_cast<ChunkHeader *>(m_memory);

	m_free_list->next = nullptr;
	m_free_list->size = static_cast<uint32_t>(m_size);
	m_free_list->is_free = 1;
}

FreeListAllocator::FreeListAllocator(const MemoryArena &region)
    : FreeListAllocator(region.memory, region.size)
{
}

FreeListAllocator::~FreeListAllocator() {}

void *FreeListAllocator::allocate(size_t size)
{
	return allocate_aligned(size, 8);
}

void *FreeListAllocator::allocate_aligned(size_t size, size_t data_alignment)
{
	std::byte *memory = nullptr;
	ChunkHeader **prev_next = &m_free_list;
	ChunkHeader *prev = nullptr;
	ChunkHeader *chunk = m_free_list;

	size_t allocation_size = utils::roundUpToMultipleOfAlignment(
	    size + sizeof(ChunkHeader) + sizeof(ChunkTrailer) + data_alignment,
	    allocation_size_alignment);

	while (chunk != nullptr) {
		if (allocation_size <= chunk->size) {
			memory = bytePtr(chunk) + sizeof(ChunkHeader);
			size_t offset = utils::alignmentOffset(memory, data_alignment);
			memory += offset;
			*(memory - 1) = static_cast<std::byte>(offset);
			chunk->is_free = 0;
			ChunkHeader *next = nullptr;

			if ((chunk->size - allocation_size) >=
			    (sizeof(ChunkHeader) + sizeof(ChunkTrailer) +
			     minimal_split_size)) {
				next = header(bytePtr(chunk) + allocation_size);
				next->size =
				    chunk->size - static_cast<uint32_t>(allocation_size);
				next->next = chunk->next;
				next->is_free = 1;

				FreeChunkTrailer *next_trailer = freeTrailer(
				    bytePtr(next) + next->size - sizeof(FreeChunkTrailer));
				next_trailer->prev = prev;
				next_trailer->size = next->size;
				next_trailer->is_free = 1;

				chunk->size = static_cast<uint32_t>(allocation_size);
			} else {
				next = chunk->next;
			}

			ChunkTrailer *chunk_trailer =
			    trailer(memory + chunk->size - sizeof(ChunkHeader) -
			            sizeof(ChunkTrailer));
			chunk_trailer->size = chunk->size;
			chunk_trailer->is_free = 0;

			*prev_next = next;
			break;
		}
		prev_next = &chunk->next;
		prev = chunk;
		chunk = chunk->next;
	}

	return memory;
}

void FreeListAllocator::deallocate(void *address)
{
	if (address != nullptr) {
		size_t offset = static_cast<size_t>(*(bytePtr(address) - 1));
		ChunkHeader *chunk_header =
		    header(bytePtr(address) - offset - sizeof(ChunkHeader));
		FreeChunkTrailer *chunk_trailer =
		    freeTrailer(bytePtr(chunk_header) + chunk_header->size -
		                sizeof(FreeChunkTrailer));

		FreeChunkTrailer *left_trailer =
		    freeTrailer(bytePtr(chunk_header) - sizeof(FreeChunkTrailer));
		// merge
		if ((bytePtr(left_trailer) > m_memory) && (left_trailer->is_free)) {
			ChunkHeader *left =
			    header(bytePtr(chunk_header) - left_trailer->size);
			left->size += chunk_header->size;
			if (left_trailer->prev != nullptr) {
				left_trailer->prev->next = left->next;
			} else {
				m_free_list = left->next;
			}
			chunk_header = left;
		}
		ChunkHeader *right = header(bytePtr(chunk_header) + chunk_header->size);
		FreeChunkTrailer *right_trailer =
		    freeTrailer(bytePtr(right) + right->size);
		// merge
		if ((bytePtr(right) < (m_memory + m_size)) && right->is_free) {
			chunk_header->size += right->size;
			if (right_trailer->prev != nullptr) {
				right_trailer->prev->next = right->next;
			} else {
				m_free_list = right->next;
			}
			chunk_trailer = right_trailer;
		}

		chunk_trailer->prev = nullptr;
		chunk_trailer->size = chunk_header->size;
		chunk_trailer->is_free = 1;
		chunk_header->is_free = 1;
		chunk_header->next = m_free_list;
		m_free_list = chunk_header;
	}
}

void *FreeListAllocator::allocate_aligned_old(size_t size,
                                              size_t data_alignment)
{
	std::byte *memory = nullptr;
	ChunkHeader **prev_next = &m_free_list;
	ChunkHeader *chunk = m_free_list;

	// size_t header_alignment = std::alignment_of<ChunkHeader>();
	size_t allocation_size = utils::roundUpToMultipleOfAlignment(
	    size + sizeof(ChunkHeader) + data_alignment,
	    /*header_alignment*/ allocation_size_alignment);

	while (chunk != nullptr) {
		if (allocation_size <= chunk->size) {
			memory = reinterpret_cast<std::byte *>(chunk) + sizeof(ChunkHeader);
			size_t offset = utils::alignmentOffset(memory, data_alignment);
			memory += offset;
			*(memory - 1) = static_cast<std::byte>(offset);

			ChunkHeader *next = nullptr;
			if (chunk->size - allocation_size >=
			    sizeof(ChunkHeader) + minimal_split_size) {
				next = reinterpret_cast<ChunkHeader *>(
				    reinterpret_cast<std::byte *>(chunk) + allocation_size);
				next->size =
				    chunk->size - static_cast<uint32_t>(allocation_size);
				next->next = chunk->next;
				chunk->size = static_cast<uint32_t>(allocation_size);
			} else {
				next = chunk->next;
			}
			*prev_next = next;
			break;
		}
		prev_next = &chunk->next;
		chunk = chunk->next;
	}

	return memory;
}

void FreeListAllocator::deallocate_old(void *address)
{
	if (address != nullptr) {
		std::byte *addr = reinterpret_cast<std::byte *>(address);
		size_t offset = static_cast<size_t>(*(addr - 1));
		ChunkHeader *header = reinterpret_cast<ChunkHeader *>(
		    addr - offset - sizeof(ChunkHeader));

		ChunkHeader *chunk = m_free_list;
		ChunkHeader *prev = nullptr;
		ChunkHeader **prev_next = &m_free_list;

		// find position
		while ((chunk != nullptr) && !((header > prev) && (header < chunk))) {
			prev = chunk;
			prev_next = &chunk->next;
			chunk = chunk->next;
		}
		// insert
		*prev_next = header;
		header->next = chunk;

		// merge with neighbors if possible
		std::byte *left = reinterpret_cast<std::byte *>(prev);
		std::byte *right = reinterpret_cast<std::byte *>(chunk);
		std::byte *middle = reinterpret_cast<std::byte *>(header);
		size_t size = header->size;
		if (left != nullptr) {
			if (left + prev->size == middle) {
				prev->next = chunk;
				prev->size += size;
				header = prev;
			}
		}
		if (right != nullptr) {
			if (middle + size == right) {
				header->next = chunk->next;
				header->size += chunk->size;
			}
		}
	}
}

}  // namespace memory
