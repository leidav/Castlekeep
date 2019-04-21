#include "world.h"
#include "engine.h"
#include "loaders/image_loader.h"

#include <ctime>
#include <random>

namespace castlekeep
{
namespace game
{
template <typename Function>
void setBlock(MapCoord pos, int block_size, Function f)
{
	MapCoord line_start = pos;
	// lower half
	int i = 0;
	for (int row = 0; row < block_size; row++) {
		MapCoord line_pos = line_start;
		for (int column = 0; column < row + 1; column++) {
			if (insideMap(line_pos)) {
				f(line_pos, i);
			}
			line_pos = east(line_pos);
			i++;
		}
		line_start = northWest(line_start);
	}
	// upper half
	line_start = east(line_start);
	for (int row = 1; row < block_size; row++) {
		MapCoord line_pos = line_start;
		for (int column = 0; column < block_size - row; column++) {
			if (insideMap(line_pos)) {
				f(line_pos, i);
			}
			line_pos = east(line_pos);
			i++;
		}
		line_start = northEast(line_start);
	}
}

using namespace assets;
World::World(const memory::Arena& arena, int terrain_size)
    : m_allocator(arena),
      m_terrain_size(terrain_size),
      m_terrain_heights(static_cast<size_t>(m_terrain_size * m_terrain_size),
                        static_cast<uint8_t>(0), m_allocator),
      m_terrain_macro_parts(
          static_cast<size_t>(m_terrain_size * m_terrain_size),
          static_cast<uint16_t>(0), m_allocator),
      m_terrain_type(static_cast<size_t>(m_terrain_size * m_terrain_size),
                     static_cast<TerrainType>(0), m_allocator),
      m_terrain_macros(static_cast<size_t>(m_terrain_size * m_terrain_size),
                       static_cast<uint16_t>(0), m_allocator),
      m_building_tilesets(static_cast<size_t>(m_terrain_size * m_terrain_size),
                          static_cast<uint16_t>(0), m_allocator),
      m_buildings(static_cast<size_t>(m_terrain_size * m_terrain_size),
                  static_cast<int16_t>(-1), m_allocator),
      m_building_tiles(static_cast<size_t>(m_terrain_size * m_terrain_size),
                       static_cast<uint16_t>(0), m_allocator),
      m_tilesets(assets::k_num_tile_assets, m_allocator),
      m_animations(assets::k_num_animation_assets, m_allocator)
{
}
World::~World() {}

void World::placeTerrain(MapCoord pos, int block_size, int macro,
                         TerrainType type)
{
	setBlock(pos, block_size, [&](MapCoord pos, int i) {
		size_t index = terrainIndex(pos);
		m_terrain_macro_parts[index] = i;
		m_terrain_type[index] = type;
		m_terrain_macros[index] = macro;
	});
}

void World::placeBuilding(MapCoord pos, int block_size, int tile_start,
                          uint16_t tileset)
{
	setBlock(pos, block_size, [&](MapCoord pos, int i) {
		size_t index = terrainIndex(pos);
		m_building_tiles[index] = tile_start + i;
		m_buildings[index] = 0;
		m_building_tilesets[index] = tileset;
	});
}

void World::generateTerrain()
{
	std::random_device random_device;
	std::mt19937 random_generator(random_device());
	std::uniform_int_distribution macro_distribution(0, 3);

	for (int y = 0; y < m_terrain_size; y++) {
		for (int x = 0; x < m_terrain_size; x++) {
			m_terrain_macro_parts[y * m_terrain_size + x] = -1;
		}
	}
	int block_size = 4;
	int blocks = m_terrain_size / (block_size * 2) - 1;

	for (int block_y = 0; block_y < blocks; block_y++) {
		for (int block_x = 0; block_x < blocks; block_x++) {
			int macro = macro_distribution(random_generator);
			auto shift = (block_y & 0x1) * block_size / 2;
			MapCoord pos;
			pos.y = block_y * block_size + block_size - 1;
			pos.x = block_x * block_size + shift - 1;
			placeTerrain(pos, block_size, macro, TerrainType::grass);
		}
	}
	graphics::TilesetHandle tileset_handle =
	    m_tilesets[mapAssetToIndex(TilesetID::tile_castle)];
	graphics::Tileset* tileset =
	    core::Engine::g_engine->graphicsManager()->tileset(tileset_handle);
	uint16_t tile_start = tileset->objects[31].start_index;
	uint16_t size = 11;
	MapCoord pos{50, 100};
	placeBuilding(pos, size, tile_start, tileset_handle);
}

int World::loadAssets()
{
	auto marker = m_allocator.markGuarded();
	size_t buffer_size = loader::k_image_default_max_width *
	                     loader::k_image_default_max_height * 4;
	std::byte* image_buffer =
	    reinterpret_cast<std::byte*>(m_allocator.allocate(buffer_size));

	auto image_loader_arena = memory::Arena(m_allocator, 1_mib);

	int width = 0;
	int height = 0;
	int channels = 0;
	for (int i = 0; i < m_tilesets.size(); i++) {
		if (loader::loadImage(image_buffer, buffer_size, width, height,
		                      channels, assets::tile_assets[i].image,
		                      image_loader_arena) != 0) {
			return -1;
		}

		render::TextureHandle texture =
		    core::Engine::g_engine->renderer()->createTexture(
		        width, height, render::PixelFormat::argb8888, image_buffer);
		if (texture == render::k_texture_invalid) {
			return -1;
		}
		graphics::TilesetHandle tileset =
		    core::Engine::g_engine->graphicsManager()->loadTileset(
		        assets::tile_assets[i].data, texture);
		if (tileset == graphics::k_tileset_invalid) {
			return -1;
		}
		m_tilesets[i] = tileset;
	}

	/*for (int i = 0; i < m_animations.size(); i++) {
		if (loader::loadImage(image_buffer, buffer_size, width, height,
							  channels, assets::animation_assets[i].image,
							  image_loader_arena) != 0) {
			return -1;
		}

		render::TextureHandle texture = g_engine->renderer()->createTexture(
			width, height, render::PixelFormat::argb8888, image_buffer);
		if (texture == render::k_texture_invalid) {
			return -1;
		}
		graphics::AnimationHandle animation =
			g_engine->graphicsManager()->loadAnimation(
				assets::animation_assets[i].data, texture);
		if (animation == graphics::k_tileset_invalid) {
			return -1;
		}
		m_animations[i] = animation;
	}
	*/
	return 0;
}
}  // namespace game
}  // namespace castlekeep
