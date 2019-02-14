#ifndef TERRAIN_H
#define TERRAIN_H

#include <cstdint>
#include <vector>

class Terrain
{
public:
	Terrain(int size);
	~Terrain();

	int m_size;
	std::vector<uint8_t> m_heights;
	std::vector<uint16_t> m_tiles;
};
#endif
