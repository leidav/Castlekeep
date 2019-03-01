#ifndef WORLD_H
#define WORLD_H

#include "container/array.h"
#include "memory/linear_allocator.h"

namespace core
{
class World
{
	friend class WorldRenderer;
	template <typename T>

	using Allocator =
	    memory::ContainerAllocatorAdapter<T, memory::LinearAllocator>;

public:
	World(const memory::Arena &arena, int terrain_size);
	~World();
	void generateTerrain();

private:
	memory::LinearAllocator m_allocator;
	int m_terrain_size;
	container::Array<uint8_t, Allocator<uint8_t>> m_heights;
	container::Array<uint16_t, Allocator<uint16_t>> m_tiles;
};
}  // namespace core
#endif
