#include "game.h"
#include "engine.h"

namespace castlekeep
{
namespace game
{
Game::Game(const memory::Arena &arena)
    : m_allocator(arena),
      m_world(nullptr, m_allocator),
      m_world_renderer(nullptr, m_allocator),
      m_camera()
{
	m_input_manager = core::Engine::g_engine->inputManager();
}

Game::~Game() {}
void Game::startUp()
{
	m_world = memory::createUniquePtrObject<game::World>(
	    m_allocator,
	    memory::Arena(m_allocator, game::World::k_minimal_arena_size), 400);
	m_world_renderer = memory::createUniquePtrObject<game::WorldRenderer>(
	    m_allocator,
	    memory::Arena(m_allocator, game::WorldRenderer::minimalArenaSize()),
	    m_world.get());
	int width = core::Engine::g_engine->renderer()->width();
	int height = core::Engine::g_engine->renderer()->height();

	m_camera.setMaxPos(m_world_renderer->worldWidth() - width,
	                   m_world_renderer->worldHeight() - height);

	m_world->loadAssets();
	m_world->generateTerrain();
}

void Game::update()
{
	int dx = 0;
	int dy = 0;
	int width = core::Engine::g_engine->renderer()->width();
	int height = core::Engine::g_engine->renderer()->height();

	auto state = m_input_manager->state();

	if (state.keyboard.keys[input::k_key_arrow_left]) {
		dx = -10;
	} else if (state.keyboard.keys[input::k_key_arrow_right]) {
		dx = 10;
	}
	if (state.keyboard.keys[input::k_key_arrow_up]) {
		dy = -10;
	} else if (state.keyboard.keys[input::k_key_arrow_down]) {
		dy = 10;
	}

	if (state.mouse.x == 0) {
		dx = -10;
	} else if (state.mouse.x >= width - 1) {
		dx = 10;
	}
	if (state.mouse.y == 0) {
		dy = -10;
	} else if (state.mouse.y >= height - 1) {
		dy = 10;
	}
	m_camera.move(dx, dy);
}

void Game::render()
{
	m_world_renderer->renderWorld(m_camera);
}

void Game::shutDown() {}

};  // namespace game
};  // namespace castlekeep
