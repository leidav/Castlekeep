#ifndef ENGINE_H
#define ENGINE_H

#include "memory/allocator.h"
#include "memory/linear_allocator.h"

#include "game.h"
#include "graphics_manager.h"
#include "loaders/image_loader.h"
#include "platform/platform.h"
#include "renderer/renderer.h"

namespace castlekeep
{
namespace core
{
class Engine
{
public:
	static Engine *g_engine;
	static std::unique_ptr<Engine> create(size_t memory_size)
	{
		if (instanciated == true) {
			return nullptr;
		}
		g_engine = new Engine(memory_size);
		return std::unique_ptr<Engine>(g_engine);
	}

	int startUp();
	int loop();
	int shutDown();

	platform::PlatformInterface *platform() { return m_platform.get(); }
	render::RenderInterface *renderer() { return m_renderer.get(); }
	graphics::GraphicsManager *graphicsManager()
	{
		return m_graphics_manager.get();
	}

private:
	Engine(size_t memory_size);
	memory::HeapRegion m_memory;
	memory::LinearAllocator m_global_allocator;
	memory::LinearAllocator m_systems_allocator;

	memory::UniquePtr<platform::PlatformInterface, memory::LinearAllocator>
	    m_platform;
	memory::UniquePtr<render::RenderInterface, memory::LinearAllocator>
	    m_renderer;
	memory::UniquePtr<graphics::GraphicsManager, memory::LinearAllocator>
	    m_graphics_manager;
	memory::UniquePtr<game::Game, memory::LinearAllocator> m_game;

	static inline bool instanciated = false;
};

}  // namespace core
}  // namespace castlekeep
#endif
