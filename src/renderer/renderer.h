#ifndef RENDERER_H
#define RENDERER_H

#include <cstddef>
#include <cstdint>

namespace render
{
typedef uint16_t TextureHandle;

enum : TextureHandle { TEXTURE_INVALID = 0 };

struct DrawCommand {
	uint16_t dst_x;
	uint16_t dst_y;
	uint16_t src_x;
	uint16_t src_y;
	uint16_t width;
	uint16_t height;
	uint16_t depth;
};

struct DrawCommandBuffer {
	DrawCommand *commands;
	int length;
	TextureHandle texture_id;
};

enum class PixelFormat { ARGB8888, PALLETIZED };

class Renderer
{
public:
	virtual ~Renderer() = 0;

	virtual TextureHandle createTexture(int width, int height,
	                                    PixelFormat format,
	                                    const std::byte *data) = 0;
	virtual void startFrame() = 0;
	virtual void draw(const DrawCommandBuffer &buffer) = 0;
	virtual void endFrame() = 0;
};

}  // namespace render
#endif
