#ifndef SDL_PLATFORM_H
#define SDL_PLATFORM_H

#include <SDL2/SDL.h>

#include "platform.h"
#include "renderer/renderer.h"

namespace platform_abstraction
{
class SDLPlatform : public Platform
{
public:
	SDLPlatform(memory::Allocator *allocator);
	~SDLPlatform() final;
	int init() final;
	int initWindow(int width, int height, const char *name) final;
	bool processEvents() final;
	render::RenderSystem *createRenderSystem() final;

private:
	SDL_Window *m_window;
};

};  // namespace platform_abstraction
#endif
