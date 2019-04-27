#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <SDL2/SDL.h>
#include <epoxy/gl.h>

#include "container/array.h"
#include "memory/linear_allocator.h"
#include "renderer.h"
#include "system.h"

namespace castlekeep
{
namespace render
{
struct DrawCall {
	TextureHandle texture;
	int start;
	int size;
};

struct Frame {
	DrawCall *draw_calls;
	int draw_call_count;
};

using namespace memory::literals;
class GLRenderSystem : public RenderInterface,
                       public core::EngineSystem<GLRenderSystem>
{
	using Allocator =
	    memory::ContainerAllocatorAdapter<GLuint, memory::LinearAllocator>;

public:
	constexpr static size_t k_minimal_arena_size = 1_mib;
	GLRenderSystem(const memory::Arena &arena, SDL_Window *window,
	               size_t max_textures);

	~GLRenderSystem() final;

	TextureHandle createTexture(int width, int height, PixelFormat format,
	                            const std::byte *data) final;
	int startUp() final;

	void startFrame() final;
	void draw(const DrawCommandBuffer &buffer) final;
	void endFrame() final;

	int width() final;
	int height() final;

private:
	GLuint texture(TextureHandle id);
	constexpr static size_t k_vbo_size = 100_mib;
	SDL_Window *m_window;
	SDL_GLContext m_gl_context;
	memory::LinearAllocator m_allocator;
	size_t m_num_textures;
	container::Array<GLuint, Allocator> m_textures;
	GLuint m_vao;
	Frame m_frame;
	int m_width;
	int m_height;
};

}  // namespace render
}  // namespace castlekeep
#endif
