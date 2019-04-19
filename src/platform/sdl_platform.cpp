#include "sdl_platform.h"
#include "memory/allocator.h"

#include <SDL2/SDL_render.h>

namespace castlekeep
{
namespace platform
{
SDLPlatform::SDLPlatform(const memory::Arena &arena)
    : m_allocator(arena), m_window(nullptr)
{
}

SDLPlatform::~SDLPlatform()
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

int SDLPlatform::startUp()
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
	                    SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	m_window = SDL_CreateWindow(
	    name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
	    SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);

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

WindowHandle SDLPlatform::window()
{
	return {m_window};
}

}  // namespace platform
}  // namespace castlekeep
