#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>

namespace render
{
typedef uint16_t TextureID;

struct DrawCommand {
	uint16_t dst_x;
	uint16_t dst_y;
	uint16_t src_x;
	uint16_t src_y;
	uint16_t width;
	uint16_t height;
};

struct DrawCommandBuffer {
	int length;
	int texture_id;
	DrawCommand *commands;
};

class RenderSystem
{
public:
	virtual ~RenderSystem() = 0;

	virtual TextureID createTexture(int width, int height, int channels,
	                                const uint8_t *data) = 0;
	virtual void draw(const DrawCommandBuffer &buffer) = 0;
};

}  // namespace render
#endif
