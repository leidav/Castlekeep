#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <cstddef>
#include <cstdint>

#define IMAGE_DEFAULT_MAX_HEIGHT 4096
#define IMAGE_DEFAULT_MAX_WIDTH 4096

#include <memory/linear_allocator.h>

namespace loaders
{
class ImageLoader
{
public:
	ImageLoader(const memory::MemoryArena &memory,
	            int max_width = IMAGE_DEFAULT_MAX_WIDTH,
	            int max_height = IMAGE_DEFAULT_MAX_HEIGHT);
	int load(std::byte image[], size_t size, int &width, int &height,
	         int &channels, const char *file);

private:
	memory::LinearAllocator m_allocator;
	int m_max_width;
	int m_max_height;
};
}  // namespace loaders
#endif
