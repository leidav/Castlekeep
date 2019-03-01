#include "engine.h"

#include "loaders/data_loader.h"
#include "platform/sdl_platform.h"
#include "utils/memory/allocator.h"

namespace core
{
using namespace memory::literals;
Engine::Engine()
    : m_memory(1024_mib),
      m_allocator(m_memory.memory()),
      m_platform(nullptr),
      m_render_system(
          memory::makeUnique<render::Renderer, memory::LinearAllocator>(
              nullptr, &m_allocator)),
      m_tileset_system(memory::Arena(m_allocator, 100_mib), 100)
{
}

int Engine::start()
{
	m_platform = memory::createObject<platform::SDLPlatform>(
	    m_allocator, memory::Arena(m_allocator, 1_mib));
	m_platform->init();
	m_platform->createWindow(1024, 768, "Castlekeep");
	m_render_system = m_platform->createRenderSystem(
	    memory::Arena(m_allocator, 100_mib), 100);
	auto marker = m_allocator.markGuarded();
	size_t buffer_size =
	    loader::IMAGE_DEFAULT_MAX_WIDTH * loader::IMAGE_DEFAULT_MAX_HEIGHT * 4;
	std::byte* image_buffer =
	    reinterpret_cast<std::byte*>(m_allocator.allocate(buffer_size));

	int width = 0;
	int height = 0;
	int channels = 0;
	if (loader::loadImage(image_buffer, buffer_size, width, height, channels,
	                      "data/terrain/tile_land8.png",
	                      memory::Arena(m_allocator, 1_mib)) != 0) {
		fprintf(stderr, "image loading failed\n");
		std::exit(1);
	}
	render::TextureHandle texture = m_render_system->createTexture(
	    width, height, render::PixelFormat::ARGB8888, image_buffer);
	if (texture == render::TEXTURE_INVALID) {
		fprintf(stderr, "texture creation failed\n");
		std::exit(1);
	}
	m_tileset =
	    m_tileset_system.createTileset("data/terrain/tile_land8.data", texture);
	if (m_tileset == TILESET_INVALID) {
		fprintf(stderr, "tileset creation failed\n");
		std::exit(1);
	}
	return 0;
}

int Engine::loop()
{
	Tileset* tileset = m_tileset_system.tileset(m_tileset);
	while (m_platform->processEvents()) {
		auto marker = m_allocator.mark();
		render::DrawCommandBuffer commands;
		commands.commands =
		    memory::createObject<render::DrawCommand>(m_allocator);
		commands.length = 1;
		commands.commands[0].src_x = tileset->atlas.rects[0].x;
		commands.commands[0].src_y = tileset->atlas.rects[0].y;
		commands.commands[0].dst_x = 0;
		commands.commands[0].dst_y = 0;
		commands.commands[0].width = tileset->atlas.rects[0].width;
		commands.commands[0].height = tileset->atlas.rects[1].height;
		commands.texture_id = tileset->atlas.texture;
		m_render_system->startFrame();
		m_render_system->draw(commands);
		m_render_system->endFrame();
		m_allocator.reset(marker);
	}
	return 0;
}

int Engine::shutdown()
{
	return 0;
}
};  // namespace core
