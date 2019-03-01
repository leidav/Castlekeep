#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "memory/allocator.h"
#include "renderer.h"

namespace render
{
class GLRenderSystem : public Renderer
{
public:
	GLRenderSystem(const memory::Arena &arena, size_t max_textures);

	~GLRenderSystem() final;

	TextureHandle createTexture(int width, int height, PixelFormat format,
	                            const std::byte *data) final;

	void startFrame() final;
	void draw(const DrawCommandBuffer &buffer) final;
	void endFrame() final;
};

}  // namespace render
#endif
