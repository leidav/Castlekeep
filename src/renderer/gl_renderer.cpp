#include "gl_renderer.h"

namespace render
{
GLRenderSystem::GLRenderSystem(const memory::Arena &arena, size_t max_textures)
{
}

GLRenderSystem::~GLRenderSystem() {}

TextureHandle GLRenderSystem::createTexture(int width, int height,
                                            PixelFormat format,
                                            const std::byte *data)
{
	return 0;
}

void GLRenderSystem::startFrame() {}

void GLRenderSystem::draw(const DrawCommandBuffer &buffer) {}

void GLRenderSystem::endFrame() {}

};  // namespace render
