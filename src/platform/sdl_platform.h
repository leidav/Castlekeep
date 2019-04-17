#ifndef SDL_PLATFORM_H
#define SDL_PLATFORM_H

#include "memory/allocator.h"
#include "platform.h"
#include "system.h"

#include <SDL2/SDL.h>

namespace castlekeep
{
namespace platform
{
using namespace memory::literals;
class SDLPlatform : public PlatformInterface,
                    public core::EngineSystem<SDLPlatform>
{
public:
	constexpr static size_t k_minimal_arena_size = 1_kib;

	SDLPlatform(const memory::Arena &arena);
	~SDLPlatform() final;
	int startUp();
	int createWindow(int width, int height, const char *name) final;
	bool processEvents() final;
	WindowHandle window() final;

private:
	memory::LinearAllocator m_allocator;
	SDL_Window *m_window;
};

};  // namespace platform
}  // namespace castlekeep
#endif
