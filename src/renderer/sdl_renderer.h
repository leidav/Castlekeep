#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include "container/array.h"
#include "memory/linear_allocator.h"
#include "renderer.h"
#include "system.h"

#include <SDL2/SDL_render.h>

namespace castlekeep
{
namespace render
{
using namespace memory::literals;

class SDLRenderSystem : public RenderInterface,
                        public core::EngineSystem<SDLRenderSystem>

{
	using Allocator =
	    memory::ContainerAllocatorAdapter<SDL_Texture *,
	                                      memory::LinearAllocator>;

public:
	constexpr static size_t k_minimal_arena_size = 1_mib;
	SDLRenderSystem(const memory::Arena &arena, SDL_Window *window,
	                size_t max_textures);

	~SDLRenderSystem() final;

	int startUp() final;

	TextureHandle createTexture(int width, int height, PixelFormat format,
	                            const std::byte *data) final;

	void startFrame() final;
	void draw(const DrawCommandBuffer &buffer) final;
	void endFrame() final;

private:
	SDL_Texture *texture(TextureHandle id);
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	memory::LinearAllocator m_allocator;
	size_t m_num_textures;
	container::Array<SDL_Texture *, Allocator> m_textures;
};

}  // namespace render
}  // namespace castlekeep
#endif
