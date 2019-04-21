#include "engine.h"

#include "assets.h"
#include "loaders/data_loader.h"
#include "platform/sdl_platform.h"
#include "renderer/gl_renderer.h"
#include "utils/memory/allocator.h"

#include <unistd.h>

namespace castlekeep
{
namespace core
{
using namespace memory::literals;

Engine* Engine::g_engine = nullptr;

Engine::Engine(size_t mem_size)
    : m_memory(mem_size),
      m_global_allocator(m_memory.memory()),
      m_systems_allocator(memory::Arena(m_global_allocator, 10_kib)),
      m_platform(nullptr, m_systems_allocator),
      m_renderer(nullptr, m_systems_allocator),
      m_graphics_manager(nullptr, m_systems_allocator),
      m_game(nullptr, m_systems_allocator)
{
}

int Engine::startUp()
{
	auto platform = memory::createObject<platform::SDLPlatform>(
	    m_systems_allocator,
	    memory::Arena(m_global_allocator,
	                  platform::SDLPlatform::minimalArenaSize()));
	platform->startUp();
	m_platform = memory::makeUnique<platform::PlatformInterface>(
	    platform, m_systems_allocator);
	m_platform->createWindow(1920, 1080, "Castlekeep");
	auto renderer = memory::createObject<render::GLRenderSystem>(
	    m_systems_allocator,
	    memory::Arena(m_global_allocator,
	                  render::GLRenderSystem::minimalArenaSize()),
	    reinterpret_cast<SDL_Window*>(m_platform->window().window),
	    assets::k_num_graphics_assets);
	if (renderer->startUp() != 0) {
		fprintf(stderr, "renderer initialization failed!\n");
		exit(1);
	}
	m_renderer = memory::makeUnique<render::RenderInterface>(
	    renderer, m_systems_allocator);

	m_graphics_manager =
	    memory::createUniquePtrObject<graphics::GraphicsManager>(
	        m_systems_allocator,
	        memory::Arena(m_global_allocator,
	                      graphics::GraphicsManager::minimalArenaSize()),
	        assets::k_num_tile_assets, assets::k_num_animation_assets,
	        assets::k_num_generic_image_assets);

	m_graphics_manager->startUp();
	m_game = memory::createUniquePtrObject<game::Game>(
	    m_systems_allocator,
	    memory::Arena(m_global_allocator, game::Game::k_minimal_arena_size));
	m_game->startUp();

	return 0;
}

int Engine::loop()
{
	while (m_platform->processEvents()) {
		m_game->input();
		m_game->update();
		m_renderer->startFrame();
		m_game->render();
		m_renderer->endFrame();
	}
	return 0;
}

int Engine::shutDown()
{
	return 0;
}
}  // namespace core

}  // namespace castlekeep
