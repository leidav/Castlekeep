#include "sdl_platform.h"
#include "memory/allocator.h"
#include "renderer/sdl_renderer.h"

#include <SDL2/SDL_render.h>

namespace platform_abstraction
{
SDLPlatform::SDLPlatform(memory::Allocator *allocator)
    : Platform(allocator), m_window(nullptr)
{
}

SDLPlatform::~SDLPlatform()
{
	SDL_DestroyWindow(m_window);
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

int SDLPlatform::initWindow(int width, int height, const char *name)
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

render::RenderSystem *SDLPlatform::createRenderSystem()
{
	SDL_Renderer *renderer = SDL_CreateRenderer(
	    m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == nullptr) {
		fprintf(stderr, "renderer creation failed!\n");
		SDL_Quit();
	}
	return memory::allocateObject<render::SDLRenderSystem>((*m_allocator),
	                                                       renderer);
}

};  // namespace platform_abstraction
