#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include <SDL2/SDL_render.h>
#include "renderer.h"

namespace render
{
class SDLRenderSystem : public RenderSystem
{
public:
	SDLRenderSystem(SDL_Renderer *renderer);

	~SDLRenderSystem();

	TextureID createTexture(int width, int height, int channels,
	                        const uint8_t *data) final;
	void draw(const DrawCommandBuffer &buffer) final;

private:
	SDL_Renderer *m_renderer;
};

}  // namespace render
#endif
