#include "polymorphic_allocator_wrappers.h"

namespace memory
{
Allocator::~Allocator() {}
LinearAllocatorWrapper::LinearAllocatorWrapper(
    memory::LinearAllocator *allocator)
    : m_allocator(allocator)
{
}

LinearAllocatorWrapper::~LinearAllocatorWrapper() {}

void *LinearAllocatorWrapper::allocate(size_t size)
{
	return m_allocator->allocate(size);
}

void *LinearAllocatorWrapper::allocate_aligned(size_t size, size_t alignment)
{
	return m_allocator->allocate_aligned(size, alignment);
}

void LinearAllocatorWrapper::deallocate(void *address)
{
	return m_allocator->deallocate(address);
}

};  // namespace memory
