#include "engine.h"
#include <SDL2/SDL.h>
#include "platform/sdl_platform.h"
#include "utils/memory/allocator.h"
#include "utils/memory/polymorphic_allocator_wrappers.h"

namespace core
{
using namespace memory::literals;
Engine::Engine()
    : m_platform(nullptr),
      m_image_loader(nullptr),
      m_render_interface(nullptr),
      m_systems_allocator(new std::byte[100_mib], 100_mib)
{
}

int Engine::start()
{
	memory::LinearAllocatorWrapper allocator(&m_systems_allocator);
	m_platform = memory::allocateObject<platform_abstraction::SDLPlatform>(
	    m_systems_allocator, &allocator);
	m_image_loader = memory::allocateObject<loaders::ImageLoader>(
	    m_systems_allocator,
	    memory::MemoryArena(1_mib, m_systems_allocator.allocate(1_mib)));
	m_platform->init();
	m_platform->initWindow(1024, 768, "Castlekeep");
	m_render_interface = m_platform->createRenderSystem();
	return 0;
}

int Engine::loop()
{
	while (m_platform->processEvents()) {
	}
}

int Engine::shutdown()
{
	SDL_Quit();
	return 0;
}
};  // namespace core
