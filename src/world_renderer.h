#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include "graphics_manager.h"
#include "memory/allocator.h"
#include "renderer/renderer.h"
#include "system.h"
#include "world.h"

namespace castlekeep
{
namespace game
{
class Camera
{
public:
	Camera() : x(0), y(0) {}

private:
	MapCoord startCoord();
	MapCoord endCoord();
	int x;
	int y;
};

using namespace memory::literals;
class WorldRenderer : public core::EngineSystem<WorldRenderer>
{
public:
	constexpr static size_t k_minimal_arena_size = 10_mib;
	WorldRenderer(const memory::Arena &arena, World *world);
	~WorldRenderer();
	int startUp();
	int renderWorld(const Camera &camera);

private:
	int renderTerrain();
	void renderTile(render::DrawCommandBuffer &draw_commads, MapCoord pos,
	                int layer, const graphics::Rect &rect);
	memory::LinearAllocator m_allocator;
	World *m_world;
};
}  // namespace game
}  // namespace castlekeep
#endif
