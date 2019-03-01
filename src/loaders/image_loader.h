#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include "memory/linear_allocator.h"

#include <cstddef>
#include <cstdint>

namespace loader
{
enum { IMAGE_DEFAULT_MAX_HEIGHT = 4096, IMAGE_DEFAULT_MAX_WIDTH = 4096 };

int loadImage(std::byte image[], size_t size, int &width, int &height,
              int &channels, const char *file, const memory::Arena &arena,
              int max_width = IMAGE_DEFAULT_MAX_WIDTH,
              int max_height = IMAGE_DEFAULT_MAX_HEIGHT);
}  // namespace loader
#endif
