#ifndef TILESET_H
#define TILESET_H

#include "container/array.h"
#include "memory/linear_allocator.h"

#include "renderer/renderer.h"
#include "texture_atlas.h"

namespace core
{
struct TileObjectPart {
	uint8_t x;
	uint8_t y;
};

struct TileObject {
	uint16_t id;
	uint16_t num_parts;
	uint16_t start_index;
};

struct Tileset {
	int num_objects;
	int num_tiles;
	TileObject *objects;
	TileObjectPart *parts;
	TextureAtlas atlas;
};

typedef uint16_t TilesetHandle;
enum : TilesetHandle { TILESET_INVALID };

class TilesetManager
{
	using Allocator =
	    memory::ContainerAllocatorAdapter<Tileset, memory::LinearAllocator>;

public:
	TilesetManager(const memory::Arena &arena, size_t max_tilesets);
	~TilesetManager();
	TilesetHandle createTileset(const char *data_file,
	                            render::TextureHandle texture);
	Tileset *tileset(TilesetHandle handle);

private:
	memory::LinearAllocator m_allocator;
	memory::Arena m_loader_arena;
	size_t m_num_tilesets;
	container::Array<Tileset, Allocator> m_tilesets;
};

}  // namespace core

#endif
