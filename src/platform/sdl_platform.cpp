#include "sdl_platform.h"
#include "memory/allocator.h"
#include "renderer/sdl_renderer.h"

#include <SDL2/SDL_render.h>

namespace platform
{
SDLPlatform::SDLPlatform(const memory::Arena &arena)
    : PlatformInterface(arena), m_window(nullptr)
{
}

SDLPlatform::~SDLPlatform()
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

int SDLPlatform::init()
{
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "initialization failed!\n");
		fprintf(stderr, "description: %s", SDL_GetError());
		SDL_Quit();
	}
	return 0;
}

int SDLPlatform::createWindow(int width, int height, const char *name)
{
	m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED,
	                            SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	if (m_window == nullptr) {
		fprintf(stderr, "window creation failed!\n");
		SDL_Quit();
	}
	return 0;
}
bool SDLPlatform::processEvents()
{
	SDL_Event event;
	bool is_running = true;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		    case SDL_QUIT:
			    is_running = false;
			    break;
		    case SDL_MOUSEBUTTONDOWN:
			    break;
		    case SDL_MOUSEBUTTONUP:
			    break;
		    default:
			    break;
		}
	}
	return is_running;
}

memory::UniquePtr<render::Renderer, memory::LinearAllocator>
SDLPlatform::createRenderSystem(const memory::Arena &arena, size_t max_textures)
{
	SDL_Renderer *renderer = SDL_CreateRenderer(
	    m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == nullptr) {
		fprintf(stderr, "renderer creation failed!\n");
		SDL_Quit();
	}

	render::Renderer *ptr = memory::createObject<render::SDLRenderSystem>(
	    m_allocator, arena, renderer, max_textures);
	return memory::makeUnique(ptr, &m_allocator);
}

};  // namespace platform
