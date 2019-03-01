#include "image_loader.h"

#include "memory/linear_allocator.h"

#include <png.h>
#include <cstdio>
#include <cstring>

#define PNG_USER_MEM_SUPPORTED

static png_voidp allocate(png_structp png_ptr, png_alloc_size_t size)
{
	auto *allocator =
	    reinterpret_cast<memory::LinearAllocator *>(png_get_mem_ptr(png_ptr));
	return allocator->allocate_aligned(size, 8);
}

static void deallocate(png_structp png_ptr, png_voidp ptr)
{
	return;
}

namespace loader
{
int loadImage(std::byte image[], size_t buffer_size, int &width, int &height,
              int &channels, const char *file, const memory::Arena &arena,
              int max_width, int max_height)
{
	memory::LinearAllocator allocator(arena);
	auto marker_guard = allocator.markGuarded();
	png_byte buffer[10];
	std::FILE *fp = fopen(file, "rb");
	if (fp == nullptr) {
		return -1;
	}
	size_t offset = std::fread(buffer, 1, 8, fp);
	if (png_sig_cmp(buffer, 0, 8) != 0) {
		return -1;
	}
	png_structp png_ptr =
	    png_create_read_struct_2(PNG_LIBPNG_VER_STRING, nullptr, nullptr,
	                             nullptr, &allocator, allocate, deallocate);
	if (!png_ptr) {
		return -1;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		return -1;
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		return -1;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return -1;
	}
	png_init_io(png_ptr, fp);

	png_set_sig_bytes(png_ptr, static_cast<int>(offset));

	png_read_info(png_ptr, info_ptr);

	width = static_cast<int>(png_get_image_width(png_ptr, info_ptr));
	height = static_cast<int>(png_get_image_height(png_ptr, info_ptr));
	// png_byte color_type = png_get_color_type(png_ptr, info_ptr);
	channels = static_cast<int>(png_get_channels(png_ptr, info_ptr));
	png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);

	int pitch = width * channels;
	size_t size = static_cast<size_t>(pitch * height);

	if (width > max_width || height > max_height || size > buffer_size ||
	    interlace_type != PNG_INTERLACE_NONE) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return -1;
	}

	png_byte *row_pointer = reinterpret_cast<png_byte *>(image);
	for (int i = 0; i < height; i++) {
		png_read_row(png_ptr, row_pointer, nullptr);
		row_pointer += pitch;
	}

	png_read_end(png_ptr, info_ptr);

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

	fclose(fp);

	return 0;
}
}  // namespace loader
