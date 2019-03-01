#include "polymorphic_allocator_wrappers.h"

namespace memory
{
AbstractAllocator::~AbstractAllocator() {}
LinearAllocatorAdapter::LinearAllocatorAdapter(LinearAllocator &allocator)
    : m_allocator(allocator)
{
}

LinearAllocatorAdapter::~LinearAllocatorAdapter() {}

void *LinearAllocatorAdapter::allocate(size_t size)
{
	return m_allocator.allocate(size);
}

void *LinearAllocatorAdapter::allocate_aligned(size_t size, size_t alignment)
{
	return m_allocator.allocate_aligned(size, alignment);
}

void LinearAllocatorAdapter::deallocate(void *address)
{
	return m_allocator.deallocate(address);
}

};  // namespace memory
