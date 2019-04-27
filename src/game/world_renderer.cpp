#include "world_renderer.h"
#include "engine.h"
#include "renderer/renderer.h"

namespace castlekeep
{
namespace game
{
using namespace assets;
WorldRenderer::WorldRenderer(const memory::Arena &arena, World *world)
    : m_allocator(arena), m_world(world)
{
}

WorldRenderer::~WorldRenderer() {}

int WorldRenderer::startUp()
{
	return 0;
}

void WorldRenderer::renderTile(render::DrawCommandBuffer &draw_commands,
                               MapCoord pos, const Camera &c, int layer,
                               const graphics::Rect &rect)
{
	int offset_x = k_tile_width / 2 + c.x();
	int offset_y = k_tile_height / 2 + c.y();
	int shift = (pos.y & 0x1) * k_tile_width / 2;
	draw_commands.commands[draw_commands.length].src_x = rect.x;
	draw_commands.commands[draw_commands.length].src_y = rect.y;
	draw_commands.commands[draw_commands.length].dst_x =
	    pos.x * k_tile_width + shift - offset_x;
	draw_commands.commands[draw_commands.length].dst_y =
	    pos.y * k_tile_height / 2 - (rect.height - k_tile_height) - offset_y;
	draw_commands.commands[draw_commands.length].width = rect.width;
	draw_commands.commands[draw_commands.length].height = rect.height;
	draw_commands.commands[draw_commands.length].depth =
	    pos.y * m_world->m_terrain_size * 32 + layer + pos.x;
	draw_commands.length++;
}

int WorldRenderer::renderWorld(const Camera &camera)
{
	auto marker = m_allocator.markGuarded();
	int terrain_size = m_world->m_terrain_size;
	render::DrawCommandBuffer draw_commands;
	draw_commands.commands =
	    reinterpret_cast<render::DrawCommand *>(m_allocator.allocate_aligned(
	        sizeof(render::DrawCommand) * terrain_size * terrain_size,
	        alignof(render::DrawCommand)));
	graphics::TilesetHandle terrain_tileset_handle =
	    m_world->m_tilesets[mapAssetToIndex(TilesetID::tile_land_macros)];

	graphics::Tileset *terrain_tileset =
	    core::Engine::g_engine->graphicsManager()->tileset(
	        terrain_tileset_handle);

	graphics::TextureAtlas *terrain_atlas =
	    core::Engine::g_engine->graphicsManager()->atlas(
	        terrain_tileset->atlas);

	draw_commands.length = 0;
	draw_commands.texture_id = terrain_atlas->texture;

	graphics::TilesetHandle castle_tileset_handle =
	    m_world->m_tilesets[mapAssetToIndex(TilesetID::tile_castle)];
	graphics::Tileset *castle_tileset =
	    core::Engine::g_engine->graphicsManager()->tileset(
	        castle_tileset_handle);

	graphics::TextureAtlas *castle_atlas =
	    core::Engine::g_engine->graphicsManager()->atlas(castle_tileset->atlas);

	render::DrawCommandBuffer draw_commands2;
	draw_commands2.commands =
	    reinterpret_cast<render::DrawCommand *>(m_allocator.allocate_aligned(
	        sizeof(render::DrawCommand) * terrain_size * terrain_size,
	        alignof(render::DrawCommand)));
	draw_commands2.length = 0;
	draw_commands2.texture_id = castle_atlas->texture;

	int startx = camera.x() / k_tile_width;
	int starty = (camera.y() / k_tile_height) * 2;
	int endx =
	    startx + core::Engine::g_engine->renderer()->width() / k_tile_width + 2;
	int endy =
	    starty +
	    (core::Engine::g_engine->renderer()->height() / k_tile_height) * 2 + 4;

	for (int y = starty; y < endy; y++) {
		for (int x = startx; x < endx; x++) {
			MapCoord pos{static_cast<int16_t>(x), static_cast<int16_t>(y)};
			size_t index = m_world->terrainIndex(pos);
			if (m_world->m_buildings[index] >= 0) {
				uint16_t tile = m_world->m_building_tiles[index];
				renderTile(draw_commands2, pos, camera, 1,
				           castle_atlas->rects[tile]);
			} else {
				uint16_t tile =
				    m_world->m_terrain_macro_parts[index] +
				    terrain_tileset
				        ->objects[m_world->m_terrain_macros[index] + 140]
				        .start_index;
				renderTile(draw_commands, pos, camera, 0,
				           terrain_atlas->rects[tile]);
			}
		}
	}
	core::Engine::g_engine->renderer()->draw(draw_commands);
	core::Engine::g_engine->renderer()->draw(draw_commands2);
	return 0;
}

int WorldRenderer::worldWidth() const
{
	return m_world->numHorizontalTiles() * k_tile_width;
}

int WorldRenderer::worldHeight() const
{
	return m_world->numVerticalTiles() * k_tile_height / 2;
}

int WorldRenderer::renderTerrain() {}
}  // namespace game
}  // namespace castlekeep
