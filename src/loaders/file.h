#ifndef FILE_H
#define FILE_H

#include <cstddef>
#include <cstdio>

namespace castlekeep
{
namespace loader
{
class File
{
public:
	File();
	~File();
	int open(const char* path, const char* mode);
	void close();
	char* readLine(char* buffer, size_t size);
	size_t readBytes(std::byte* buffer, size_t num);
	int seek(size_t pos);
	size_t size();

private:
	FILE* m_file;
};

}  // namespace loader
}  // namespace castlekeep
#endif
