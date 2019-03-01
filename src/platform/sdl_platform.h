#ifndef SDL_PLATFORM_H
#define SDL_PLATFORM_H

#include "platform.h"

#include <SDL2/SDL.h>

namespace platform
{
class SDLPlatform : public PlatformInterface
{
public:
	SDLPlatform(const memory::Arena &arena);
	~SDLPlatform() final;
	int init() final;
	int createWindow(int width, int height, const char *name) final;
	bool processEvents() final;

	memory::UniquePtr<render::Renderer, memory::LinearAllocator>
	createRenderSystem(const memory::Arena &arena, size_t max_textures) final;

private:
	SDL_Window *m_window;
};

};  // namespace platform
#endif
