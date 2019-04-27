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
	Camera() : m_x(0), m_y(0), m_maxx(0), m_maxy(0) {}
	Camera(int maxx, int maxy) : m_x(0), m_y(0), m_maxx(maxx), m_maxy(maxy) {}
	Camera(int x, int y, int maxx, int maxy)
	    : m_x(x), m_y(y), m_maxx(maxx), m_maxy(maxy)
	{
	}
	void setMaxPos(int maxx, int maxy)
	{
		m_maxx = maxx;
		m_maxy = maxy;
	}

	void move(int dx, int dy)
	{
		int x = m_x + dx;
		int y = m_y + dy;

		if (x < 0) {
			m_x = 0;
		} else if (x > m_maxx) {
			m_x = m_maxx;
		} else {
			m_x = x;
		}
		if (y < 0) {
			m_y = 0;
		} else if (y > m_maxy) {
			m_y = m_maxy;
		} else {
			m_y = y;
		}
	}
	int x() const { return m_x; }
	int y() const { return m_y; }

private:
	int m_x;
	int m_y;
	int m_maxx;
	int m_maxy;
};

using namespace memory::literals;
class WorldRenderer : public core::EngineSystem<WorldRenderer>
{
public:
	constexpr static size_t k_minimal_arena_size = 10_mib;

	constexpr static int k_tile_width = 32;
	constexpr static int k_tile_height = 16;

	WorldRenderer(const memory::Arena &arena, World *world);
	~WorldRenderer();
	int startUp();
	int renderWorld(const Camera &camera);
	int worldWidth() const;
	int worldHeight() const;

private:
	int renderTerrain();
	void renderTile(render::DrawCommandBuffer &draw_commads, MapCoord pos,
	                const Camera &c, int layer, const graphics::Rect &rect);
	memory::LinearAllocator m_allocator;
	World *m_world;
};
}  // namespace game
}  // namespace castlekeep
#endif
