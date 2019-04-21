#ifndef WORLD_H
#define WORLD_H

#include "assets.h"
#include "container/array.h"
#include "graphics_manager.h"
#include "memory/linear_allocator.h"
#include "system.h"

namespace castlekeep
{
namespace game
{
class Engine;

enum class TerrainType : uint8_t {
	grass = 0,
	dirt = 1,
};

enum Type : uint8_t {
	k_terrain_type_grass = 0,
	k_terrain_type_dirt = 1,
};

enum MacroSize : uint8_t {
	k_macro_size_1x1 = 0,
	k_macro_size_2x2 = 1,
	k_macro_size_3x3 = 2,
	k_macro_size_4x4 = 3,
};

struct TerrainMacroCell {
	Type type;
	MacroSize macro_size;
	uint8_t part;
	uint8_t macro;
};

struct MapCoord {
	int16_t x;
	int16_t y;
};

constexpr MapCoord north(MapCoord pos)
{
	pos.y -= 2;
	return pos;
}
constexpr MapCoord south(MapCoord pos)
{
	pos.y += 2;
	return pos;
}
constexpr MapCoord east(MapCoord pos)
{
	pos.x++;
	return pos;
}
constexpr MapCoord west(MapCoord pos)
{
	pos.x--;
	return pos;
}

constexpr MapCoord northEast(MapCoord pos)
{
	int even = pos.y & 1;
	pos.y--;
	pos.x += even;
	return pos;
}
constexpr MapCoord northWest(MapCoord pos)
{
	int even = 1 - (pos.y & 1);
	pos.y--;
	pos.x -= even;
	return pos;
}
constexpr MapCoord southEast(MapCoord pos)
{
	int even = (pos.y & 1);
	pos.y++;
	pos.x += even;
	return pos;
}

constexpr MapCoord southWest(MapCoord pos)
{
	int even = 1 - (pos.y & 1);
	pos.y++;
	pos.x -= even;
	return pos;
}

constexpr bool insideMap(MapCoord pos)
{
	return (pos.x >= 0) && (pos.y >= 0);
}

using namespace memory::literals;
class World
{
	friend class WorldRenderer;

	template <typename T>
	using Allocator =
	    memory::ContainerAllocatorAdapter<T, memory::LinearAllocator>;

public:
	constexpr static int k_tile_width = 32;
	constexpr static int k_tile_height = 16;

	constexpr static size_t k_minimal_arena_size = 100_mib;
	World(const memory::Arena& arena, int terrain_size);
	~World();
	int loadAssets();
	void generateTerrain();

private:
	size_t terrainIndex(MapCoord coord) const
	{
		return coord.y * m_terrain_size + coord.x;
	}

	void placeTerrain(MapCoord pos, int block_size, int macro,
	                  TerrainType type);
	void placeBuilding(MapCoord pos, int block_size, int tile_start,
	                   uint16_t tileset);
	void place(MapCoord pos, int size, uint16_t tiles_start, uint16_t tileset);

	memory::LinearAllocator m_allocator;
	int m_terrain_size;
	container::Array<uint8_t, Allocator<uint8_t>> m_terrain_heights;
	container::Array<uint16_t, Allocator<uint16_t>> m_terrain_macro_parts;
	container::Array<TerrainType, Allocator<TerrainType>> m_terrain_type;
	container::Array<uint16_t, Allocator<uint16_t>> m_terrain_macros;
	container::Array<uint16_t, Allocator<uint16_t>> m_building_tilesets;
	container::Array<int16_t, Allocator<int16_t>> m_buildings;
	container::Array<uint16_t, Allocator<uint16_t>> m_building_tiles;
	container::Array<graphics::TilesetHandle,
	                 Allocator<graphics::TilesetHandle>>
	    m_tilesets;
	container::Array<graphics::AnimationHandle,
	                 Allocator<graphics::AnimationHandle>>
	    m_animations;
};
}  // namespace game
}  // namespace castlekeep
#endif
