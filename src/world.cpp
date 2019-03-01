#include "world.h"

namespace core
{
World::World(const memory::Arena &arena, int terrain_size)
    : m_allocator(arena),
      m_terrain_size(terrain_size),
      m_heights(static_cast<size_t>(m_terrain_size * m_terrain_size),
                m_allocator),
      m_tiles(static_cast<size_t>(m_terrain_size * m_terrain_size), m_allocator)
{
}
World::~World() {}
}  // namespace core
