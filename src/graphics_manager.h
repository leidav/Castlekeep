#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "container/array.h"
#include "memory/linear_allocator.h"
#include "renderer/renderer.h"
#include "system.h"

namespace castlekeep
{
namespace graphics
{
typedef uint16_t TilesetHandle;
typedef uint16_t AnimationHandle;
typedef uint16_t TextureAtlasHandle;

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

struct TileObjectPart {
	uint8_t x;
	uint8_t y;
};

struct TileObject {
	uint16_t num_parts;
	uint16_t start_index;
};

struct Tileset {
	int num_objects;
	int num_tiles;
	TileObject *objects;
	TileObjectPart *parts;
	TextureAtlasHandle atlas;
};

struct Pivot {
	uint16_t x;
	uint16_t y;
};

struct Animation {
	int num_frames;
	Pivot *frame_pivots;
	TextureAtlasHandle atlas;
};

constexpr TextureAtlasHandle k_atlas_invalid = 0;
constexpr TilesetHandle k_tileset_invalid = 0;
constexpr AnimationHandle k_animation_invalid = 0;

using namespace memory::literals;
class GraphicsManager : public core::EngineSystem<GraphicsManager>
{
	using AtlasAllocator =
	    memory::ContainerAllocatorAdapter<TextureAtlas,
	                                      memory::LinearAllocator>;
	using TilesetAllocator =
	    memory::ContainerAllocatorAdapter<Tileset, memory::LinearAllocator>;
	using AnimationAllocator =
	    memory::ContainerAllocatorAdapter<Animation, memory::LinearAllocator>;

public:
	constexpr static size_t k_minimal_arena_size = 10_mib;

	GraphicsManager(const memory::Arena &arena, size_t max_tilesets,
	                size_t max_animations, size_t max_generic_image_sets);
	~GraphicsManager();

	int startUp();

	TilesetHandle loadTileset(const char *data_file,
	                          render::TextureHandle texture);
	AnimationHandle loadAnimation(const char *data_file,
	                              render::TextureHandle texture);
	TextureAtlasHandle loadGenericImageSet(const char *data_file,
	                                       render::TextureHandle texture);
	TextureAtlas *atlas(TextureAtlasHandle handle);
	Tileset *tileset(TilesetHandle handle);
	Animation *animation(AnimationHandle handle);

private:
	memory::LinearAllocator m_allocator;
	memory::Arena m_loader_arena;
	size_t m_num_atlases;
	size_t m_num_tilesets;
	size_t m_num_animations;
	container::Array<TextureAtlas, AtlasAllocator> m_atlases;
	container::Array<Tileset, TilesetAllocator> m_tilesets;
	container::Array<Animation, AnimationAllocator> m_animations;
};

}  // namespace graphics
}  // namespace castlekeep

#endif
