#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include "renderer/renderer.h"
#include "world.h"

namespace core
{
class WorldRenderer
{
public:
	WorldRenderer(World *world);
	~WorldRenderer();
	int renderWorld();

private:
	World *m_world;
};
}  // namespace core
#endif
