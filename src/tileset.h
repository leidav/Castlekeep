#ifndef TILESET_H
#define TILESET_H

#include <memory>
#include <vector>

#include "renderer/renderer.h"

struct Rect {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
};

struct TileObjectPart {
	uint8_t x;
	uint8_t y;
	uint16_t object_id;
	// uint16_t tile;
};

struct Tileset {
	std::vector<Rect> rects;
	std::vector<render::TextureID> texture_handles;
	// std::vector<TileObjectPart> m_tile_object_parts;
};

#endif
