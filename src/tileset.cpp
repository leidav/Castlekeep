#include "tileset.h"
#include "debug.h"
#include "loaders/data_loader.h"

#include <cstring>

namespace core
{
using namespace memory::literals;

TilesetManager::TilesetManager(const memory::Arena &arena, size_t max_tilesets)
    : m_allocator(arena),
      m_loader_arena(10_kib, m_allocator.allocate(10_kib)),
      m_num_tilesets(0),
      m_tilesets(max_tilesets, m_allocator)
{
}

TilesetManager::~TilesetManager() {}

TilesetHandle TilesetManager::createTileset(const char *data_file,
                                            render::TextureHandle texture)
{
	DEBUG_ASSERT(m_num_tilesets <= m_tilesets.size(), "Too much tilesets!");
	loader::DataLoader loader(m_loader_arena);
	loader.open(data_file);
	loader::Value values[10];
	for (int section = 0; section < 3; section++) {
		const char *name;
		int rows;
		int columns;
		if (loader.readSectionInfo(&name, rows, columns) != 0) {
			return 0;
		}
		if (std::strcmp(name, "images") == 0) {
			if (loader.skipRows(rows) != 0) {
				return 0;
			}
		} else if (std::strcmp(name, "objects") == 0) {
			m_tilesets[m_num_tilesets].num_objects = rows;
			m_tilesets[m_num_tilesets].objects =
			    reinterpret_cast<TileObject *>(m_allocator.allocate_aligned(
			        sizeof(TileObject) * rows, alignof(TileObject)));
			for (int row = 0; row < rows; row++) {
				if (loader.readRow(values, columns) != columns) {
					return 0;
				}
				m_tilesets[m_num_tilesets].objects[row].id =
				    static_cast<uint16_t>(values[0].number);
				m_tilesets[m_num_tilesets].objects[row].num_parts =
				    static_cast<uint16_t>(values[1].number);
				m_tilesets[m_num_tilesets].objects[row].num_parts =
				    static_cast<uint16_t>(values[2].number);
			}
		} else if (std::strcmp(name, "tiles") == 0) {
			m_tilesets[m_num_tilesets].num_tiles = rows;
			m_tilesets[m_num_tilesets].atlas.num = rows;
			m_tilesets[m_num_tilesets].parts =
			    reinterpret_cast<TileObjectPart *>(m_allocator.allocate_aligned(
			        sizeof(TileObjectPart) * rows, alignof(TileObjectPart)));
			m_tilesets[m_num_tilesets].atlas.rects =
			    reinterpret_cast<Rect *>(m_allocator.allocate_aligned(
			        sizeof(Rect) * rows, alignof(Rect)));

			for (int row = 0; row < rows; row++) {
				if (loader.readRow(values, columns) != columns) {
					return 0;
				}
				m_tilesets[m_num_tilesets].parts[row].x =
				    static_cast<uint8_t>(values[1].number);
				m_tilesets[m_num_tilesets].parts[row].y =
				    static_cast<uint8_t>(values[2].number);
				m_tilesets[m_num_tilesets].atlas.rects[row].x =
				    static_cast<uint16_t>(values[3].number);
				m_tilesets[m_num_tilesets].atlas.rects[row].y =
				    static_cast<uint16_t>(values[4].number);
				m_tilesets[m_num_tilesets].atlas.rects[row].width =
				    static_cast<uint16_t>(values[5].number);
				m_tilesets[m_num_tilesets].atlas.rects[row].height =
				    static_cast<uint16_t>(values[6].number);
			}
		} else {
			return 0;
		}
	}
	m_tilesets[m_num_tilesets].atlas.texture = texture;
	return static_cast<TilesetHandle>(++m_num_tilesets);
}

Tileset *TilesetManager::tileset(TilesetHandle handle)
{
	return &m_tilesets[static_cast<size_t>(handle) - 1];
}
}  // namespace core
