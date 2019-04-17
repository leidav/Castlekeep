#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include "memory/linear_allocator.h"

#include <cstddef>
#include <cstdint>

namespace castlekeep
{
namespace loader
{
constexpr int k_image_default_max_width = 2048;
constexpr int k_image_default_max_height = k_image_default_max_width * 4;

int loadImage(std::byte image[], size_t size, int &width, int &height,
              int &channels, const char *file, const memory::Arena &arena,
              int max_width = k_image_default_max_width,
              int max_height = k_image_default_max_height);
}  // namespace loader
}  // namespace castlekeep
#endif
