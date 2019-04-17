#include "file.h"

#include <cstdio>
#include <cstring>

namespace castlekeep
{
namespace loader
{
File::File() : m_file(nullptr) {}
int File::open(const char* path, const char* mode)
{
	m_file = std::fopen(path, mode);
	if (m_file == nullptr) {
		return -1;
	}
	return 0;
}

void File::close()
{
	if (m_file != nullptr) {
		std::fclose(m_file);
	}
}

char* File::readLine(char* buffer, size_t size)
{
	return std::fgets(buffer, size, m_file);
}

size_t File::readBytes(std::byte* buffer, size_t num)
{
	return std::fread(buffer, 1, num, m_file);
}

File::~File()
{
	close();
}

int File::seek(size_t pos)
{
	return std::fseek(m_file, pos, SEEK_SET);
}

size_t File::size()
{
	size_t pos = std::ftell(m_file);
	std::fseek(m_file, 0, SEEK_END);
	size_t size = std::ftell(m_file);
	seek(pos);
	return size;
}

}  // namespace loader
}  // namespace castlekeep
