#include "sdl_renderer.h"

#include <SDL2/SDL.h>

namespace castlekeep
{
namespace render
{
SDLRenderSystem::SDLRenderSystem(const memory::Arena &arena, SDL_Window *window,
                                 size_t max_textures)
    : m_window(window),
      m_renderer(nullptr),
      m_allocator(arena),
      m_num_textures(0),
      m_textures(max_textures, m_allocator)
{
}

SDLRenderSystem::~SDLRenderSystem()
{
	for (int i = 0; i < m_num_textures; i++) {
		SDL_DestroyTexture(m_textures[i]);
	}

	if (m_renderer) {
		SDL_DestroyRenderer(m_renderer);
	}
}

int SDLRenderSystem::startUp()
{
	m_renderer = SDL_CreateRenderer(
	    m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (m_renderer == nullptr) {
		return -1;
	}

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	return 0;
}

TextureHandle SDLRenderSystem::createTexture(int width, int height,
                                             PixelFormat format,
                                             const std::byte *data)
{
	DEBUG_ASSERT(m_num_textures < m_textures.size(), "Too much textures!");
	SDL_Texture *texture =
	    SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA32,
	                      SDL_TEXTUREACCESS_STATIC, width, height);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	if (texture == nullptr) {
		return 0;
	}
	if (SDL_UpdateTexture(texture, nullptr, data, width * 4) != 0) {
		return 0;
	}

	m_textures[m_num_textures] = texture;
	return static_cast<TextureHandle>(++m_num_textures);
}

void SDLRenderSystem::startFrame()
{
	SDL_RenderClear(m_renderer);
}

void SDLRenderSystem::draw(const DrawCommandBuffer &buffer)
{
	for (int i = 0; i < buffer.length; i++) {
		SDL_Rect src = {buffer.commands[i].src_x, buffer.commands[i].src_y,
		                buffer.commands[i].width, buffer.commands[i].height};
		SDL_Rect dst = {buffer.commands[i].dst_x, buffer.commands[i].dst_y,
		                buffer.commands[i].width, buffer.commands[i].height};
		SDL_RenderCopy(m_renderer, texture(buffer.texture_id), &src, &dst);
	}
}

void SDLRenderSystem::endFrame()
{
	SDL_RenderPresent(m_renderer);
}
SDL_Texture *SDLRenderSystem::texture(TextureHandle id)
{
	return m_textures[id - 1];
}

}  // namespace render
}  // namespace castlekeep
