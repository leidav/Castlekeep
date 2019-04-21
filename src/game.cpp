#include "game.h"
#include "engine.h"

namespace castlekeep
{
namespace game
{
Game::Game(const memory::Arena &arena)
    : m_allocator(arena),
      m_world(nullptr, m_allocator),
      m_world_renderer(nullptr, m_allocator)
{
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

	m_world->loadAssets();
	m_world->generateTerrain();
}

void Game::input() {}

void Game::update() {}

void Game::render()
{
	m_world_renderer->renderWorld(m_camera);
}

void Game::shutDown() {}

};  // namespace game
};  // namespace castlekeep
