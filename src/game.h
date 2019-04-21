#ifndef GAME_H
#define GAME_H

#include "memory/linear_allocator.h"
#include "world.h"
#include "world_renderer.h"

namespace castlekeep
{
namespace game
{
class Game
{
public:
	constexpr static size_t k_minimal_arena_size =
	    10_mib + game::World::k_minimal_arena_size +
	    game::WorldRenderer::k_minimal_arena_size;
	Game(const memory::Arena &arena);
	~Game();
	void startUp();
	void input();
	void update();
	void render();
	void shutDown();

private:
	memory::LinearAllocator m_allocator;
	memory::UniquePtr<World, memory::LinearAllocator> m_world;
	memory::UniquePtr<WorldRenderer, memory::LinearAllocator> m_world_renderer;
	Camera m_camera;
};
}  // namespace game
};  // namespace castlekeep

#endif
