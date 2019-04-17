#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include "graphics_manager.h"
#include "memory/allocator.h"
#include "renderer/renderer.h"
#include "system.h"
#include "world.h"

namespace castlekeep
{
namespace core
{
using namespace memory::literals;
class WorldRenderer : public EngineSystem<WorldRenderer>
{
public:
	constexpr static size_t k_minimal_arena_size = 10_mib;
	WorldRenderer(const memory::Arena &arena, World *world);
	~WorldRenderer();
	int startUp();
	int renderWorld();

private:
	int renderTerrain();
	void renderTile(render::DrawCommandBuffer &draw_commads, MapCoord pos,
	                const graphics::Rect &rect);
	memory::LinearAllocator m_allocator;
	World *m_world;
};
}  // namespace core
}  // namespace castlekeep
#endif
