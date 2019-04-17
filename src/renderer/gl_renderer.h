#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <SDL2/SDL.h>
#include <epoxy/gl.h>

#include "container/array.h"
#include "memory/linear_allocator.h"
#include "renderer.h"

namespace castlekeep
{
namespace render
{
class GLRenderSystem : public RenderInterface
{
	using Allocator =
	    memory::ContainerAllocatorAdapter<GLuint, memory::LinearAllocator>;

public:
	GLRenderSystem(const memory::Arena &arena, SDL_Window *window,
	               size_t max_textures);

	~GLRenderSystem() final;

	TextureHandle createTexture(int width, int height, PixelFormat format,
	                            const std::byte *data) final;
	int startUp() final;

	void startFrame() final;
	void draw(const DrawCommandBuffer &buffer) final;
	void endFrame() final;

private:
	SDL_Window *m_window;
	SDL_GLContext m_gl_context;
	memory::LinearAllocator m_allocator;
	size_t m_num_textures;
	container::Array<GLuint, Allocator> m_textures;
};

}  // namespace render
}  // namespace castlekeep
#endif
