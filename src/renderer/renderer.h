#ifndef RENDERER_H
#define RENDERER_H

#include <cstddef>
#include <cstdint>

namespace castlekeep
{
namespace render
{
typedef uint16_t TextureHandle;

constexpr TextureHandle k_texture_invalid = 0;

struct DrawCommand {
	int16_t dst_x;
	int16_t dst_y;
	int16_t src_x;
	int16_t src_y;
	uint16_t width;
	uint16_t height;
	uint32_t depth;
};

struct DrawCommandBuffer {
	DrawCommand *commands;
	int length;
	TextureHandle texture_id;
};

enum class PixelFormat { argb8888, palettized };

class RenderInterface
{
public:
	virtual ~RenderInterface() = 0;

	virtual TextureHandle createTexture(int width, int height,
	                                    PixelFormat format,
	                                    const std::byte *data) = 0;
	virtual int startUp() = 0;
	virtual void startFrame() = 0;
	virtual void draw(const DrawCommandBuffer &buffer) = 0;
	virtual void endFrame() = 0;
};

}  // namespace render
}  // namespace castlekeep
#endif
