#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "renderer/renderer.h"

namespace core
{
struct Rect {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
};

struct TextureAtlas {
	int num;
	Rect *rects;
	render::TextureHandle texture;
};

}  // namespace core

#endif
