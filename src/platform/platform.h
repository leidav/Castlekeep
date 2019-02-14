#ifndef PLATFORM_H
#define PLATFORM_H

#include <memory>

#include "memory/allocator.h"
#include "memory/polymorphic_allocator_wrappers.h"
#include "renderer/renderer.h"

namespace platform_abstraction
{
enum class Platforms { LINUX_SDL };

class Platform
{
public:
	Platform(memory::Allocator *allocator);
	virtual ~Platform() = 0;
	virtual int init() = 0;
	virtual int initWindow(int width, int height, const char *name) = 0;
	virtual bool processEvents() = 0;
	virtual render::RenderSystem *createRenderSystem() = 0;

protected:
	memory::Allocator *m_allocator;
};

};  // namespace platform_abstraction
#endif
