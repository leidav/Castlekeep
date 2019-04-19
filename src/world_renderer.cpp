#include "world_renderer.h"
#include "engine.h"
#include "renderer/renderer.h"

namespace castlekeep
{
namespace core
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
                               MapCoord pos, int layer,
                               const graphics::Rect &rect)
{
	int offset_x = World::k_tile_width / 2;
	int offset_y = World::k_tile_height / 2;
	int shift = (pos.y & 0x1) * World::k_tile_width / 2;
	draw_commands.commands[draw_commands.length].src_x = rect.x;
	draw_commands.commands[draw_commands.length].src_y = rect.y;
	draw_commands.commands[draw_commands.length].dst_x =
	    pos.x * World::k_tile_width + shift - offset_x;
	draw_commands.commands[draw_commands.length].dst_y =
	    pos.y * World::k_tile_height / 2 -
	    (rect.height - World::k_tile_height) - offset_y;
	draw_commands.commands[draw_commands.length].width = rect.width;
	draw_commands.commands[draw_commands.length].height = rect.height;
	draw_commands.commands[draw_commands.length].depth =
	    pos.y * m_world->m_terrain_size * 32 + layer + pos.x;
	fprintf(stderr, "%d\n", draw_commands.commands[draw_commands.length].depth);
	draw_commands.length++;
}

int WorldRenderer::renderWorld()
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
	    g_engine->graphicsManager()->tileset(terrain_tileset_handle);

	graphics::TextureAtlas *terrain_atlas =
	    g_engine->graphicsManager()->atlas(terrain_tileset->atlas);

	draw_commands.length = 0;
	draw_commands.texture_id = terrain_atlas->texture;

	graphics::TilesetHandle castle_tileset_handle =
	    m_world->m_tilesets[mapAssetToIndex(TilesetID::tile_castle)];
	graphics::Tileset *castle_tileset =
	    g_engine->graphicsManager()->tileset(castle_tileset_handle);

	graphics::TextureAtlas *castle_atlas =
	    g_engine->graphicsManager()->atlas(castle_tileset->atlas);

	render::DrawCommandBuffer draw_commands2;
	draw_commands2.commands =
	    reinterpret_cast<render::DrawCommand *>(m_allocator.allocate_aligned(
	        sizeof(render::DrawCommand) * terrain_size * terrain_size,
	        alignof(render::DrawCommand)));
	draw_commands2.length = 0;
	draw_commands2.texture_id = castle_atlas->texture;

	for (int y = 0; y < terrain_size; y++) {
		for (int x = 0; x < terrain_size; x++) {
			MapCoord pos{static_cast<int16_t>(x), static_cast<int16_t>(y)};
			size_t index = m_world->terrainIndex(pos);
			if (m_world->m_buildings[index] >= 0) {
				uint16_t tile = m_world->m_building_tiles[index];
				renderTile(draw_commands2, pos, 1, castle_atlas->rects[tile]);
			} else {
				uint16_t tile =
				    m_world->m_terrain_macro_parts[index] +
				    terrain_tileset
				        ->objects[m_world->m_terrain_macros[index] + 140]
				        .start_index;
				renderTile(draw_commands, pos, 0, terrain_atlas->rects[tile]);
			}
		}
	}
	g_engine->renderer()->draw(draw_commands);
	g_engine->renderer()->draw(draw_commands2);
	return 0;
}

int WorldRenderer::renderTerrain() {}
}  // namespace core
}  // namespace castlekeep
