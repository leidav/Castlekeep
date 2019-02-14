#include "terrain.h"

Terrain::Terrain(int size) : m_size(size)
{
	m_heights.resize(m_size * m_size, 0);
	m_tiles.resize(m_size * m_size, 0);
}
Terrain::~Terrain() {}
