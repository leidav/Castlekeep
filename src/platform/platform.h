#ifndef PLATFORM_H
#define PLATFORM_H

#include "input_manager.h"
#include "memory/allocator.h"
#include "memory/polymorphic_allocator_wrappers.h"
#include "renderer/renderer.h"

namespace castlekeep
{
namespace platform
{
enum class Platforms { linux_sdl };

struct WindowHandle {
	void *window;
};

class PlatformInterface
{
public:
	virtual ~PlatformInterface() = 0;
	virtual int createWindow(int width, int height, const char *name) = 0;
	virtual bool processEvents(input::InputState &state) = 0;
	virtual WindowHandle window() = 0;
};

};  // namespace platform
}  // namespace castlekeep
#endif
