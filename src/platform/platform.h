#ifndef PLATFORM_H
#define PLATFORM_H

#include "memory/allocator.h"
#include "memory/polymorphic_allocator_wrappers.h"
#include "renderer/renderer.h"

namespace platform
{
enum class Platforms { LINUX_SDL };

class PlatformInterface
{
public:
	PlatformInterface(const memory::Arena &arena);
	virtual ~PlatformInterface() = 0;
	virtual int init() = 0;
	virtual int createWindow(int width, int height, const char *name) = 0;
	virtual bool processEvents() = 0;

	virtual memory::UniquePtr<render::Renderer, memory::LinearAllocator>
	createRenderSystem(const memory::Arena &arena, size_t max_textures) = 0;

protected:
	memory::LinearAllocator m_allocator;
};

};  // namespace platform
#endif
