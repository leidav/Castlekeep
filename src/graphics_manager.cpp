#include "graphics_manager.h"
#include "debug.h"
#include "loaders/data_loader.h"
#include "loaders/file.h"

#include <cstring>

namespace castlekeep
{
namespace graphics
{
using namespace memory::literals;

GraphicsManager::GraphicsManager(const memory::Arena &arena,
                                 size_t max_tilesets, size_t max_animations,
                                 size_t max_generic_image_sets)
    : m_allocator(arena),
      m_loader_arena(10_kib, m_allocator.allocate(10_kib)),
      m_num_atlases(0),
      m_num_tilesets(0),
      m_num_animations(0),
      m_atlases(max_tilesets + max_animations + max_generic_image_sets,
                m_allocator),
      m_tilesets(max_tilesets, m_allocator),
      m_animations(max_animations, m_allocator)
{
	DEBUG_ASSERT(arena.size >= k_minimal_arena_size, "Arena too small!");
}

GraphicsManager::~GraphicsManager() {}

int GraphicsManager::startUp()
{
	return 0;
}

TilesetHandle GraphicsManager::loadTileset(const char *data_file,
                                           render::TextureHandle texture)
{
	DEBUG_ASSERT(m_num_tilesets < m_tilesets.size(), "Too much tilesets!");
	DEBUG_ASSERT(m_num_atlases < m_atlases.size(), "Too much atlases!");
	auto allocation_mark = m_allocator.mark();
	loader::File file;
	if (file.open(data_file, "r") != 0) {
		DEBUG_ERROR("Can not open file!");
		return k_tileset_invalid;
	}
	loader::DataLoader loader(m_loader_arena, file);
	if (loader.begin("tile") != 0) {
		DEBUG_ERROR("Not a tileset file!");
		return k_tileset_invalid;
	}
	loader::Value values[10];
	for (int section = 0; section < 3; section++) {
		const char *name;
		int rows;
		int columns;
		if (loader.readSectionInfo(&name, rows, columns) != 0) {
			DEBUG_ERROR("Could not read section info!");
			return k_tileset_invalid;
		}
		if (std::strcmp(name, "images") == 0) {
			if (loader.skipRows(rows) != 0) {
				m_allocator.reset(allocation_mark);
				return k_tileset_invalid;
			}
		} else if (std::strcmp(name, "objects") == 0) {
			m_tilesets[m_num_tilesets].num_objects = rows;
			m_tilesets[m_num_tilesets].objects =
			    reinterpret_cast<TileObject *>(m_allocator.allocate_aligned(
			        sizeof(TileObject) * rows, alignof(TileObject)));
			for (int row = 0; row < rows; row++) {
				if (loader.readRow(values, columns) != 2) {
					m_allocator.reset(allocation_mark);
					DEBUG_ERROR("Could not read row!");
					return k_tileset_invalid;
				}
				m_tilesets[m_num_tilesets].objects[row].start_index =
				    static_cast<uint16_t>(values[0].number);
				m_tilesets[m_num_tilesets].objects[row].num_parts =
				    static_cast<uint16_t>(values[1].number);
			}
		} else if (std::strcmp(name, "tiles") == 0) {
			m_tilesets[m_num_tilesets].num_tiles = rows;
			m_atlases[m_num_atlases].num = rows;
			m_tilesets[m_num_tilesets].parts =
			    reinterpret_cast<TileObjectPart *>(m_allocator.allocate_aligned(
			        sizeof(TileObjectPart) * rows, alignof(TileObjectPart)));
			m_atlases[m_num_atlases].rects =
			    reinterpret_cast<Rect *>(m_allocator.allocate_aligned(
			        sizeof(Rect) * rows, alignof(Rect)));

			for (int row = 0; row < rows; row++) {
				if (loader.readRow(values, columns) != 6) {
					m_allocator.reset(allocation_mark);
					DEBUG_ERROR("Could not read row!");
					return k_tileset_invalid;
				}
				m_tilesets[m_num_tilesets].parts[row].x =
				    static_cast<uint8_t>(values[0].number);
				m_tilesets[m_num_tilesets].parts[row].y =
				    static_cast<uint8_t>(values[1].number);

				m_atlases[m_num_atlases].rects[row].x =
				    static_cast<uint16_t>(values[2].number);
				m_atlases[m_num_atlases].rects[row].y =
				    static_cast<uint16_t>(values[3].number);
				m_atlases[m_num_atlases].rects[row].width =
				    static_cast<uint16_t>(values[4].number);
				m_atlases[m_num_atlases].rects[row].height =
				    static_cast<uint16_t>(values[5].number);
			}
		} else {
			m_allocator.reset(allocation_mark);
			DEBUG_ERROR("Unknown section name!");
			return k_tileset_invalid;
		}
	}
	m_atlases[m_num_atlases].texture = texture;
	m_num_atlases++;

	m_tilesets[m_num_tilesets].atlas =
	    static_cast<TextureAtlasHandle>(m_num_atlases);
	m_num_tilesets++;

	return static_cast<TilesetHandle>(m_num_tilesets);
}

AnimationHandle GraphicsManager::loadAnimation(const char *data_file,
                                               render::TextureHandle texture)
{
	DEBUG_ASSERT(m_num_animations < m_animations.size(),
	             "Too much animations!");
	DEBUG_ASSERT(m_num_atlases < m_atlases.size(), "Too much atlases!");
	auto allocation_mark = m_allocator.mark();
	loader::File file;
	if (file.open(data_file, "r") != 0) {
		DEBUG_ERROR("Can not open file!");
		return k_animation_invalid;
	}
	loader::DataLoader loader(m_loader_arena, file);
	if (loader.begin("anim") != 0) {
		DEBUG_ERROR("Not a animation file!");
		return k_animation_invalid;
	}
	loader::Value values[10];
	for (int section = 0; section < 2; section++) {
		const char *name;
		int rows;
		int columns;
		if (loader.readSectionInfo(&name, rows, columns) != 0) {
			m_allocator.reset(allocation_mark);
			DEBUG_ERROR("Could not read section info!");
			return k_animation_invalid;
		}
		if (std::strcmp(name, "images") == 0) {
			m_atlases[m_num_atlases].num = rows;
			m_atlases[m_num_atlases].rects =
			    reinterpret_cast<Rect *>(m_allocator.allocate_aligned(
			        sizeof(Rect) * rows, alignof(Rect)));
			for (int row = 0; row < rows; row++) {
				if (loader.readRow(values, columns) != 4) {
					m_allocator.reset(allocation_mark);
					return k_animation_invalid;
				}
				m_atlases[m_num_atlases].rects[row].x =
				    static_cast<uint16_t>(values[0].number);
				m_atlases[m_num_atlases].rects[row].y =
				    static_cast<uint16_t>(values[1].number);
				m_atlases[m_num_atlases].rects[row].width =
				    static_cast<uint16_t>(values[2].number);
				m_atlases[m_num_atlases].rects[row].height =
				    static_cast<uint16_t>(values[3].number);
			}
		} else if (std::strcmp(name, "animation") == 0) {
			m_animations[m_num_animations].num_frames = rows;
			m_animations[m_num_animations].frame_pivots =
			    reinterpret_cast<Pivot *>(m_allocator.allocate_aligned(
			        sizeof(Pivot) * rows, alignof(Pivot)));
			for (int row = 0; row < rows; row++) {
				if (loader.readRow(values, columns) != 2) {
					DEBUG_ERROR("Could not read row!");
					m_allocator.reset(allocation_mark);
					return k_animation_invalid;
				}
				m_animations[m_num_animations].frame_pivots[row].x =
				    static_cast<uint16_t>(values[0].number);
				m_animations[m_num_animations].frame_pivots[row].y =
				    static_cast<uint16_t>(values[1].number);
			}
		} else {
			m_allocator.reset(allocation_mark);
			DEBUG_ERROR("Unknown section name!");
			return k_animation_invalid;
		}
	}
	m_atlases[m_num_atlases].texture = texture;
	m_num_atlases++;

	m_animations[m_num_animations].atlas =
	    static_cast<TextureAtlasHandle>(m_num_atlases);
	m_num_animations++;

	return static_cast<AnimationHandle>(m_num_animations);
}
TextureAtlasHandle GraphicsManager::loadGenericImageSet(
    const char *data_file, render::TextureHandle texture)
{
	DEBUG_ASSERT(m_num_atlases < m_atlases.size(), "Too much atlases!");
	auto allocation_mark = m_allocator.mark();
	loader::File file;
	if (file.open(data_file, "r") != 0) {
		DEBUG_ERROR("Can not open file!");
		return k_atlas_invalid;
	}
	loader::DataLoader loader(m_loader_arena, file);
	if (loader.begin(nullptr) != 0) {
		DEBUG_ERROR("Could not open file!");
		return k_atlas_invalid;
	}
	loader::Value values[10];
	const char *name;
	int rows;
	int columns;
	if (loader.readSectionInfo(&name, rows, columns) != 0) {
		m_allocator.reset(allocation_mark);
		DEBUG_ERROR("Could not read section info!");
		return k_atlas_invalid;
	}
	if (std::strcmp(name, "images") == 0) {
		m_atlases[m_num_atlases].num = rows;
		m_atlases[m_num_atlases].rects = reinterpret_cast<Rect *>(
		    m_allocator.allocate_aligned(sizeof(Rect) * rows, alignof(Rect)));
		for (int row = 0; row < rows; row++) {
			if (loader.readRow(values, columns) != 4) {
				m_allocator.reset(allocation_mark);
				return k_atlas_invalid;
			}
			m_atlases[m_num_atlases].rects[row].x =
			    static_cast<uint16_t>(values[0].number);
			m_atlases[m_num_atlases].rects[row].y =
			    static_cast<uint16_t>(values[1].number);
			m_atlases[m_num_atlases].rects[row].width =
			    static_cast<uint16_t>(values[2].number);
			m_atlases[m_num_atlases].rects[row].height =
			    static_cast<uint16_t>(values[3].number);
		}
	} else {
		m_allocator.reset(allocation_mark);
		DEBUG_ERROR("Unknown section name!");
		return k_atlas_invalid;
	}
	m_atlases[m_num_atlases].texture = texture;
	m_num_atlases++;
	return static_cast<TextureAtlasHandle>(m_num_atlases);
}

TextureAtlas *GraphicsManager::atlas(TextureAtlasHandle handle)
{
	return &m_atlases[static_cast<size_t>(handle) - 1];
}

Tileset *GraphicsManager::tileset(TilesetHandle handle)
{
	return &m_tilesets[static_cast<size_t>(handle) - 1];
}

Animation *GraphicsManager::animation(AnimationHandle handle)
{
	return &m_animations[static_cast<size_t>(handle) - 1];
}
}  // namespace graphics
}  // namespace castlekeep
