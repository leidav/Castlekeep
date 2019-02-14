#ifndef POLYMORPHIC_ALLOCATOR_WRAPPERS
#define POLYMORPHIC_ALLOCATOR_WRAPPERS

#include "allocator.h"
#include "block_allocator.h"
#include "linear_allocator.h"

namespace memory
{
class Allocator
{
public:
	virtual ~Allocator() = 0;
	virtual void *allocate(size_t size) = 0;
	virtual void *allocate_aligned(size_t size, size_t alignment) = 0;

	virtual void deallocate(void *address) = 0;
};

class LinearAllocatorWrapper : public Allocator
{
public:
	LinearAllocatorWrapper(LinearAllocator *allocator);
	~LinearAllocatorWrapper() final;
	void *allocate(size_t size) final;
	void *allocate_aligned(size_t size, size_t alignment) final;
	void deallocate(void *address) final;

private:
	LinearAllocator *m_allocator;
};

template <size_t block_size>
class BlockAllocatorWrapper : public Allocator
{
public:
	BlockAllocatorWrapper(BlockAllocator<block_size> *allocator)
	    : m_allocator(allocator)
	{
	}
	~BlockAllocatorWrapper() final {}
	void *allocate(size_t size) final { return m_allocator->allocate(size); }
	void *allocate_aligned(size_t size, size_t alignment) final
	{
		return m_allocator->allocate_aligned(size, alignment);
	}

	void deallocate(void *address) final
	{
		return m_allocator->deallocate(address);
	}

private:
	BlockAllocator<block_size> *m_allocator;
};

}  // namespace memory
#endif
