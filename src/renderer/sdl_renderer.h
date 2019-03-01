#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include "container/array.h"
#include "memory/linear_allocator.h"
#include "renderer.h"

#include <SDL2/SDL_render.h>

namespace render
{
class SDLRenderSystem : public Renderer
{
	using Allocator =
	    memory::ContainerAllocatorAdapter<SDL_Texture *,
	                                      memory::LinearAllocator>;

public:
	SDLRenderSystem(const memory::Arena &arena, SDL_Renderer *renderer,
	                size_t max_textures);

	~SDLRenderSystem() final;

	TextureHandle createTexture(int width, int height, PixelFormat format,
	                            const std::byte *data) final;

	void startFrame() final;
	void draw(const DrawCommandBuffer &buffer) final;
	void endFrame() final;

private:
	SDL_Texture *texture(TextureHandle id);
	SDL_Renderer *m_renderer;
	memory::LinearAllocator m_allocator;
	size_t m_num_textures;
	container::Array<SDL_Texture *, Allocator> m_textures;
};

}  // namespace render
#endif
