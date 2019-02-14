#ifndef ENGINE_H
#define ENGINE_H

#include "memory/allocator.h"
#include "memory/block_allocator.h"
#include "memory/linear_allocator.h"

#include "loaders/image_loader.h"
#include "platform/platform.h"
#include "renderer/renderer.h"

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
	platform_abstraction::Platform *m_platform;
	loaders::ImageLoader *m_image_loader;
	render::RenderSystem *m_render_interface;

	memory::LinearAllocator m_systems_allocator;
};
};  // namespace core
#endif
