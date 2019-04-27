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
bool SDLPlatform::processEvents(input::InputState &state)
{
	SDL_Event event;
	bool is_running = true;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		    case SDL_QUIT:
			    is_running = false;
			    break;
		    case SDL_MOUSEBUTTONDOWN:
			    if (event.button.button == SDL_BUTTON_LEFT) {
					state.mouse.left_button_down = true;
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					state.mouse.right_button_down = true;
				} else if (event.button.button == SDL_BUTTON_MIDDLE) {
					state.mouse.middle_button_down = true;
				}
			    break;
		    case SDL_MOUSEBUTTONUP:
			    if (event.button.button == SDL_BUTTON_LEFT) {
					state.mouse.left_button_down = false;
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					state.mouse.right_button_down = false;
				} else if (event.button.button == SDL_BUTTON_MIDDLE) {
					state.mouse.middle_button_down = false;
				}
			    break;
		    case SDL_MOUSEWHEEL:
			    break;
		    case SDL_MOUSEMOTION:
			    state.mouse.x = event.motion.x;
				state.mouse.y = event.motion.y;
				state.mouse.dx = event.motion.xrel;
				state.mouse.dy = event.motion.yrel;
			    break;
		    case SDL_KEYDOWN:
			    state.keyboard.key_down = true;
				switch (event.key.keysym.sym) {
				    case SDLK_0:
					    state.keyboard.keys[input::k_key_0] = true;
					    break;
				    case SDLK_1:
					    state.keyboard.keys[input::k_key_1] = true;
					    break;
				    case SDLK_2:
					    state.keyboard.keys[input::k_key_2] = true;
					    break;
				    case SDLK_3:
					    state.keyboard.keys[input::k_key_3] = true;
					    break;
				    case SDLK_4:
					    state.keyboard.keys[input::k_key_4] = true;
					    break;
				    case SDLK_5:
					    state.keyboard.keys[input::k_key_5] = true;
					    break;
				    case SDLK_6:
					    state.keyboard.keys[input::k_key_6] = true;
					    break;
				    case SDLK_7:
					    state.keyboard.keys[input::k_key_7] = true;
					    break;
				    case SDLK_8:
					    state.keyboard.keys[input::k_key_8] = true;
					    break;
				    case SDLK_9:
					    state.keyboard.keys[input::k_key_9] = true;
					    break;
				    case SDLK_q:
					    state.keyboard.keys[input::k_key_q] = true;
					    break;
				    case SDLK_w:
					    state.keyboard.keys[input::k_key_w] = true;
					    break;
				    case SDLK_e:
					    state.keyboard.keys[input::k_key_e] = true;
					    break;
				    case SDLK_r:
					    state.keyboard.keys[input::k_key_r] = true;
					    break;
				    case SDLK_t:
					    state.keyboard.keys[input::k_key_t] = true;
					    break;
				    case SDLK_y:
					    state.keyboard.keys[input::k_key_y] = true;
					    break;
				    case SDLK_u:
					    state.keyboard.keys[input::k_key_u] = true;
					    break;
				    case SDLK_i:
					    state.keyboard.keys[input::k_key_i] = true;
					    break;
				    case SDLK_o:
					    state.keyboard.keys[input::k_key_o] = true;
					    break;
				    case SDLK_p:
					    state.keyboard.keys[input::k_key_p] = true;
					    break;
				    case SDLK_a:
					    state.keyboard.keys[input::k_key_a] = true;
					    break;
				    case SDLK_s:
					    state.keyboard.keys[input::k_key_s] = true;
					    break;
				    case SDLK_d:
					    state.keyboard.keys[input::k_key_d] = true;
					    break;
				    case SDLK_f:
					    state.keyboard.keys[input::k_key_f] = true;
					    break;
				    case SDLK_g:
					    state.keyboard.keys[input::k_key_g] = true;
					    break;
				    case SDLK_h:
					    state.keyboard.keys[input::k_key_h] = true;
					    break;
				    case SDLK_j:
					    state.keyboard.keys[input::k_key_j] = true;
					    break;
				    case SDLK_k:
					    state.keyboard.keys[input::k_key_k] = true;
					    break;
				    case SDLK_l:
					    state.keyboard.keys[input::k_key_l] = true;
					    break;
				    case SDLK_z:
					    state.keyboard.keys[input::k_key_z] = true;
					    break;
				    case SDLK_x:
					    state.keyboard.keys[input::k_key_x] = true;
					    break;
				    case SDLK_c:
					    state.keyboard.keys[input::k_key_c] = true;
					    break;
				    case SDLK_v:
					    state.keyboard.keys[input::k_key_v] = true;
					    break;
				    case SDLK_b:
					    state.keyboard.keys[input::k_key_b] = true;
					    break;
				    case SDLK_n:
					    state.keyboard.keys[input::k_key_n] = true;
					    break;
				    case SDLK_m:
					    state.keyboard.keys[input::k_key_m] = true;
					    break;
				    case SDLK_SPACE:
					    state.keyboard.keys[input::k_key_space] = true;
					    break;
				    case SDLK_RETURN:
					    state.keyboard.keys[input::k_key_return] = true;
					    break;
				    case SDLK_BACKSPACE:
					    state.keyboard.keys[input::k_key_backspace] = true;
					    break;
				    case SDLK_UP:
					    state.keyboard.keys[input::k_key_arrow_up] = true;
					    break;
				    case SDLK_DOWN:
					    state.keyboard.keys[input::k_key_arrow_down] = true;
					    break;
				    case SDLK_LEFT:
					    state.keyboard.keys[input::k_key_arrow_left] = true;
					    break;
				    case SDLK_RIGHT:
					    state.keyboard.keys[input::k_key_arrow_right] = true;
					    break;
				    default:
					    state.keyboard.key_down = false;
					    break;
				}
			    break;
		    case SDL_KEYUP:
			    state.keyboard.key_down = false;
				switch (event.key.keysym.sym) {
				    case SDLK_0:
					    state.keyboard.keys[input::k_key_0] = false;
					    break;
				    case SDLK_1:
					    state.keyboard.keys[input::k_key_1] = false;
					    break;
				    case SDLK_2:
					    state.keyboard.keys[input::k_key_2] = false;
					    break;
				    case SDLK_3:
					    state.keyboard.keys[input::k_key_3] = false;
					    break;
				    case SDLK_4:
					    state.keyboard.keys[input::k_key_4] = false;
					    break;
				    case SDLK_5:
					    state.keyboard.keys[input::k_key_5] = false;
					    break;
				    case SDLK_6:
					    state.keyboard.keys[input::k_key_6] = false;
					    break;
				    case SDLK_7:
					    state.keyboard.keys[input::k_key_7] = false;
					    break;
				    case SDLK_8:
					    state.keyboard.keys[input::k_key_8] = false;
					    break;
				    case SDLK_9:
					    state.keyboard.keys[input::k_key_9] = false;
					    break;
				    case SDLK_q:
					    state.keyboard.keys[input::k_key_q] = false;
					    break;
				    case SDLK_w:
					    state.keyboard.keys[input::k_key_w] = false;
					    break;
				    case SDLK_e:
					    state.keyboard.keys[input::k_key_e] = false;
					    break;
				    case SDLK_r:
					    state.keyboard.keys[input::k_key_r] = false;
					    break;
				    case SDLK_t:
					    state.keyboard.keys[input::k_key_t] = false;
					    break;
				    case SDLK_y:
					    state.keyboard.keys[input::k_key_y] = false;
					    break;
				    case SDLK_u:
					    state.keyboard.keys[input::k_key_u] = false;
					    break;
				    case SDLK_i:
					    state.keyboard.keys[input::k_key_i] = false;
					    break;
				    case SDLK_o:
					    state.keyboard.keys[input::k_key_o] = false;
					    break;
				    case SDLK_p:
					    state.keyboard.keys[input::k_key_p] = false;
					    break;
				    case SDLK_a:
					    state.keyboard.keys[input::k_key_a] = false;
					    break;
				    case SDLK_s:
					    state.keyboard.keys[input::k_key_s] = false;
					    break;
				    case SDLK_d:
					    state.keyboard.keys[input::k_key_d] = false;
					    break;
				    case SDLK_f:
					    state.keyboard.keys[input::k_key_f] = false;
					    break;
				    case SDLK_g:
					    state.keyboard.keys[input::k_key_g] = false;
					    break;
				    case SDLK_h:
					    state.keyboard.keys[input::k_key_h] = false;
					    break;
				    case SDLK_j:
					    state.keyboard.keys[input::k_key_j] = false;
					    break;
				    case SDLK_k:
					    state.keyboard.keys[input::k_key_k] = false;
					    break;
				    case SDLK_l:
					    state.keyboard.keys[input::k_key_l] = false;
					    break;
				    case SDLK_z:
					    state.keyboard.keys[input::k_key_z] = false;
					    break;
				    case SDLK_x:
					    state.keyboard.keys[input::k_key_x] = false;
					    break;
				    case SDLK_c:
					    state.keyboard.keys[input::k_key_c] = false;
					    break;
				    case SDLK_v:
					    state.keyboard.keys[input::k_key_v] = false;
					    break;
				    case SDLK_b:
					    state.keyboard.keys[input::k_key_b] = false;
					    break;
				    case SDLK_n:
					    state.keyboard.keys[input::k_key_n] = false;
					    break;
				    case SDLK_m:
					    state.keyboard.keys[input::k_key_m] = false;
					    break;
				    case SDLK_SPACE:
					    state.keyboard.keys[input::k_key_space] = false;
					    break;
				    case SDLK_RETURN:
					    state.keyboard.keys[input::k_key_return] = false;
					    break;
				    case SDLK_BACKSPACE:
					    state.keyboard.keys[input::k_key_backspace] = false;
					    break;
				    case SDLK_UP:
					    state.keyboard.keys[input::k_key_arrow_up] = false;
					    break;
				    case SDLK_DOWN:
					    state.keyboard.keys[input::k_key_arrow_down] = false;
					    break;
				    case SDLK_LEFT:
					    state.keyboard.keys[input::k_key_arrow_left] = false;
					    break;
				    case SDLK_RIGHT:
					    state.keyboard.keys[input::k_key_arrow_right] = false;
					    break;
				    default:
					    break;
				}
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
