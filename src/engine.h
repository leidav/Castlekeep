#ifndef ENGINE_H
#define ENGINE_H

#include "memory/allocator.h"
#include "memory/linear_allocator.h"

#include "loaders/image_loader.h"
#include "platform/platform.h"
#include "renderer/renderer.h"
#include "tileset.h"

namespace core
{
class Engine
{
public:
	Engine();
	int start();
	int loop();
	int shutdown();

private:
	memory::HeapRegion m_memory;
	memory::LinearAllocator m_allocator;

	platform::PlatformInterface *m_platform;
	memory::UniquePtr<render::Renderer, memory::LinearAllocator>
	    m_render_system;
	TilesetManager m_tileset_system;
	TilesetHandle m_tileset;
};
};  // namespace core
#endif
