#include "gl_renderer.h"

namespace castlekeep
{
namespace render
{
GLRenderSystem::GLRenderSystem(const memory::Arena &arena, SDL_Window *window,
                               size_t max_textures)
    : m_window(window),
      m_gl_context(nullptr),
      m_allocator(arena),
      m_num_textures(0),
      m_textures(max_textures, m_allocator)
{
}

GLRenderSystem::~GLRenderSystem()
{
	SDL_GL_DeleteContext(m_gl_context);
}

TextureHandle GLRenderSystem::createTexture(int width, int height,
                                            PixelFormat format,
                                            const std::byte *data)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
	             GL_UNSIGNED_BYTE, data);
}

int GLRenderSystem::startUp()
{
	m_gl_context = SDL_GL_CreateContext(m_window);
	if (!m_gl_context) {
		return -1;
	}
}

void GLRenderSystem::startFrame() {}

void GLRenderSystem::draw(const DrawCommandBuffer &buffer) {}

void GLRenderSystem::endFrame()
{
	SDL_GL_SwapWindow(m_window);
}

};  // namespace render
}  // namespace castlekeep
