#include "sdl_renderer.h"
namespace render
{
SDLRenderSystem::SDLRenderSystem(SDL_Renderer *renderer) : m_renderer(renderer)
{
}

SDLRenderSystem::~SDLRenderSystem()
{
	SDL_DestroyRenderer(m_renderer);
}

TextureID SDLRenderSystem::createTexture(int width, int height, int channels,
                                         const uint8_t *data)
{
}

void SDLRenderSystem::draw(const DrawCommandBuffer &buffer) {}

};  // namespace render
