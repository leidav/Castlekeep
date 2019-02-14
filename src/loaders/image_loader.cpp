#include "image_loader.h"

#include <memory/linear_allocator.h>

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

namespace loaders
{
ImageLoader::ImageLoader(const memory::MemoryArena &memory, int max_width,
                         int max_height)
    : m_allocator(memory), m_max_width(max_width), m_max_height(max_height)
{
}

int ImageLoader::load(std::byte image[], std::size_t buffer_size, int &width,
                      int &height, int &channels, const char *file)
{
	auto marker_guard = m_allocator.markGuarded();
	png_byte buffer[10];
	std::FILE *fp = fopen(file, "rb");
	if (fp == nullptr) {
		return -1;
	}
	size_t offset = std::fread(buffer, 1, 10, fp);
	if (!png_sig_cmp(buffer, 0, 10)) {
		return -1;
	}
	png_structp png_ptr =
	    png_create_read_struct_2(PNG_LIBPNG_VER_STRING, nullptr, nullptr,
	                             nullptr, &m_allocator, allocate, deallocate);
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

	if (width > m_max_width || height > m_max_height || size > buffer_size ||
	    interlace_type != PNG_INTERLACE_NONE) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return -1;
	}

	// static png_byte *row_pointers[MAX_HEIGHT];

	png_byte *row_pointer = reinterpret_cast<png_byte *>(image);
	for (int i = 0; i < height; i++) {
		png_read_row(png_ptr, row_pointer, nullptr);
		row_pointer += pitch;
		// row_pointers[i] = reinterpret_cast<png_byte *>(output) + (pitch * i);
	}
	// png_set_rows(png_ptr, info_ptr, row_pointers);

	// png_read_image(png_ptr, row_pointers);

	png_read_end(png_ptr, info_ptr);

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

	fclose(fp);

	return 0;
}
}  // namespace loaders
